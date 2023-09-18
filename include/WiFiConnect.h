#ifndef __WIFI_CONNECT_H
#define __WIFI_CONNECT_H


//Only for supported hardware
#if defined(ESP8266) || defined(ESP32)

//Wifi connection control
class WiFiConnection{
public:
  WiFiConnection();

  //Command processing
  bool onCmd(struct CtrlQueueItemEx &itm);

protected:
  //Read and write config
  bool writeConfig();
  bool readConfig();

protected:
  WIFI_CONFIG_ALL _wcn;
};

//Web server initialization
void initWebServer(uint16_t port);

#endif

#endif //__WIFI_CONNECT_H