#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public EffectColor{
  public:
    EffectStatic(const CHSV &hsv);
    ~EffectStatic();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();
};

inline EffectStatic::EffectStatic(const CHSV &hsv):
  EffectColor(hsv){

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
