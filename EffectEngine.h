#ifndef __EFFECTENGINE_H
#define __EFFECTENGINE_H

#include "EffectEngineCtx.h"
#include "Static.h"

class Effect;

#define MAX_EFFECTS 15

class EffectEngine{
  public:
    EffectEngine();
    ~EffectEngine();

    void init(CRGB *leds, int maxLeds, uint8_t mode = EEM_OFF);
    void addEffect(Effect *effect);
    
    void loop(const struct CtrlQueueItem &itm);    
    
  protected:
    void showStrip();

    //Command handling
    void onModeChange(const struct CtrlQueueData &data);
    void onNumLedsChange(const struct CtrlQueueData &data);
    void onEffectChange(const struct CtrlQueueData &data); 
    void onColorChange(int index, const struct CtrlQueueData &data);
    void onSpeedChange(const struct CtrlQueueData &data); 

    //Internal routines
    void setMode(uint8_t mode);
    void setEffect(Effect *effect);

    //Reading/writing config from/to EEPROM
    void readConfig();
    void writeConfig(); 
    void preSafeConfig();
    
  protected:
    Effect*       _effects[MAX_EFFECTS];  //Effects
    uint8_t       _numEffects;            //Total number of effects
    Effect*       _curEffect;             //Pointer to current effect including special like static
    uint8_t       _effectNum;             //Index of the current or last effect from _effects

    EffectStatic _eStatic;

    CRGB       *_leds;              //Leds
    uint16_t   _maxLeds;            //Max number of leds    
    uint16_t   _numLeds;            //Actual number of leds    
    uint8_t    _mode;               //Current mode

    unsigned long _millis;              //Processing
    unsigned long _millisToSaveCfg;     //When to safe config
};

#endif //__EFFECTENGINE_H
