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
  setColor( 0xFFFF00 );
  setSpeedDelay(20);
}

inline EffectRunningLights::~EffectRunningLights(){
}

inline void EffectRunningLights::reset(){
  _step = 0;
}

inline void EffectRunningLights::proceed(int speedDelay){
        
  for(int i = 0; i< _numLeds; i++) {
    float f = (sin((i + _step) ) * 127 + 128) / 255; 
    
    setPixel(i,f * _color.r, f * _color.g, f * _color.b);
  }  
  
  _step = (_step + 1) % (_numLeds * 2);
}

#endif //_RUNNING_LIGHS
