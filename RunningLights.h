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
    int  _step;
};


inline EffectRunningLights::EffectRunningLights(){
  _step = 0;
  setHSV(CHSV(HUE_RED, 0xFF, 0xFF));
  setSpeedDelay(20);
}

inline EffectRunningLights::~EffectRunningLights(){
}

inline void EffectRunningLights::reset(){
  _step = 0;
}

inline void EffectRunningLights::proceed(CRGB *leds, int numLeds){
        
  for(int i = 0; i< numLeds; i++) {
    float f = (sin((i + _step) ) * 127 + 128) / 255; 

    CRGB color = getColor();
    setPixel(leds[i], f * color.r, f * color.g, f * color.b);
  }  
  
  _step = (_step + 1) % (numLeds * 2);
}

#endif //_RUNNING_LIGHS
