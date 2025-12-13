#ifndef __FIRE_H
#define __FIRE_H

/////////////////////////////////////////
// Effect Fire
class EffectFire: public Effect{
protected:
  void proceed(CRGB *leds, uint16_t numLeds); 
  void reset();
};

inline void EffectFire::reset(){  
  setSpeedDelay(20);
}

#define FIRE_X_SCALE 20 
#define FIRE_Y_SCALE 3

inline void EffectFire::proceed(CRGB *leds, uint16_t numLeds){  
  CRGBPalette16 pal = LavaColors_p;
  
  for(uint16_t i = 0; i < numLeds ; i++) {        
    leds[i] = ColorFromPalette(pal, 
                              min( (i % 50) * (inoise8((i % 50) * FIRE_X_SCALE, millis() * FIRE_Y_SCALE * numLeds / 255))  >> 6, 255),
                              255,
                              LINEARBLEND ); 
  }
}


#endif //__FIRE_H
