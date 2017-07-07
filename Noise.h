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

#endif //__NOISE_H
