// For mono-stable buttons it will trigger state: LOW - when button is pressed, HIGH - when button is released
const uint8_t MONO_STABLE_TRIGGER = LOW;

// time interval for RELAY_IMPULSE type relay, ignored when button type is DING_DONG or REED_SWITCH
const unsigned long RELAY_IMPULSE_INTERVAL = 250;

// debounce interval in milliseconds
const unsigned long BUTTON_DEBOUNCE_INTERVAL = 50;

// double-click interval in milliseconds
const unsigned long BUTTON_DOUBLE_CLICK_INTERVAL = 350;

// long-press interval in milliseconds
const unsigned long BUTTON_LONG_PRESS_INTERVAL = 800;

#ifdef USE_EXPANDER
  uint8_t expanderAddresses[] = {0};
#endif

const char MULTI_RELAY_DESCRIPTION[] = "Oświetlenie moduł 1";

// Relays config - ID and PIN MUST BE UNIQUE (can't be repeated)!
// Row params: relay ID - [0-254] sensor ID reported on MySensor Gateway
//             relay pin - pin used for relay, can be expander pin via "E(x, y)" macro
//             relay options - [RELAY_TRIGGER_LOW|RELAY_TRIGGER_HIGH] {RELAY_STARTUP_ON|RELAY_STARTUP_OFF} {RELAY_IMPULSE}
//             relay description - reported on MySensor Gateway, can help identify device on initial configuration in Home Automation App, can be empty ("")
const RelayConfigDef gRelayConfig[] PROGMEM = {
  {1, E(0,8),  RELAY_TRIGGER_HIGH, "Światło 1"},
  {2, E(0,9),  RELAY_TRIGGER_HIGH, "Światło 2"},
  {3, E(0,10), RELAY_TRIGGER_HIGH, "Światło 3"},
  {4, E(0,11), RELAY_TRIGGER_HIGH, "Światło 4"},
  {5, E(0,12), RELAY_TRIGGER_HIGH, "Światło 5"},
  {6, E(0,13), RELAY_TRIGGER_HIGH, "Światło 6"},
  {7, E(0,14), RELAY_TRIGGER_HIGH, "Światło 7"},
  {8, E(0,15), RELAY_TRIGGER_HIGH, "Światło 8"},
};

// Buttons config
// Row params: button pin - pin used for button, can be expander pin (but NOT RECOMMENDED) via "E(x, y)" macro
//             button type - [MONO_STABLE|BI_STABLE|DING_DONG|REED_SWITCH]
//             click action relay ID - MUST be defined in gRelayConfig
//             long-press action relay ID - ignored for BI_STABLE, DING_DONG, REED_SWITCH
//             double-click action relay ID - ignored for DING_DONG, REED_SWITCH
//             button description - debug only, can be empty ("")
const ButtonConfigDef gButtonConfig[] PROGMEM = {
  {E(0,0), MONO_STABLE, 1, -1, -1, "Przycisk 1"},
  {E(0,1), MONO_STABLE, 2, -1, -1, "Przycisk 2"},
  {E(0,2), MONO_STABLE, 3, -1, -1, "Przycisk 3"},
  {E(0,3), MONO_STABLE, 4, -1, -1, "Przycisk 4"},
  {E(0,4), MONO_STABLE, 5, -1, -1, "Przycisk 5"},
  {E(0,5), MONO_STABLE, 6, -1, -1, "Przycisk 6"},
  {E(0,6), MONO_STABLE, 7, -1, -1, "Przycisk 7"},
  {E(0,7), MONO_STABLE, 8, -1, -1, "Przycisk 8"},
};
