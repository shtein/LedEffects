#ifndef __BLUR_H
#define __BLUR_H

/////////////////////////////////////
// class EffectBlur
class EffectBlur: public Effect{
  public:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};



inline void EffectBlur::reset(){
  setSpeedDelay(25);
}

inline void EffectBlur::proceed(CRGB *leds, uint16_t numLeds){
  
  uint8_t blurAmount = dim8_raw( beatsin8(3, 64, 192) );       
  blur1d( leds, numLeds, blurAmount);                        
  
  uint16_t  i = beatsin16(9, 0, numLeds);
  uint16_t  j = beatsin16(7, 0, numLeds);
  uint16_t  k = beatsin16(5, 0, numLeds);
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  
  leds[((i + j) / 2) % numLeds]     = CHSV(ms / 29, 200, 255);
  leds[((j + k) / 2) % numLeds]     = CHSV(ms / 41, 200, 255);
  leds[((k + i) / 2) % numLeds]     = CHSV(ms / 73, 200, 255);
  leds[((k + i + j) / 3) % numLeds] = CHSV(ms / 53, 200, 255);
}


#endif //__BLUR_H
