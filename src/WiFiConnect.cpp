#if defined(ESP8266) || defined(ESP32)

#include "LedEffects.h"
#include <Controls.h>
#include <CtrlWiFi.h>
#include <CtrlWebSrv.h>
#include <EEPROMCfg.h>

#include "WiFiConnect.h"
#include "EffectEngineCtx.h"



///////////////////////
//Default host name
void getHostDefaultName(char *name){
//Configure name
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);  

  sprintf(name, "LED-%X%X%X", mac[3], mac[4], mac[5]);
}

///////////////////////
// WiFiConnection

WiFiConnection::WiFiConnection(){     
  //See if wifi should be on
  readConfig();

  //Host name and AP ssid are the same
  initWiFi(_wcn.wifiAP.ssid, _wcn.wifiAP.ssid);

  //Try to connect to wifi
  if(_wcn.wifiConnect.ssid[0] != 0)
    connectWiFi(_wcn.wifiConnect);    

}

bool WiFiConnection::onCmd(struct CtrlQueueItemEx &itm){

  switch(itm.cmd){
    case EEMC_WIFI_STATUS: // Retreieve current wifi status
      itm.ntf.put(CmdResponse<WIFI_STATUS>{itm.cmd});      
    break;

    case EEMC_WIFI_STATUS_CHANGE: { //Change of WiFi status 
      if(WiFi.status() == WL_CONNECTED){
        //Save last known configutation
        writeConfig();
      }

      itm.ntf.put(CmdResponse<WIFI_STATUS_STATION>{itm.cmd});  
    }
    break;

    case EEMC_WIFI_CONNECT:{ //Connect to WiFi network                
      _wcn.wifiConnect = *(WIFI_CONNECT *)itm.data.str;          
      connectWiFi(_wcn.wifiConnect);
      itm.ntf.put(CmdResponse<>{itm.cmd, EEER_DELAYED});     
    }    
    break;

    case EEMC_WIFI_DISCONNECT: //Disconnect 
      WiFi.disconnect(true);    
      itm.ntf.put(CmdResponse<>{itm.cmd, EEER_DELAYED});        
    break;
    
    case EEMC_WIFI_AP_ON:{ //Connect AP                                 
      connectWiFiAP(_wcn.wifiAP);
      enableCaptivePortalDNS(true);      
      itm.ntf.put(CmdResponse<WIFI_STATUS_AP>{itm.cmd}); 
    }
    break;

    case EEMC_WIFI_AP_OFF: //Disconnect AP 
      enableCaptivePortalDNS(false);
      WiFi.softAPdisconnect(true);
      itm.ntf.put(CmdResponse<WIFI_STATUS_AP>{itm.cmd});       
    break;
    
    case EEMC_WIFI_SCAN:{
      switch(WiFi.scanComplete()){
        case WIFI_SCAN_FAILED:
          WiFi.scanNetworks(true, true);
          itm.ntf.put(CmdResponse<>{itm.cmd, EEER_DELAYED});    
        break;
        case WIFI_SCAN_RUNNING:
          itm.ntf.put(CmdResponse<>{itm.cmd, EEER_DELAYED});    
        break;
        default:
          itm.ntf.put(CmdResponse<WIFI_SCAN>{itm.cmd});        
          WiFi.scanDelete();
        break;
      }
    }
    break;

    case EEMC_WIFI_CFG_GET: {      
      //Send reponse      
      CmdResponse<const WIFI_CONFIG_ALL &> resp{itm.cmd, _wcn};
      itm.ntf.put(resp);              
    }
    break;      

    case EEMC_WIFI_CFG_CLEAR:{
      memset(&_wcn, 0, sizeof(_wcn));
      writeConfig();
      readConfig();

      //Send reponse      
      CmdResponse<const WIFI_CONFIG_ALL &> resp{itm.cmd, _wcn};
      itm.ntf.put(resp);              
    }
    break;

    default: 
    return false;
  }

  return true;
}



bool WiFiConnection::writeConfig(){
  
  EEPROMCfg ee(EE_WIFI_IDX);  
  
  //Write all
  ee << (uint8_t)EE_VERSION << _wcn;
  
  return true;
}

bool WiFiConnection::readConfig(){

  EEPROMCfg ee(EE_WIFI_IDX);

  memset(&_wcn, 0, sizeof(_wcn));

  //Check version
  uint8_t version;
  ee >> version;

  if(version == EE_VERSION){
    //Read all
    ee >> _wcn;  
  }
  
  //AP configuration is always static
  getHostDefaultName(_wcn.wifiAP.ssid);
  _wcn.wifiAP.ipaddress  = IPAddress(172, 16, 25, 25);
  _wcn.wifiAP.gateway    = IPAddress(172, 16, 25, 25);
  _wcn.wifiAP.subnetMask = IPAddress(255, 255, 255, 0);

  return true;
}


////////////////////////////////////
// Web Server functions

//Web server initialization
void initWebServer(uint16_t port){
  LittleFS.begin();
  webServer.begin(port);

  //Enable CORS
  webServer.enableCORS(true);  

  //API
  ADD_API_REQUEST_HANDLER(HTTP_GET, "/api");

  webServer.serveStatic("/", LittleFS, "/web/");
  webServer.serveStatic("/favicon.ico", LittleFS, "/web/favicon.ico");

  //Not found - captive portal
  webServer.onNotFound([](){
    DBG_OUTLN("URI %s, method %d", webServer.uri().c_str(), (int)webServer.method());

    //Captive portal
    if(!handleCaptivePortal("/welcome.html")){        
      webServer.send(404);
    }
  });
}


#endif