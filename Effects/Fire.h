#ifndef __FIRE_H
#define __FIRE_H

/////////////////////////////////////////
// Effect Fire
class EffectFire: public Effect{
  public:
    EffectFire();
    ~EffectFire();
  
    void proceed(CRGB *leds, int numLeds); 
    void reset();
};


inline EffectFire::EffectFire(){ 

  setSpeedDelay(20);
}

inline EffectFire::~EffectFire(){
}

inline void EffectFire::reset(){  
}

#define FIRE_X_SCALE 20 
#define FIRE_Y_SCALE 3

inline void EffectFire::proceed(CRGB *leds, int numLeds){  
  CRGBPalette16 pal = LavaColors_p;
  
  for(int i = 0; i < numLeds ; i++) {        
    leds[i] = ColorFromPalette(pal, 
                              min( (i % 50) * (inoise8((i % 50) * FIRE_X_SCALE, millis() * FIRE_Y_SCALE * numLeds / 255))  >> 6, 255),
                              255,
                              LINEARBLEND ); 
  }
}


#endif //__FIRE_H
