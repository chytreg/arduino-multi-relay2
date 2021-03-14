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
  uint8_t expanderAddresses[] = {0, 1};
#endif

const char MULTI_RELAY_DESCRIPTION[] = "Oświetlenie moduł 2";

// Relays config - ID and PIN MUST BE UNIQUE (can't be repeated)!
// Row params: relay ID - [0-254] sensor ID reported on MySensor Gateway
//             relay pin - pin used for relay, can be expander pin via "E(x, y)" macro
//             relay options - [RELAY_TRIGGER_LOW|RELAY_TRIGGER_HIGH] {RELAY_STARTUP_ON|RELAY_STARTUP_OFF} {RELAY_IMPULSE}
//             relay description - reported on MySensor Gateway, can help identify device on initial configuration in Home Automation App, can be empty ("")
const RelayConfigDef gRelayConfig[] PROGMEM = {
  {11, 24, RELAY_TRIGGER_HIGH, "Światło 11"},
  {12, 25, RELAY_TRIGGER_HIGH, "Światło 12"},
  {13, 26, RELAY_TRIGGER_HIGH, "Światło 13"},
  {14, 27, RELAY_TRIGGER_HIGH, "Światło 14"},
  {15, 28, RELAY_TRIGGER_HIGH, "Światło 15"},
  {16, 29, RELAY_TRIGGER_HIGH, "Światło 16"},
  {17, 30, RELAY_TRIGGER_HIGH, "Światło 17"},
  {18, 31, RELAY_TRIGGER_HIGH, "Światło 18"},

  {21, 2,  RELAY_TRIGGER_HIGH, "Światło 21"},
  {22, 5,  RELAY_TRIGGER_HIGH, "Światło 22"},
  {23, 4,  RELAY_TRIGGER_HIGH, "Światło 23"},
  {24, 7,  RELAY_TRIGGER_HIGH, "Światło 24"},
  {25, 6,  RELAY_TRIGGER_HIGH, "Światło 25"},
  {26, 9,  RELAY_TRIGGER_HIGH, "Światło 26"},
  {27, 8,  RELAY_TRIGGER_HIGH, "Światło 27"},
  {28, 11, RELAY_TRIGGER_HIGH, "Światło 28"},

  {31, E(0,8),  RELAY_TRIGGER_HIGH, "Światło 31"},
  {32, E(0,9),  RELAY_TRIGGER_HIGH, "Światło 32"},
  {33, E(0,10), RELAY_TRIGGER_HIGH, "Światło 33"},
  {34, E(0,11), RELAY_TRIGGER_HIGH, "Światło 34"},
  {35, E(0,12), RELAY_TRIGGER_HIGH, "Światło 35"},
  {36, E(0,13), RELAY_TRIGGER_HIGH, "Światło 36"},
  {37, E(0,14), RELAY_TRIGGER_HIGH, "Światło 37"},
  {38, E(0,15), RELAY_TRIGGER_HIGH, "Światło 38"},

  {41, E(1,8),  RELAY_TRIGGER_HIGH, "Światło 41"},
  {42, E(1,9),  RELAY_TRIGGER_HIGH, "Światło 42"},
  {43, E(1,10), RELAY_TRIGGER_HIGH, "Światło 43"},
  {44, E(1,11), RELAY_TRIGGER_HIGH, "Światło 44"},
  {45, E(1,12), RELAY_TRIGGER_HIGH, "Światło 45"},
  {46, E(1,13), RELAY_TRIGGER_HIGH, "Światło 46"},
  {47, E(1,14), RELAY_TRIGGER_HIGH, "Światło 47"},
  {48, E(1,15), RELAY_TRIGGER_HIGH, "Światło 48"},
};

// Buttons config
// Row params: button pin - pin used for button, can be expander pin (but NOT RECOMMENDED) via "E(x, y)" macro
//             button type - [MONO_STABLE|BI_STABLE|DING_DONG|REED_SWITCH]
//             click action relay ID - MUST be defined in gRelayConfig
//             long-press action relay ID - ignored for BI_STABLE, DING_DONG, REED_SWITCH
//             double-click action relay ID - ignored for DING_DONG, REED_SWITCH
//             button description - debug only, can be empty ("")
const ButtonConfigDef gButtonConfig[] PROGMEM = {
  {47, MONO_STABLE, 11, -1, -1, "Przycisk 11"},
  {45, MONO_STABLE, 12, -1, -1, "Przycisk 12"},
  {43, MONO_STABLE, 13, -1, -1, "Przycisk 13"},
  {41, MONO_STABLE, 14, -1, -1, "Przycisk 14"},
  {39, MONO_STABLE, 15, -1, -1, "Przycisk 15"},
  {37, MONO_STABLE, 16, -1, -1, "Przycisk 16"},
  {35, MONO_STABLE, 17, -1, -1, "Przycisk 17"},
  {33, MONO_STABLE, 18, -1, -1, "Przycisk 18"},

  {46, MONO_STABLE, 21, -1, -1, "Przycisk 21"},
  {44, MONO_STABLE, 22, -1, -1, "Przycisk 22"},
  {42, MONO_STABLE, 23, -1, -1, "Przycisk 23"},
  {40, MONO_STABLE, 24, -1, -1, "Przycisk 24"},
  {38, MONO_STABLE, 25, -1, -1, "Przycisk 25"},
  {36, MONO_STABLE, 26, -1, -1, "Przycisk 26"},
  {34, MONO_STABLE, 27, -1, -1, "Przycisk 27"},
  {32, MONO_STABLE, 28, -1, -1, "Przycisk 28"},

  {E(0,0), MONO_STABLE, 31, -1, -1, "Przycisk 31"},
  {E(0,1), MONO_STABLE, 32, -1, -1, "Przycisk 32"},
  {E(0,2), MONO_STABLE, 33, -1, -1, "Przycisk 33"},
  {E(0,3), MONO_STABLE, 34, -1, -1, "Przycisk 34"},
  {E(0,4), MONO_STABLE, 35, -1, -1, "Przycisk 35"},
  {E(0,5), MONO_STABLE, 36, -1, -1, "Przycisk 36"},
  {E(0,6), MONO_STABLE, 37, -1, -1, "Przycisk 37"},
  {E(0,7), MONO_STABLE, 38, -1, -1, "Przycisk 38"},

  {E(1,0), MONO_STABLE, 41, -1, -1, "Przycisk 41"},
  {E(1,1), MONO_STABLE, 42, -1, -1, "Przycisk 42"},
  {E(1,2), MONO_STABLE, 43, -1, -1, "Przycisk 43"},
  {E(1,3), MONO_STABLE, 44, -1, -1, "Przycisk 44"},
  {E(1,4), MONO_STABLE, 45, -1, -1, "Przycisk 45"},
  {E(1,5), MONO_STABLE, 46, -1, -1, "Przycisk 46"},
  {E(1,6), MONO_STABLE, 47, -1, -1, "Przycisk 47"},
  {E(1,7), MONO_STABLE, 48, -1, -1, "Przycisk 48"},
};
