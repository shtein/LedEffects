#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public EffectColor{
  public:
    EffectStatic(const CHSV &hsv);

  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};

inline EffectStatic::EffectStatic(const CHSV &hsv):
  EffectColor(hsv){

  setSpeedDelay(100);
}

inline void EffectStatic::reset(){
}

inline void EffectStatic::proceed(CRGB *leds, uint16_t numLeds){
  setAll(leds, numLeds, getColor());
}

#endif //__STATIC_H
