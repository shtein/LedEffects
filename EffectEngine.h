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

    void setMode(uint8_t mode);
    void setEffect(Effect *effect);
    
  protected:
    Effect*       _effects[MAX_EFFECTS];  //Effects
    uint8_t       _numEffects;            //Total number of effects
    Effect*       _curEffect;             //Pointer to current effect including special like static
    uint8_t       _effectNum;             //Index of the current or last effect from _effects

    EffectStatic _eStatic;

    CRGB        *_leds;                //Leds
    uint32_t    _maxLeds:12;           //Current number of leds    
    uint32_t    _numLeds:12;           //Current number of leds    
    uint32_t    _mode:8;               //Current mode

    unsigned long _millis;              //Processing
};

#define BEGIN_EFFECT_ENGINE() \
  _CM EffectEngine ee;

#define END_EFFECT_ENGINE()  \
  _CM CtrlQueueItem itm; \
  for( ;; ){\
     cp.loop(itm); \
     ee.loop(itm); \
  }

#define BEGIN_EFFECTS()

#define END_EFFECTS()

#define ADD_EFFECT(ClassEffect) \
  _CM ClassEffect e##ClassEffect; \
  ee.addEffect(&e##ClassEffect);


#define BEGIN_LEDS(xmaxleds, xmode) \
   _CM CRGB leds[xmaxleds]; \
    uint8_t mode = xmode;
    
#define ADD_STRIP(Type, ...) \
  FastLED.addLeds<Type, __VA_ARGS__ >(leds, sizeof(leds) / sizeof(leds[0])).setCorrection( TypicalLEDStrip );  

#define END_LEDS() \
    ee.init(leds, sizeof(leds) / sizeof(leds[0]), mode);

#endif //__EFFECTENGINE_H
