#ifndef __NOISE_H
#define __NOISE_H

class EffectNoise: public Effect{
  public:
     EffectNoise(); 
    ~EffectNoise();

    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    CRGBPalette16 _palCurrent;
    CRGBPalette16 _palTarget;
    int           _step;
    int           _dist;
};

inline EffectNoise::EffectNoise(){
  setSpeedDelay(25);
}

inline EffectNoise::~EffectNoise(){ 
}

inline void EffectNoise::reset(){
  _palCurrent = CRGBPalette16(CRGB::Black);
  _palTarget  = CRGBPalette16( CHSV(random8(), 255, random8(128,255)), 
                               CHSV(random8(), 255, random8(128,255)), 
                               CHSV(random8(), 192, random8(128,255)), 
                               CHSV(random8(), 255, random8(128,255))
                             );
  
  _dist       = random16(millis());
  _step       = 0;
}

#define MAX_PAL_CHANGES 24

#define XSCALE          30
#define YSCALE          30

#define CHANGE_PAL_STEP 500


inline void EffectNoise::proceed(CRGB *leds, int numLeds){
  //Check if it is to update target palette
  if(_step == CHANGE_PAL_STEP){

    //Reset step
    _step = 0;
    
    //Change target palette
    _palTarget = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), 
                               CHSV(random8(), 255, random8(128,255)), 
                               CHSV(random8(), 192, random8(128,255)), 
                               CHSV(random8(), 255, random8(128,255))
                              );


  }
  
  

  //Proceed with palette transtion
  nblendPaletteTowardPalette(_palCurrent, _palTarget, MAX_PAL_CHANGES); 


  //Set colors
  for(int i = 0; i < numLeds; i++) {                                    
    uint8_t index = inoise8(i  * XSCALE, _dist + i * YSCALE) % 255;             
    leds[i] = ColorFromPalette(_palCurrent, index, 255, LINEARBLEND);
  }

  //Prepare for the next move
  _dist += beatsin8(10, 1, 4);                                               
  _step++;
}


/////////////////////////////////////
// class EffectBeatWave
class EffectBeatWave: public Effect{
  public:
     EffectBeatWave(); 
    ~EffectBeatWave();

    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    CRGBPalette16 _palCurrent;
    CRGBPalette16 _palTarget;
    int           _step;
    
};


EffectBeatWave::EffectBeatWave(){
  setSpeedDelay(100);
}

EffectBeatWave::~EffectBeatWave(){
}

void EffectBeatWave::reset(){
  _step = 0;  
}

inline void EffectBeatWave::proceed(CRGB *leds, int numLeds){

    //Proceed with palette transtion
  nblendPaletteTowardPalette(_palCurrent, _palTarget, MAX_PAL_CHANGES); 

  //Set colors
  for(int i = 0; i < numLeds; i++) {                                   
    leds[i] = ColorFromPalette( _palCurrent, i+ beatsin8(9, 0, 255) + beatsin8(8, 0, 255) + beatsin8(7, 0, 255) + beatsin8(6, 0, 255), 255, LINEARBLEND); 
  }

  _step++;

  
  //Check if it is to update target palette
  if(_step == CHANGE_PAL_STEP){

    //Reset step
    _step = 0;
    
    
    _palTarget = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), 
                               CHSV(random8(), 255, random8(128,255)), 
                               CHSV(random8(), 192, random8(128,255)), 
                               CHSV(random8(), 255, random8(128,255))
                              );    

  }
}


/////////////////////////////////////
// class EffectBlur
class EffectBlur: public Effect{
  public:
     EffectBlur(); 
    ~EffectBlur();

    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    int           _step;
    
};


EffectBlur::EffectBlur(){
  setSpeedDelay(25);
}

EffectBlur::~EffectBlur(){
}

void EffectBlur::reset(){
}

inline void EffectBlur::proceed(CRGB *leds, int numLeds){
  
  uint8_t blurAmount = dim8_raw( beatsin8(3, 64, 192) );       
  blur1d( leds, numLeds, blurAmount);                        
  
  uint8_t  i = beatsin8(9, 0, numLeds);
  uint8_t  j = beatsin8(7, 0, numLeds);
  uint8_t  k = beatsin8(5, 0, numLeds);
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  leds[(i + j) / 2]    = CHSV(ms / 29, 200, 255);
  leds[(j + k) / 2]    = CHSV(ms / 41, 200, 255);
  leds[(k + i) / 2]    = CHSV(ms / 73, 200, 255);
  leds[(k + i+ j) / 3] = CHSV(ms / 53, 200, 255);
}




#endif //__NOISE_H
