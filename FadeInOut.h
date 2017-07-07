#ifndef __FADEINOUT_H
#define __FADEINOUT_H

//////////////////////////////////////////
// Fade in and out effect


class EffectFadeInOut: public Effect{
  public:
    EffectFadeInOut();
    ~EffectFadeInOut();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    int  _fade:5;  //negative - fade in, positive fade out
    int  _step:11; //cycle
};


#define FADE_SPEED

inline EffectFadeInOut::EffectFadeInOut(){
  _fade       = -5;
  _step       = 0xFF;
  setSpeedDelay(50); 
}

inline EffectFadeInOut::~EffectFadeInOut(){
}

inline void EffectFadeInOut::reset(){ 
  _fade  = -5;
  _step  = 0xFF;
  setRandomColor();
}

inline void EffectFadeInOut::proceed(CRGB *leds, int numLeds){

  _step = _step + _fade;
  if(_step == 0 || _step == 0xFF)
    _fade = -_fade; 

  if(_step == 0xFF )
    setRandomColor();

  //get current color
  CRGB color = getColor();
  color.fadeLightBy(_step);
  
  //change color
  setAll(leds, numLeds, color );
}


#endif //__FADEINOUT_H
