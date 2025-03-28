#ifndef __STATIC_H
#define __STATIC_H

class EffectStatic: public EffectColor{
  protected:
  
    void proceed(CRGB *leds, uint16_t numLeds){
      CRGB rgb;
      rgb.setHSV(_cfg.bytes[0], _cfg.bytes[1], _cfg.bytes[2]);

      fill_solid(leds, numLeds, rgb);
    }

    void reset(){
      setSpeedDelay(100);
    }
};


#endif //__STATIC_H
