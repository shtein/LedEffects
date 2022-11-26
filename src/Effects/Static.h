#ifndef __STATIC_H
#define __STATIC_H

template<const uint8_t hue, const uint8_t sat = 0xFF, const uint8_t val = 0xFF>
class EffectStatic: public EffectColor{
  protected:
  
    void proceed(CRGB *leds, uint16_t numLeds){
      setAll(leds, numLeds, getColor());
    }

    void reset(){
      setSpeedDelay(100);
      setHSV(CHSV(hue, sat, val));
    }
};


#endif //__STATIC_H
