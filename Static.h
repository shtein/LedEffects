#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public Effect{
  public:
    EffectStatic(const CHSV &hsv);
    ~EffectStatic();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();
};

inline EffectStatic::EffectStatic(const CHSV &hsv = CHSV(HUE_RED, 0xFF, 0xFF)){
  setHSV(hsv);
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
