#ifndef __RAINBOW_H
#define __RAINBOW_H

//////////////////////////////
// Rainbow effect 

class EffectRainbow: public Effect{
  public:
    EffectRainbow();
    ~EffectRainbow();

  protected:
    byte *wheel(byte wheelPos) const;
  
    void reset();
    void proceed(CRGB *leds, int numLeds); 
    
  protected:
    int _step; //current step
};

inline EffectRainbow::EffectRainbow(){
  _step = 0;
  setSpeedDelay(100);
}

inline EffectRainbow::~EffectRainbow(){
}

inline void EffectRainbow::reset(){
  _step = 0;
}


inline void EffectRainbow::proceed(CRGB *leds, int numLeds){
  byte *c;

  for(int i = 0; i< numLeds; i++) {
    c = wheel(((i * 256 / numLeds) + _step) & 255);
    setPixel(leds[i], *c, *(c + 1), *(c + 2));
  }
 
  _step ++;
  if(_step > 256 * 5)
    _step = 0;
}

inline byte * EffectRainbow::wheel(byte wheelPos) const{
  static byte c[3];
  
  if(wheelPos < 85) {
   c[0] = wheelPos * 3;
   c[1] = 255 - wheelPos * 3;
   c[2] = 0;
  } else if(wheelPos < 170) {
   wheelPos -= 85;
   c[0] = 255 - wheelPos * 3;
   c[1] = 0;
   c[2] = wheelPos * 3;
  } else {
   wheelPos -= 170;
   c[0]=0;
   c[1] = wheelPos * 3;
   c[2] = 255 - wheelPos * 3;
  }

  return c;
}


/////////////////////////////////////
// Theater Chase Rainbow Effect
class EffectTheaterChaseRainbow: public EffectRainbow{
  public:
    EffectTheaterChaseRainbow();
    
  protected:
    void proceed(CRGB *leds, int numLeds);  

};

inline EffectTheaterChaseRainbow::EffectTheaterChaseRainbow(){
  setSpeedDelay(130);
}

inline void EffectTheaterChaseRainbow::proceed(CRGB *leds, int numLeds){
  byte *counters = (byte *)&_step; //counter[0] - color for colorwheel, counter[1] - pixel (from 0 to 2)
  
  
  //Turn every third pixell off
  for(int i = 0; i < numLeds; i = i + 3){
      setPixel(leds[(i + counters[1]) % numLeds], 0, 0, 0);          
  }

  //Move forward
  counters[1]  = (counters[1] + 1) % 3;
  if (counters[1]  == 0){ // Next cycle    
    counters[0]  = counters[0] == 255 ? 0: counters[0] + 1;  
  }

  //Turn every third pixell on
  for(int i = 0; i < numLeds; i = i + 3){
    byte *c = wheel((counters[0] + i) % 255);
    setPixel(leds[(i + counters[1] ) % numLeds], *c, *(c+1), *(c+2));    
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
  
  fill_rainbow(leds, numLeds, (beatA + beatB) / 2, 8);
}

inline void EffectRainbowMove::reset(){
}


#endif //__RAINBOW_H
