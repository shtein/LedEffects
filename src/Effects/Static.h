#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public EffectColor{
  protected:
  
    void proceed(CRGB *leds, uint16_t numLeds){
      fill_solid(leds, numLeds, getHSV());
    }

    void reset(){
      setSpeedDelay(100);
    }
};


#endif //__STATIC_H
