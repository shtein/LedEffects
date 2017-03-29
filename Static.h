#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public Effect{
  public:
    EffectStatic();
    ~EffectStatic();

  protected:
    void proceed(int speedDelay); 
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

inline void EffectStatic::proceed(int speedDelay){
  setAll(getColor());
}

#endif //__STATIC_H
