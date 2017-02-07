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
    CRGB _color;
};


inline EffectRunningLights::EffectRunningLights(){
  _step = 0;
  _color = 0xFFFF00;
}

inline EffectRunningLights::~EffectRunningLights(){
}

inline void EffectRunningLights::reset(){
  _step = 0;
  _color = 0xC71585;
}

inline void EffectRunningLights::proceed(int speedDelay){
        
  for(int i = 0; i< _numLeds; i++) {
    float f = (sin((i + _step) ) * 127 + 128) / 255; 
    
    setPixel(i,f * _color.r, f * _color.g, f * _color.b);
  }
    
  
  _step = (_step + 1) % (_numLeds * 2);
}

  
