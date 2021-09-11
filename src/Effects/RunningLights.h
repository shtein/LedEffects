#ifndef __RUNNING_LIGHS
#define __RUNNING_LIGHS

////////////////////
// Running Lights Effect


class EffectRunningLights: public EffectColor{
  public:
    EffectRunningLights();

  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};


inline EffectRunningLights::EffectRunningLights(){
  setHSV(CHSV(HUE_RED, 0xFF, 0xFF));
  setSpeedDelay(50);
}

inline void EffectRunningLights::reset(){
  _ctx.step = 0;
}


#define RL_SIZE 6
inline void EffectRunningLights::proceed(CRGB *leds, uint16_t numLeds){
  
  for(uint16_t i = 0; i < numLeds; i++) {   
    
    CHSV hsv = getHSV();
    hsv.v  = sin8( ((i + _ctx.step) % RL_SIZE) * 255 / (RL_SIZE - 1) );
    setPixel(leds[i], hsv);
  }  
  
  _ctx.step = (_ctx.step + 1) % RL_SIZE;
}



#endif //__RUNNING_LIGHS
