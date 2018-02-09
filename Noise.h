#ifndef __NOISE_H
#define __NOISE_H


////////////////////////////
// Effect EffectPaletteTransformFast - simple transition between palettes
class EffectPaletteTransformFast: public EffectPaletteTransform{
  public:
    EffectPaletteTransformFast();
    ~EffectPaletteTransformFast();

  protected:
    int getPalClrIndex(int ledIndex, int numLeds) const;
    int getMaxStep() const;
};

inline EffectPaletteTransformFast::EffectPaletteTransformFast(){
}

inline EffectPaletteTransformFast::~EffectPaletteTransformFast(){
}

inline int EffectPaletteTransformFast::getPalClrIndex(int ledIndex, int numLeds) const{
  //Full range
  return map(ledIndex, 0, numLeds - 1, 0, 65535);
}

inline int EffectPaletteTransformFast::getMaxStep() const{
  return  MAX_PAL_CHANGES * 3;
}


////////////////////////////
//EffectNoise
class EffectNoise: public EffectPaletteTransform{
  public:
     EffectNoise(); 
    ~EffectNoise();

    void proceed(CRGB *leds, int numLeds); 
    void reset();

protected:
    int getPalClrIndex(int ledIndex, int numLeds) const;

  protected:
    int _dist;
};

inline EffectNoise::EffectNoise(){
}

inline EffectNoise::~EffectNoise(){ 
}

inline void EffectNoise::reset(){
  //Do default actions
  EffectPaletteTransform::reset();
  
  //Reset dist
  _dist = random16(millis());
}

inline void EffectNoise::proceed(CRGB *leds, int numLeds){
  //Do default actions
  EffectPaletteTransform::proceed(leds, numLeds);
  
  //Prepare for the next move
  _dist += beatsin8(10, 1, 4);                                               
}

#define XSCALE          30
#define YSCALE          30

inline int EffectNoise::getPalClrIndex(int ledIndex, int /*numLeds*/) const{
  return inoise8(ledIndex  * XSCALE, _dist + ledIndex * YSCALE) % 255;             
}


/////////////////////////////////////
// Effect Beat Wave
class EffectBeatWave: public EffectPaletteTransform{
  public:
     EffectBeatWave(); 
    ~EffectBeatWave();

  protected:
    int getPalClrIndex(int ledIndex, int numLeds) const;
    bool isReadyToBlendPal() const;
};


inline EffectBeatWave::EffectBeatWave(){
}

inline EffectBeatWave::~EffectBeatWave(){
}

inline int EffectBeatWave::getPalClrIndex(int ledIndex, int /*numLeds*/) const{
  return ledIndex + beatsin8(9, 0, 255) + beatsin8(8, 0, 255) + beatsin8(7, 0, 255) + beatsin8(6, 0, 255);
}

inline bool EffectBeatWave::isReadyToBlendPal() const{
  return _step % 50 == 0 ? true : false; 
}


//////////////////////////////
// Effect Confetti
class EffectConfetti: public EffectPaletteTransform{
  public:
    EffectConfetti();
    ~EffectConfetti();

  protected:
    void updateLeds(CRGB *leds, int numLeds);
    void updateColors(); 

    int getMaxStep() const;

  protected:
    uint8_t _hue;
    uint8_t _hueDiff;
    uint8_t _hueInc:2;
    uint8_t _fade:6;
    
};

inline EffectConfetti::EffectConfetti(){  
  _hue     = 50;
  _hueDiff = 255;
  _hueInc  = 1;
  _fade    = 8;

  setSpeedDelay(20);
}

inline EffectConfetti::~EffectConfetti(){  
}


inline int EffectConfetti::getMaxStep() const{
  return 100;
}

inline void EffectConfetti::updateLeds(CRGB *leds, int numLeds){
  
  fadeToBlackBy(leds, numLeds, _fade);                     
  leds[random16(numLeds)] = ColorFromPalette(_palCurrent, _hue + random8(_hueDiff) / 4 , 255, LINEARBLEND);
  
  _hue += _hueInc;
}


inline void EffectConfetti::updateColors(){
  
  struct ConfSettings {
    const TProgmemRGBPalette16 &pal;    
    uint8_t                     hue;
    uint8_t                     hueDiff; 
    uint8_t                     hueInc:2;
    uint8_t                     fade:6;
  } 
  
  stgs[] = { { OceanColors_p,  192, 255, 1, 16 },
             { LavaColors_p,   128, 64, 2, 8 },  
             { ForestColors_p, random8(255), 16, 1, 4 },
             { CloudColors_p, 36, 16, 1, 4 }
           };


   uint8_t idx = random8(sizeof(stgs) / sizeof(stgs[0]) + 1);

  _palTarget = stgs[idx].pal;
  _hueInc    = stgs[idx].hueInc; 
  _hueDiff   = stgs[idx].hueDiff; 
  _hue       = stgs[idx].hue; 
  _fade      = stgs[idx].fade; 
}


///////////////////////////
// Effect Matrix, I have no idea why it is called matrix, it is its name in original code

#define MATRIX_STEP_MIN 200
#define MATRIX_STEP_MAX 100

class EffectMatrix: public EffectPaletteTransform{
  public:
    EffectMatrix();
    ~EffectMatrix();

  protected:
    void reset();    

    void updateColors();
    void updateLeds(CRGB *leds, int numLeds);

    int getMaxStep() const;

  protected:
    uint8_t  _dir:4;         //direction
    uint8_t  _hueInc:4;      //change hue
    uint8_t  _indexPal;      //palette index
};

inline EffectMatrix::EffectMatrix(){
  setSpeedDelay(50);
}

inline EffectMatrix::~EffectMatrix(){
  
}

inline void EffectMatrix::reset(){  
  //Default actions
  EffectPaletteTransform::reset();
  

  //Blue background  
  setHSV(CHSV(80, 255, 16));

    //Init with black
  _palCurrent = CRGBPalette16(CHSV(0, 0, 0));    
  _palTarget  = CRGBPalette16(CHSV(0, 0, 0));  


  //Default update rate
  _dir        = false;
  _hueInc     = true;
  _indexPal   = 0;
}

inline int EffectMatrix::getMaxStep() const{
  return random(MATRIX_STEP_MIN, MATRIX_STEP_MAX);  
}

inline void EffectMatrix::updateColors(){
  
   struct MatSettings {
    CHSV                          bg;
    const TProgmemRGBPalette16   &pal;
    uint8_t                       useBg:4;
    uint8_t                       usePal:4;
   } 
   stgs[] = { { CHSV(160, 255, 32), OceanColors_p, true, true },
              { CHSV(50, 255, 32), LavaColors_p, true, true },
              { CHSV(80, 255, 16), ForestColors_p, true, true },
              { CHSV(0, 0, 0), ForestColors_p, true, false },
              { CHSV(0, 0, 0), PartyColors_p, false, true },
            };

  
  uint8_t idx = random8(sizeof(stgs) / sizeof(stgs[0]) + 1);
  

  //Set background color, target palette and update rate
  if(stgs[idx].useBg){
    setHSV(stgs[idx].bg);                    
  }

  if(stgs[idx].usePal){
     _palTarget   = stgs[idx].pal;
  }
     

  //Reset palette index
  _indexPal = random8();

  //Change direction
  _dir = random8() % 2 == 0 ? true: false; 
}


inline void EffectMatrix::updateLeds(CRGB *leds, int numLeds){
  
  //First led color
  CRGB clr = random8(100) > 90 ? ColorFromPalette(_palCurrent, _indexPal, 255, LINEARBLEND) : getColor();
  
  if(!_dir){ //forward
    for (int i = numLeds - 1; i > 0; i--){
      leds[i] = leds[i-1];
    }

    //First updated led color
    leds[0] = clr;
  } 
  else { //backward
    for (int i = 0; i < numLeds - 1; i++){
      leds[i] = leds[i+1];
    }

    //First updated led color
    leds[numLeds - 1] = clr;
  }


  //Change palette index
  if(_hueInc) {
    _indexPal++;
  }
}


#endif //__NOISE_H
