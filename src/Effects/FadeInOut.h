#ifndef __FADEINOUT_H
#define __FADEINOUT_H

//////////////////////////////////////////
// Fade in and out effect


class EffectFadeInOut: public Effect{
  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};


inline void EffectFadeInOut::reset(){   
  setRandomColor(_ctx.hsv);
  setSpeedDelay(50); 
}

#define FIO_STEP 5

inline void EffectFadeInOut::proceed(CRGB *leds, uint16_t numLeds){
  //See if we need to change color
  if(_ctx.hsv.value == 0 ){
     setRandomColor(_ctx.hsv);    
  }
  else{
    //Change value
    _ctx.hsv.value -= FIO_STEP;
  }

  //Transalate value 
  CHSV hsv = _ctx.hsv;
  hsv.value = triwave8(hsv.value);
  
  //Change color
  fill_solid(leds, numLeds, hsv);  
}


#endif //__FADEINOUT_H
