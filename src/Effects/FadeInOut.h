#ifndef __FADEINOUT_H
#define __FADEINOUT_H

//////////////////////////////////////////
// Fade in and out effect


class EffectFadeInOut: public EffectColor{
  public:
    EffectFadeInOut();
    ~EffectFadeInOut();

  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};


inline EffectFadeInOut::EffectFadeInOut(){
  setSpeedDelay(50); 
}

inline EffectFadeInOut::~EffectFadeInOut(){
}

inline void EffectFadeInOut::reset(){   
  setRandomColor();
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
