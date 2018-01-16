#ifndef __RAINBOW_H
#define __RAINBOW_H

//////////////////////////////
// Rainbow effect 

class EffectRainbow: public Effect{
  public:
    EffectRainbow();
    ~EffectRainbow();

  protected:
    //CRGB wheel(byte wheelPos) const;
  
    void reset();
    void proceed(CRGB *leds, int numLeds); 
    
  protected:
    uint8_t _step; //current step
};

inline EffectRainbow::EffectRainbow(){
  _step = 0;
  setSpeedDelay(75);
}

inline EffectRainbow::~EffectRainbow(){
}

inline void EffectRainbow::reset(){
  _step = 0;
}


inline void EffectRainbow::proceed(CRGB *leds, int numLeds){
  
  for(int i = 0; i< numLeds; i++) {    
    leds[i] = CHSV(((i * 255 / numLeds) + _step), 255, 255);
  }

  _step ++;
}


/////////////////////////////////////
// Theater Chase Rainbow Effect
class EffectTheaterChaseRainbow: public Effect{
  public:
    EffectTheaterChaseRainbow();
    
  protected:
    void reset();
    void proceed(CRGB *leds, int numLeds);  

  protected:
    uint8_t _step;
    uint8_t _pos;
};

inline EffectTheaterChaseRainbow::EffectTheaterChaseRainbow(){
  setSpeedDelay(100);
  _pos  = 0;
  _step = 0;
}

inline void EffectTheaterChaseRainbow::reset(){
  _step = 0;
  _pos  = 0;
}


inline void EffectTheaterChaseRainbow::proceed(CRGB *leds, int numLeds){  
  //Turn every third pixell off
  for(int i = 0; i < numLeds; i = i + 3){
      setPixel(leds[(i + _pos) % numLeds], 0, 0, 0);          
  }

  //Move forward
  _pos  = (_pos + 1) % 3;
  if (_pos  == 0){ // Next cycle    
    _step ++;
  }

  //Turn every third pixell on
  for(int i = 0; i < numLeds; i = i + 3){ 
    leds[(i + _pos ) % numLeds] = CHSV(((i * 255 / numLeds) + _step), 255 ,255);
  }
}


/////////////////////////
// EffectRainbowMove

class EffectRainbowMove: public Effect{
  public:
    EffectRainbowMove();
    ~EffectRainbowMove();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();
};


inline EffectRainbowMove::EffectRainbowMove(){
  setSpeedDelay(50);
}

inline EffectRainbowMove::~EffectRainbowMove(){
}

void EffectRainbowMove::proceed(CRGB *leds, int numLeds){
  uint8_t beatA = beatsin8(17, 0, 255);                        
  uint8_t beatB = beatsin8(13, 0, 255);
  
  fill_rainbow(leds, numLeds, (beatA + beatB) / 2, 16);
}

inline void EffectRainbowMove::reset(){
}


#endif //__RAINBOW_H
