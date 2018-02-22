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
};

inline EffectRainbow::EffectRainbow(){
  setSpeedDelay(75);
}

inline EffectRainbow::~EffectRainbow(){
}

inline void EffectRainbow::reset(){
  setHSV(CHSV(0, 255, 255)); 
}


inline void EffectRainbow::proceed(CRGB *leds, int numLeds){

  //Get current colors
  CHSV hsv      = getHSV();
  uint8_t delta = numLeds >= 255 ? 1 : 255 / numLeds;

  //Move forward and save
  hsv.hue = (hsv.hue + delta) & 0xFF;
  setHSV(hsv);

  //Set colors - same as fill_rainbow
  for(int i = 0; i < numLeds; i++){ 
    leds[i] = hsv;
    hsv.hue = (hsv.hue + delta) & 0xFF;
  }
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

};

inline EffectTheaterChaseRainbow::EffectTheaterChaseRainbow(){
  setSpeedDelay(100);  
}

inline void EffectTheaterChaseRainbow::reset(){

  _step = 0;
  
  setHSV(CHSV(0, 255, 255)); setHSV(CHSV(0, 255, 255)); 
}


inline void EffectTheaterChaseRainbow::proceed(CRGB *leds, int numLeds){  
  
  //Turn every third pixell off
  for(int i = 0; i < numLeds; i = i + 3){
      setPixel(leds[(i + _step) % numLeds], 0, 0, 0);          
  }

  CHSV hsv      = getHSV();
  uint8_t delta = numLeds >= 255 ? 1 : 255 / numLeds;
  
  //Move forward
  _step  = (_step + 1) % 3;
  if (_step  == 0){ // Next cycle    
    
    //Move to the next color
    hsv.hue = (hsv.hue + delta) & 0xFF;

    //Save it
    setHSV(hsv);
  }

  //Turn every third pixell on
  for(int i = 0; i < numLeds; i = i + 3){ 
    leds[(i + _step ) % numLeds] = hsv;
    hsv.hue = (hsv.hue + delta) & 0xFF;
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


inline void EffectRainbowMove::reset(){
}

void EffectRainbowMove::proceed(CRGB *leds, int numLeds){
  uint8_t beatA = beatsin8(17, 0, 255);                        
  uint8_t beatB = beatsin8(13, 0, 255);
  
  fill_rainbow(leds, numLeds, (beatA + beatB) / 2, 8);
}



#endif //__RAINBOW_H
