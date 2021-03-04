

int getRelayNum(int sensorId) {

  if (sensorId > -1) {
    for (int relayNum = 0; relayNum < gNumberOfRelays; relayNum++) {
      RelayConfigDef relayConfig = {};
      PROGMEM_readAnything(&gRelayConfig[relayNum], relayConfig);
      if (relayConfig.sensorId == sensorId) return(relayNum);
    }
  }
  return(-1);
}
