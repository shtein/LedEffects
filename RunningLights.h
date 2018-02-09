#ifndef __RUNNING_LIGHS
#define __RUNNING_LIGHS

////////////////////
// Running Lights Effect


class EffectRunningLights: public Effect{
  public:
    EffectRunningLights();
    ~EffectRunningLights();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    uint8_t  _step;
};


inline EffectRunningLights::EffectRunningLights(){
  _step = 0;
  setHSV(CHSV(HUE_RED, 0xFF, 0xFF));
  setSpeedDelay(50);
}

inline EffectRunningLights::~EffectRunningLights(){
}

inline void EffectRunningLights::reset(){
  _step = 0;
}


#define RL_SIZE 6
inline void EffectRunningLights::proceed(CRGB *leds, int numLeds){
  
  for(int i = 0; i< numLeds; i++) {   
    
    CHSV hsv = getHSV();
    hsv.v  = sin8( ((i + _step) % RL_SIZE) * 255 / (RL_SIZE - 1) );
    setPixel(leds[i], hsv);
  }  
  
  _step = (_step + 1) % RL_SIZE;
}



#endif //__RUNNING_LIGHS
