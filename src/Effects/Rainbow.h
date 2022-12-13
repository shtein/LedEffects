#ifndef __RAINBOW_H
#define __RAINBOW_H

//////////////////////////////
// Rainbow effect 

class EffectRainbow: public Effect{
  protected:
    //CRGB wheel(byte wheelPos) const;
  
    void reset();
    void proceed(CRGB *leds, uint16_t numLeds); 
};


inline void EffectRainbow::reset(){  
  _ctx.hsv = CHSV(0, 0xFF, 0xFF);
  setSpeedDelay(75);
}


inline void EffectRainbow::proceed(CRGB *leds, uint16_t numLeds){

  //Get current colors  
  uint8_t delta = numLeds >= 255 ? 1 : 255 / numLeds;

  //Move forward and save
  _ctx.hsv.hue = (_ctx.hsv.hue + delta) & 0xFF;
  
  
  //Set colors - same as fill_rainbow
  CHSV hsv = _ctx.hsv;
  
  for(uint16_t i = 0; i < numLeds; i++){ 
    leds[i] = hsv;
    hsv.hue = (hsv.hue + delta) & 0xFF;
  }
}


/////////////////////////////////////
// Theater Chase Rainbow Effect
class EffectTheaterChaseRainbow: public Effect{  
  protected:
    void reset();
    void proceed(CRGB *leds, uint16_t numLeds);  
};

inline void EffectTheaterChaseRainbow::reset(){
  _ctx.step = 0;  
  _ctx.hsv = CHSV(0, 0xFF, 0xFF);

  setSpeedDelay(100);  
}


inline void EffectTheaterChaseRainbow::proceed(CRGB *leds, uint16_t numLeds){  
  
  //Turn every third pixell off
  for(uint16_t i = 0; i < numLeds; i = i + 3){
      leds[(i + _ctx.step) % numLeds] = CRGB::Black;          
  }

  uint8_t delta = numLeds >= 255 ? 1 : 255 / numLeds;
  
  //Move forward
  _ctx.step  = (_ctx.step + 1) % 3;
  if (_ctx.step  == 0){ // Next cycle    
    
    //Move to the next color
    _ctx.hsv.hue = (_ctx.hsv.hue + delta) & 0xFF;
  }

  CHSV hsv = _ctx.hsv;
  //Turn every third pixell on
  for(uint16_t i = 0; i < numLeds; i = i + 3){ 
    leds[(i + _ctx.step ) % numLeds] = hsv;
    hsv.hue = (hsv.hue + delta) & 0xFF;
  }
}


/////////////////////////
// EffectRainbowMove

class EffectRainbowMove: public Effect{
  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};


inline void EffectRainbowMove::reset(){
  setSpeedDelay(50);
}

void EffectRainbowMove::proceed(CRGB *leds, uint16_t numLeds){
  uint8_t beatA = beatsin8(17, 0, 255);                        
  uint8_t beatB = beatsin8(13, 0, 255);
  
  fill_rainbow(leds, numLeds, (beatA + beatB) / 2, 8);
}



#endif //__RAINBOW_H
