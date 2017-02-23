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
  setColor(0xFF0000);
}

inline EffectStatic::~EffectStatic(){
}

inline void EffectStatic::reset(){
}

inline void EffectStatic::proceed(int speedDelay){
  setAll(_color);
}

#endif //__STATIC_H
