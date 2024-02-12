#ifndef __EFFECTENGINE_H
#define __EFFECTENGINE_H

#include "EffectEngineCtx.h"
#include "EffectEngineCfg.h"
#include "WiFiConnect.h"


class CtrlQueueItemEx;
class Effect;
class EEPROMCfg;

//Engine flags
#define EFF_RANDOM_START_MODE     0x01
#define EFF_RANDOM_START_EFFECT   0x02

typedef CRGB EELEDS[MAX_LEDS];

/////////////////////////////////
// EffectEngine
class EffectEngine{
  public:
    EffectEngine();
    ~EffectEngine();

    void init();    
    void loop(const struct CtrlQueueItem &itm, NtfSet &ntf);   

    CRGB *getLeds() const;
    
  protected:
    
    //Command handling
    bool onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf);
    bool onCmdEE(const struct CtrlQueueItem &itm, NtfSet &ntf);

    void onModeChange(const struct CtrlQueueData &data);
    void onNumLedsChange(const struct CtrlQueueData &data);
    void onEffectChange(const struct CtrlQueueData &data); 

    //Notifications    
#ifdef NTF_ENABLED    
    void ntf(uint8_t cmd, uint8_t error, NtfSet &ntf);
#endif //NTF_ENABLED 

    //Internal routines
    void setMode(uint8_t mode);
    void setEffect(uint8_t effectNum);

    //Reading/writing config from/to EEPROM
    void saveConfig(); 
    void preSaveConfig();
  
  protected:    
    EFFECT_ENGINE_CONFIG _cfgEngine;      //Engine config
    EFFECT_MODE_CONFIG   _cfgMode;        //Current mode config
    Effect              *_curEffect;      //Current Effect

    EELEDS     _leds;                     //Leds  
   
    unsigned long _millis;                //Processing
    unsigned long _millisToSaveCfg;       //When to safe config

#if defined(ESP8266) || defined(ESP32)
    WiFiConnection   _wifi;               //WiFi connectivity
#endif
};


//////////////////////////////////
// Serial command line
uint8_t parseCommandInput(const char *tokens[], CtrlQueueData &data);

#endif //__EFFECTENGINE_H
