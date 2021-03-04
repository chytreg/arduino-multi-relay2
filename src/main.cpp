#include <Arduino.h>
#include <assert.h>
#include <EEPROM.h>
#include <Relay.h>
#include <Button.h>
#include <PROGMEM_readAnything.h>
// #define MY_DEBUG
// #define MY_SPLASH_SCREEN_DISABLED
// #define MY_DISABLE_RAM_ROUTING_TABLE_FEATURE
#define MY_PJON
#define MY_NODE_ID 1
#define MY_TRANSPORT_WAIT_READY_MS 1
// #define MY_GATEWAY_SERIAL
#include <MySensors.h>

using namespace lkankowski;

#define xstr(a) str(a)
#define str(a) #a
const char * MULTI_RELAY_VERSION = xstr(SKETCH_VERSION);

#if defined(EXPANDER_PCF8574) || defined(EXPANDER_MCP23017)
  #if defined(EXPANDER_PCF8574)
    #include "PCF8574.h"
    #define EXPANDER_PINS 8
  #elif defined(EXPANDER_MCP23017)
    #include "Adafruit_MCP23017.h"
    #define EXPANDER_PINS 16
  #endif
  #define USE_EXPANDER
  #include <Wire.h>    // Required for I2C communication
  #define E(expanderNo, ExpanderPin) (((expanderNo+1)<<8) | (ExpanderPin))
#endif

// Configuration in separate file
#include "config.h"

const int gNumberOfRelays = 8; //sizeof(gRelayConfig) / sizeof(RelayConfigDef);
const int gNumberOfButtons = 8; //sizeof(gButtonConfig) / sizeof(ButtonConfigDef);
#ifdef USE_EXPANDER
  const int gNumberOfExpanders = sizeof(expanderAddresses);
  #if defined(PCF8574_H)
    PCF8574 gExpander[gNumberOfExpanders];
  #elif defined(_Adafruit_MCP23017_H_)
    Adafruit_MCP23017 gExpander[gNumberOfExpanders];
  #endif
#endif

#ifdef DEBUG_STATS
  bool debugStatsOn = false;
  int loopCounter = 0;
  unsigned long loopInterval = 0;
  unsigned long loopCumulativeMillis = 0;
#endif
#ifdef DEBUG_STARTUP
  unsigned long debugCounter = 0;
#endif



MyMessage myMessage; // MySensors - Sending Data
#if defined(DEBUG_COMMUNICATION) || defined(DEBUG_STATS)
  MyMessage debugMessage(255, V_TEXT);
#endif
Relay gRelay[gNumberOfRelays];
lkankowski::Button gButton[gNumberOfButtons];
#include <common.h>
void(* resetFunc) (void) = 0; //declare reset function at address 0


// MySensors - This will execute before MySensors starts up
void before() {
  delay(10);
  Serial.begin(115200);
  // Serial.println("before");

  // Serial.println("debug21");
  // validate config
  #ifdef USE_EXPANDER
    for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
      // Serial.println("debug22");
            RelayConfigDef relayConfig = {};
      PROGMEM_readAnything(&gRelayConfig[relayNum], relayConfig);
      int pin = relayConfig.relayPin;
      Serial.println(printf_P(PSTR("PIN: %i"), pin));
      if (pin & 0xff00) {
        if (((pin >> 8) > gNumberOfExpanders) || ((pin & 0xff) >= EXPANDER_PINS)) {
          Serial.println(printf_P(PSTR("Configuration failed - expander no or number of pins out of range for relay: %i"), relayNum));
          delay(1000);
          assert(0);
        }
      }
    }
  #endif
  // Serial.println("debug23");
  for (int buttonNum = 0; buttonNum < gNumberOfButtons; buttonNum++) {
     ButtonConfigDef buttonConfig = {};
      PROGMEM_readAnything(&gButtonConfig[buttonNum], buttonConfig);
    #ifdef USE_EXPANDER
      int pin = buttonConfig.buttonPin;
      if (pin & 0xff00) {
        if (((pin >> 8) > gNumberOfExpanders) || ((pin & 0xff) >= EXPANDER_PINS)) {
          Serial.println(String(F("Configuration failed - expander no or number of pins out of range for button: ")) + buttonNum);
          delay(1000);
          assert(0);
        }
      }
    #endif
    const char * failAction[] = {"OK", "click", "long-press", "double-click"};
    int fail = 0;
    if ((buttonConfig.clickRelayId != -1) && (getRelayNum(buttonConfig.clickRelayId) == -1)) fail = 1;
    if ((buttonConfig.longClickRelayId != -1) && (getRelayNum(buttonConfig.longClickRelayId) == -1)) fail = 2;
    if ((buttonConfig.doubleClickRelayId != -1) && (getRelayNum(buttonConfig.doubleClickRelayId) == -1)) fail = 3;
    if (fail) {
        Serial.println(printf_P(PSTR("Configuration failed - invalid '%s relay ID' for button: %i"), failAction[fail], buttonNum));
        // Serial.println(printf_P(PSTR("Configuration failed - expander no or number of pins out of range for relay: %i"), relayNum));

        delay(1000);
        assert(0);
    }
    // TODO: validate if pin is correct to the current board
  }

  // if version has changed, reset state of all relays
  bool versionChangeResetState = (CONFIG_VERSION == EEPROM.read(0) ) ? false : true;

  #ifdef USE_EXPANDER
    /* Start I2C bus and PCF8574 instance */
    for(int i = 0; i < gNumberOfExpanders; i++) {
      gExpander[i].begin(expanderAddresses[i]);
    }

    Relay::expanderInit(gExpander);
    lkankowski::Button::expanderInit(gExpander);
  #endif

  // initialize relays
  Relay::setImpulseInterval(RELAY_IMPULSE_INTERVAL);

  for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {

      RelayConfigDef relayConfig = {};
      PROGMEM_readAnything(&gRelayConfig[relayNum], relayConfig);

    gRelay[relayNum].initialize(relayNum, relayConfig.sensorId, relayConfig.relayDescription);
    gRelay[relayNum].attachPin(relayConfig.relayPin);
    gRelay[relayNum].setModeAndStartupState(relayConfig.relayOptions, versionChangeResetState);
    gRelay[relayNum].start();
  }
  if (versionChangeResetState) {
    // version has changed, so store new version in eeporom
    EEPROM.write(0, CONFIG_VERSION);
  }
}; // before()

// executed AFTER mysensors has been initialised
void setup() {
  // Send state to MySensor Gateway
  myMessage.setType(V_STATUS);
  for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
    myMessage.setSensor(gRelay[relayNum].getSensorId());
    send(myMessage.set(gRelay[relayNum].getState())); // send current state
  }

  // Setup buttons
  lkankowski::Button::Button::setEventIntervals(BUTTON_DOUBLE_CLICK_INTERVAL, BUTTON_LONG_PRESS_INTERVAL);
  lkankowski::Button::Button::setMonoStableTrigger(MONO_STABLE_TRIGGER);

  for (int buttonNum = 0; buttonNum < gNumberOfButtons; buttonNum++) {
      ButtonConfigDef buttonConfig = {};
      PROGMEM_readAnything(&gButtonConfig[buttonNum], buttonConfig);
    gButton[buttonNum].initialize(buttonConfig.buttonType, buttonConfig.buttonDescription);
    gButton[buttonNum].setAction((buttonConfig.clickRelayId),
                                 (buttonConfig.longClickRelayId),
                                 (buttonConfig.doubleClickRelayId));
    gButton[buttonNum].setDebounceInterval(BUTTON_DEBOUNCE_INTERVAL);
    gButton[buttonNum].attachPin(buttonConfig.buttonPin);
  }
};

void loop() {
  // Serial.println("loop");
  // delay(10);
  #ifdef DEBUG_STATS
    unsigned long loopStartMillis = millis();
    if (loopCounter == 0) {
      loopCumulativeMillis = 0;
      loopInterval = loopStartMillis;
    }
  #endif

  for (int buttonNum = 0; buttonNum < gNumberOfButtons; buttonNum++) {

    int relayNum = gButton[buttonNum].updateAndGetRelayNum();
    if (relayNum > -1) {
      // mono/bi-stable button toggles the relay, ding-dong/reed-switch switch to exact state
      bool relayState = gButton[buttonNum].getRelayState(gRelay[relayNum].getState());

      #ifdef IGNORE_BUTTONS_START_MS
        if (millis() > IGNORE_BUTTONS_START_MS) {
      #endif
          if (gRelay[relayNum].changeState(relayState)) {
            myMessage.setSensor(gRelay[relayNum].getSensorId());
            send(myMessage.set(relayState));
          }
      #ifdef IGNORE_BUTTONS_START_MS
        }
      #endif
    }
  }

  if (Relay::isImpulsePending()) {
    for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
      if (gRelay[relayNum].impulseProcess()) {
        myMessage.setSensor(gRelay[relayNum].getSensorId());
        send(myMessage.set(0));
      }
    }
  }

  #ifdef DEBUG_STATS
    if (debugStatsOn) {
      unsigned long loopIntervalCurrent = millis();
      loopCumulativeMillis += loopIntervalCurrent - loopStartMillis;
      loopCounter++;
      if ( loopCounter > DEBUG_STATS) {
        Serial.println(String(F("# ")) + DEBUG_STATS + F(" loop stats: (end-start)=") + (loopIntervalCurrent - loopInterval)
                       + F("ms, cumulative_loop_millis=") + loopCumulativeMillis + F("ms"));
        loopCounter = 0;
        debugStatsOn = false;
      }
    }
  #endif
};


// MySensors - Presentation - Your sensor must first present itself to the controller.
// Executed after "before()" and before "setup()"
void presentation() {
  sendSketchInfo(MULTI_RELAY_DESCRIPTION, MULTI_RELAY_VERSION );

  // Register every relay as separate sensor
  for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
    present(gRelay[relayNum].getSensorId(), S_BINARY, gRelay[relayNum].getDescription());
  }
};


// MySensors - Handling incoming messages
// Nodes that expects incoming data, must implement the receive() function to handle the incoming messages.
void receive(const MyMessage &message) {

  #ifdef DEBUG_COMMUNICATION
    Serial.println(String(F("# Incoming message: sensorId=")) + message.getSensor() + F(", command=") + message.getCommand()
                 + F(", ack=") + message.isAck() + F(", echo=") + message.isEcho() + F(", type=") + message.getType()
                 + F(", payload=") + message.getString());
  #endif
  if (message.getCommand() == C_SET) {
    if (message.getType() == V_STATUS) {
      int relayNum = getRelayNum(message.getSensor());
      if (relayNum == -1) return;
      gRelay[relayNum].changeState(message.getBool());
      myMessage.setSensor(message.getSensor());
      send(myMessage.set(message.getBool())); // support for OPTIMISTIC=FALSE (Home Asistant)
    #ifdef DEBUG_STATS
    } else if (message.getType() == V_VAR1) {
      int debugCommand = message.getInt();
      if (debugCommand == 1) {
        debugStatsOn = message.getBool();
        loopCounter = 0;
        send(debugMessage.set(F("toogle debug stats")));
      } else if (debugCommand == 2) {
        for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
          Serial.println(String(F("# Sensor ")) + gRelay[relayNum].getSensorId() + F(" state=")
                         + gRelay[relayNum].getState() + "; " + gRelay[relayNum].getDescription());
        }
      } else if (debugCommand == 3) {
        for (int buttonNum = 0; buttonNum < gNumberOfButtons; buttonNum++) {
          Serial.println(String(F("# Button ")) + buttonNum + F(" state=") + gButton[buttonNum].getState()
                         + F("; ") + gButton[buttonNum].getDescription());
        }
      } else if (debugCommand == 4) { // dump EEPROM
        Serial.print(F("# Dump EEPROM: "));
        for (int relayNum = 0; relayNum < gNumberOfRelays+RELAY_STATE_STORAGE; relayNum++) {
          Serial.print(EEPROM.read(relayNum));
          Serial.print(F(","));
        }
        Serial.println();
      } else if (debugCommand == 5) { // clear EEPROM & reset
        for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
          EEPROM.write(RELAY_STATE_STORAGE + relayNum, 0);
        }
        resetFunc();
      } else if (debugCommand == 6) { // reset
        resetFunc();
      }
    #endif
    }
  }
};
