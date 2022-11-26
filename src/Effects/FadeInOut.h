#ifndef __FADEINOUT_H
#define __FADEINOUT_H

//////////////////////////////////////////
// Fade in and out effect


class EffectFadeInOut: public EffectColor{
  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};



inline void EffectFadeInOut::reset(){   
  setRandomColor();
  setSpeedDelay(50); 
}

#define FIO_STEP 5

inline void EffectFadeInOut::proceed(CRGB *leds, uint16_t numLeds){
  CHSV hsv = getHSV();

  //See if we need to change color
  if(hsv.value == 0 ){
     setRandomColor();
     hsv = getHSV();
  }

  //Change value
  hsv.value -= FIO_STEP;
  setHSV(hsv);

  //Transalate value 
  hsv.value = triwave8(hsv.value);
  
  //Change color
  setAll(leds, numLeds, hsv );
}


#endif //__FADEINOUT_H
