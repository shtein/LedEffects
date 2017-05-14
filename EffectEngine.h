#ifndef __EFFECTENGINE_H
#define __EFFECTENGINE_H

#include "EffectEngineCtx.h"
#include "Static.h"

class Effect;

#define MAX_EFFECTS 20

class EffectEngine{
  public:
    EffectEngine();
    ~EffectEngine();

    void init(int numLeds = MAX_LEDS, uint8_t mode = EEM_OFF);
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

    void setMode(uint8_t mode);
    void setEffect(Effect *effect);
    
  protected:
    Effect*       _effects[MAX_EFFECTS];  //Effects
    uint8_t       _numEffects;            //Total number of effects
    Effect*       _curEffect;             //Pointer to current effect including special like static
    uint8_t       _effectNum;             //Index of the current or last effect from _effects

    EffectStatic _eStatic;

    CRGB         _leds[MAX_LEDS];       //LEDs
    int          _numLeds;              //Current number of leds
    
    uint8_t      _mode;                 //Current mode

    unsigned long _millis;              //Processing
};

#endif //__EFFECTENGINE_H
