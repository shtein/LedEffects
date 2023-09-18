#ifndef __EFFECTENGINE_H
#define __EFFECTENGINE_H

#include "EffectEngineCtx.h"
#include "WiFiConnect.h"

class CtrlQueueItemEx;
class Effect;
class EEPROMCfg;

//Engine flags
#define EFF_RANDOM_START_MODE     0x01
#define EFF_RANDOM_START_EFFECT   0x02
#define EEF_DEFINED               0x08 //Marker that flag is defined

typedef CRGB EELEDS[MAX_LEDS];

struct EFFECT_MODE{
  uint8_t  *effects;                    //Effects
  uint8_t  numEffects;                  //Total number of effects
  uint8_t  effectNum;                   //Index of the current or last effect from _effects
#ifdef NTF_ENABLED      
  const __FlashStringHelper *modeName;  //Name of the mode
#endif      
}; 

/////////////////////////////////
// EffectEngine
class EffectEngine{
  public:
    EffectEngine(uint8_t flags = 0);
    ~EffectEngine();

    void init();
    void addMode(const __FlashStringHelper *modeName, uint8_t *effects); //add mode
    void addEffect(uint8_t effect); //add effect to current mode
    
    void loop(struct CtrlQueueItemEx &itm);   

    CRGB *getLeds() const;
    
  protected:
    
    //Command handling
    bool onCmd(struct CtrlQueueItemEx &itm);
    bool onCmdEE(struct CtrlQueueItemEx &itm);

    void onModeChange(struct CtrlQueueData &data);
    void onNumLedsChange(struct CtrlQueueData &data);
    void onEffectChange(struct CtrlQueueData &data); 

    //Notifications    
#ifdef NTF_ENABLED    
    void ntf(uint8_t cmd, uint8_t error, NtfSet &ntf);
#endif //NTF_ENABLED 

    //Internal routines
    void setMode(uint8_t mode);
    void setEffect(uint8_t effectNum);

    //Reading/writing config from/to EEPROM
    void readConfig();
    void writeConfig(); 
    void configCurEffect(bool read);    
    void preSaveConfig();
  
  protected:
    //Effects for modes
    EFFECT_MODE _modes[MAX_MODES]; 

    uint8_t    _numModes:4;               //Total number of modes
    uint8_t    _modeNum:4;                //Current mode
    Effect    *_curEffect;                //Current Effect

    EELEDS     _leds;                     //Leds
    uint16_t   _numLeds:12;               //Max number of leds    
    uint16_t   _flags:4;                  //Flags 
   
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
