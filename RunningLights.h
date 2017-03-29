#ifndef __RUNNING_LIGHS
#define __RUNNING_LIGHS

////////////////////
// Running Lights Effect


class EffectRunningLights: public Effect{
  public:
    EffectRunningLights();
    ~EffectRunningLights();

  protected:
    void proceed(int speedDelay); 
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

inline void EffectRunningLights::proceed(int speedDelay){
        
  for(int i = 0; i< getNumLeds(); i++) {
    float f = (sin((i + _step) ) * 127 + 128) / 255; 

    CRGB color = getColor();
    setPixel(i, f * color.r, f * color.g, f * color.b);
  }  
  
  _step = (_step + 1) % (getNumLeds() * 2);
}

#endif //_RUNNING_LIGHS
