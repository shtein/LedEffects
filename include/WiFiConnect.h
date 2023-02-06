#ifndef __WIFI_CONNECT_H
#define __WIFI_CONNECT_H

class EEPROMCfg;

//Only for supported hardware
#if defined(ESP8266) || defined(ESP32)
class WiFiConnection{
public:
  WiFiConnection();

  //Command processing
  bool onCmd(struct CtrlQueueItemEx &itm);

protected:
  //Read and write config
  bool writeConfig(const WIFI_CONNECT &wcn);
  bool readConfig(WIFI_CONNECT &wcn);
};

#endif

#endif //__WIFI_CONNECT_H