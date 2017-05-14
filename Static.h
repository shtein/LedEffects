#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public Effect{
  public:
    EffectStatic();
    ~EffectStatic();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();
};

inline EffectStatic::EffectStatic(){
  setHSV(CHSV(HUE_RED, 0xFF, 0xFF));
  setSpeedDelay(100);
}

inline EffectStatic::~EffectStatic(){
}

inline void EffectStatic::reset(){
}

inline void EffectStatic::proceed(CRGB *leds, int numLeds){
  setAll(leds, numLeds, getColor());
}

#endif //__STATIC_H
