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

    void init(struct EffectEngineCtx &ctx);
    void addEffect(Effect *);

    void loop(struct EffectEngineCtx &ctx);
    
protected:
    void showStrip();

    void onEffectChange(struct EffectEngineCtx &ctx); 
    void onNumLedsChange(struct EffectEngineCtx &ctx);
    void onModeChange(struct EffectEngineCtx &ctx);
    
  protected:
    Effect*       _effects[MAX_EFFECTS];
    int           _numEffects;
    Effect*       _curEffect;

    EffectStatic _eStatic;

    CRGB         _leds[MAX_LEDS];
    int          _numLeds;
};

#endif //__EFFECTENGINE_H
