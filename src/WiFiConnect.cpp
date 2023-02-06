#include "LedEffects.h"
#include <Controls.h>
#include <CtrlWiFi.h>
#include <EEPROMCfg.h>

#include "WiFiConnect.h"
#include "EffectEngineCtx.h"


///////////////////////
//AP Name

String APName(){
//Configure name
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);

  String apName = "LEDS-";

  for(size_t i = 0; i < WL_MAC_ADDR_LENGTH; i++){    
    apName += String(mac[i], 16);
  }

  return apName;
}

///////////////////////
// WiFiConnection

WiFiConnection::WiFiConnection(){    
  //Configure AP ip
  WiFi.softAPConfig( IPAddress(172, 16, 25, 25),
                     IPAddress(172, 16, 25, 25), 
                     IPAddress(255, 255, 255, 0) 
                    );   

  WiFi.softAP(APName());
  WiFi.softAPdisconnect(true);

  //Configure wifi
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  //See if wifi should be on
  WIFI_CONNECT wcn;
  if(readConfig(wcn)){
    WiFi.begin(wcn.ssid, wcn.pwd);
  }
}

bool WiFiConnection::onCmd(struct CtrlQueueItemEx &itm){
  
  switch(itm.cmd){
    case EEMC_WIFI_STATUS:{
      itm.ntf.put(EECmdResponse<WIFI_STATUS>{itm.cmd});

      if(WiFi.status() == WL_CONNECTED){
        //Handle connect, save wifi params etc
        WIFI_CONNECT wcn;

        strncpy(wcn.ssid, WiFi.SSID().c_str(), sizeof(wcn.ssid) - 1);
        strncpy(wcn.pwd, WiFi.psk().c_str(), sizeof(wcn.pwd) - 1);

        writeConfig(wcn);
      }
    }
    break;

    case EEMC_WIFI_CONNECT:{ //Connect to WiFi network                
      WIFI_CONNECT *c = (WIFI_CONNECT *)itm.data.str;
      WiFi.begin(c->ssid, c->pwd);      
      itm.ntf.put(EECmdResponse<>{itm.cmd, EEER_DELAYED});        
    }    
    break;

    case EEMC_WIFI_DISCONNECT: //Disconnect 
      WiFi.disconnect(true);
      itm.ntf.put(EECmdResponse<>{itm.cmd, EEER_DELAYED});        
    break;
    
    case EEMC_WIFI_AP_ON: //Connect AP
      WiFi.softAP(APName());      
      itm.ntf.put(EECmdResponse<WIFI_STATUS>{itm.cmd}); 
    break;

    case EEMC_WIFI_AP_OFF:
      WiFi.softAPdisconnect(true);
      itm.ntf.put(EECmdResponse<WIFI_STATUS>{itm.cmd});       
    break;
    
    case EEMC_WIFI_SCAN:{
      switch(WiFi.scanComplete()){
        case WIFI_SCAN_FAILED:
          WiFi.scanNetworks(true, true);
          itm.ntf.put(EECmdResponse<>{itm.cmd, EEER_DELAYED});    
        break;
        case WIFI_SCAN_RUNNING:
          itm.ntf.put(EECmdResponse<>{itm.cmd, EEER_DELAYED});    
        break;
        default:
          itm.ntf.put(EECmdResponse<WIFI_SCAN>{itm.cmd});        
          WiFi.scanDelete();
        break;
      }
    }
    break;

    case EEMC_WIFI_CFG_SET:{
      EECmdResponse<WIFI_CONFIG> resp;
      resp.cmd = itm.cmd;
      resp.data = *(WIFI_CONFIG *)itm.data.str;
      itm.ntf.put(resp);        
    }
    break;            

    default: 
    return false;
  }

  return true;
}


bool WiFiConnection::writeConfig(const WIFI_CONNECT &wcn){
  EEPROMCfg ee(EE_WIFI_IDX);  
  ee << (uint8_t)EE_VERSION <<  wcn;

  return true;
}

bool WiFiConnection::readConfig(WIFI_CONNECT &wcn){
  EEPROMCfg ee(EE_WIFI_IDX);

  memset(&wcn, 0, sizeof(wcn));

  //Check version
  uint8_t version;
  ee >> version;

  if(version != EE_VERSION){
    return false;
  }

  //Read SSID and password
  ee >> wcn;

  return true;
}