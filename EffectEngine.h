#ifndef __EFFECTENGINE_H
#define __EFFECTENGINE_H

class Effect;

#define MAX_EFFECTS 20

#define MAX_LEDS    300
#define LED_PIN     11
#define LED_CLOCK   13


///////////////////////////////////////ß
// mode - off, maintanance or run
// delaySpeed - to be passed to the effect
// curEffect  - effect to run
// numLeds    - number of of leds to use

struct EffectEngineCtx {
  int mode;
  int speedDelay;
  int effectNum;
  int numLeds;
};

class EffectEngine{
  public:
    EffectEngine();
    ~EffectEngine();

    void init();
    void addEffect(Effect *);

    void loop(const struct EffectEngineCtx &ctx);
    
protected:
    void showStrip();
    void checkSetNumLeds(int numLeds);
    void checkSetEffect(Effect *effect, bool force = false);
    
  protected:
    Effect* _effects[MAX_EFFECTS];
    int     _numEffects;
    Effect* _curEffect;

    CRGB    _leds[MAX_LEDS];
    int     _numLeds;
};

#endif //__EFFECTENGINE_H
