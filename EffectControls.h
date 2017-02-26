#ifndef __EFFECTCONTROLS_H
#define __EFFECTCONTROLS_H

#include "EffectEngineCtx.h"
#include "AnalogInput.h"

class EffectControls{
  public:
    EffectControls();
    ~EffectControls();

   void init(struct EffectEngineCtx &ctx);

    //To be called before or after EffectEngine::loop
    void loop(struct EffectEngineCtx &ctx);

  protected:
    int getCurrentEffect() const;
    void setCurrentEffect(int curEffect);
  
    int getSpeedDelay() const;
    void setSpeedDelay(int speedDelay);

    CRGB getColor() const;
    void setColor(const CRGB &color);

    int getNumLeds() const;
    void setNumLeds(int numLeds);

    int getMode() const;

    int getRemotePushedDec(int value, int limit, unsigned long key, int repeatLimit) const;
    int getRemotePushedInc(int value, int limit, unsigned long key, int repeatLimit) const;
    int getRemotePushedValue(int value, int limitDec, int limitInc, unsigned long keyDec, unsigned long keyInc, int repeatLimit) const;
    
    void readControls();

  protected:
    //Mode
    int _mode;
  
    //Effects
    int _maxEffects;
    int _effectNum;
  
    //Speed delay
    int _speedDelay;

    //Color
    CRGB _color;

    //Num leds
    int _numLeds;
    
    //Test
    IRRemoteRecv  _remote;
};

#endif //__EFFECTCONTROLS_H
