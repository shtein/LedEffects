#ifndef __FADEINOUT_H
#define __FADEINOUT_H

//////////////////////////////////////////
// Fade in and out effect


class EffectFadeInOut: public Effect{
  public:
    EffectFadeInOut();
    ~EffectFadeInOut();

  protected:
    void proceed(int speedDelay); 
    void reset();

  protected:
    int  _fade; //negative - fade in, positive fade out
    int  _step; //cycle
};


#define FADE_SPEED

inline EffectFadeInOut::EffectFadeInOut(){
  _fade       = -1;
  _step       = 0xFF;
  _color      = 0x00000;
  _speedDelay = 50; 
}

inline EffectFadeInOut::~EffectFadeInOut(){
}

inline void EffectFadeInOut::reset(){ 
  _fade  = -5;
  _step  = 0xFF;
  setRandomColor();
}

inline void EffectFadeInOut::proceed(int speedDelay){

  _step = _step + _fade;
  if(_step == 0 || _step == 0xFF)
    _fade = -_fade; 

  if(_step == 0xFF )
    setRandomColor();

  //get current color
  CRGB color = _color;
  color.fadeLightBy(_step);
  
  //change color
  setAll( color );
}


#endif //__FADEINOUT_H
