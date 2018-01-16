#ifndef __RUNNING_LIGHS
#define __RUNNING_LIGHS

////////////////////
// Running Lights Effect


class EffectRunningLights: public Effect{
  public:
    EffectRunningLights();
    ~EffectRunningLights();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    uint8_t  _step;
};


inline EffectRunningLights::EffectRunningLights(){
  _step = 0;
  setHSV(CHSV(HUE_RED, 0xFF, 0xFF));
  setSpeedDelay(50);
}

inline EffectRunningLights::~EffectRunningLights(){
}

inline void EffectRunningLights::reset(){
  _step = 0;
}


#define RL_SIZE 6
inline void EffectRunningLights::proceed(CRGB *leds, int numLeds){
  
  for(int i = 0; i< numLeds; i++) {   
    
    CHSV hsv = getHSV();
    hsv.v  = sin8( ((i + _step) % RL_SIZE) * 255 / (RL_SIZE - 1) );
    setPixel(leds[i], hsv);
  }  
  
  _step = (_step + 1) % RL_SIZE;
}

///////////////////////////
// Effect Matrix, I have no idea why it is called matrix, it is its name in original code

#define MATRIX_STEP_MIN 200
#define MATRIX_STEP_MAX 100


class EffectMatrix: public Effect{
  public:
    EffectMatrix();
    ~EffectMatrix();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();    

    void updateColors();
    void updateLeds(CRGB *leds, int numLeds);

    CRGB getColor() const;
    CRGB getBgColor() const;

  protected:
    uint16_t  _step:9;        //current step    
    uint16_t  _dir:1;         //direction
    uint16_t  _hueInc:1;      //change hue
    uint16_t  _updateRate:5;  //steps between each update
    uint8_t   _indexPal;      //palette index

    CRGBPalette16 &_palCurrent;
    CRGBPalette16 &_palTarget;

    CRGBPalette16 &_palBgCurrent;
    CRGBPalette16 &_palBgTarget;   
};

inline EffectMatrix::EffectMatrix():
                                _palCurrent (allocPalette(0)), 
                                _palTarget (allocPalette(1)), 
                                _palBgCurrent (allocPalette(2)), 
                                _palBgTarget (allocPalette(3)) {
  setSpeedDelay(25);

  _step       = 0;
  _dir        = false;
  _hueInc     = true;
  _updateRate = 1;
  _indexPal   = 0;
  _updateRate = 0; 
}

inline EffectMatrix::~EffectMatrix(){
  
}

inline void EffectMatrix::reset(){  
  //Init with black
  _palCurrent = CRGBPalette16(CHSV(80, 0, 0));    
  _palTarget  = CRGBPalette16(CHSV(180, 0, 0));  

  //Blue background  
  _palBgCurrent = CRGBPalette16 (CHSV(80, 255, 255));
  _palBgTarget  = CRGBPalette16 (CHSV(180, 255, 255) );


  //Default update rate
  _updateRate = 2;        
  _step = random(MATRIX_STEP_MIN, MATRIX_STEP_MAX);  
}

inline void EffectMatrix::updateColors(){
  
   struct MatSettings {
    CHSV                          bg;
    const TProgmemRGBPalette16   &pal;
    uint8_t                       useBg:4;
    uint8_t                       usePal:4;
   } 
   stgs[] = { { CHSV(140, 255, 255), OceanColors_p, true, true },
              { CHSV(50, 255, 255), LavaColors_p, true, true },
              { CHSV(96, 255, 255), ForestColors_p, true, true },
              { CHSV(0, 0, 0), ForestColors_p, true, false },
              { CHSV(0, 0, 0), PartyColors_p, false, true },
            };

  
  uint8_t idx = random8(sizeof(stgs) / sizeof(stgs[0]) + 1);
  

  //Set background color, target palette and update rate
  if(stgs[idx].useBg){
    _palBgTarget =  CRGBPalette16(stgs[idx].bg);                    
  }

  if(stgs[idx].usePal){
     _palTarget   = stgs[idx].pal;
  }
     

  //Set number of passes
  _step = random(MATRIX_STEP_MIN, MATRIX_STEP_MAX);  

  //Reset palette index
  _indexPal = random8();

  //Change direction
  _dir = random8() % 2 == 0 ? true: false; 
}

inline CRGB EffectMatrix::getColor() const{
  return ColorFromPalette(_palCurrent, _indexPal, 255, LINEARBLEND);
}

inline CRGB EffectMatrix::getBgColor() const{
  return ColorFromPalette(_palBgCurrent, 0, 8, LINEARBLEND);
}


inline void EffectMatrix::updateLeds(CRGB *leds, int numLeds){
  
  //First led color
  CRGB clr = random8(100) > 90 ? getColor() : getBgColor();
  
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

inline void EffectMatrix::proceed(CRGB *leds, int numLeds){
  //See if need to change setup, i.e. palette, backround etc
  if(_step == 0){
    updateColors();
  }

  //See if need to change pallette of the sparckles
  if(_step % 2){ 
    nblendPaletteTowardPalette(_palCurrent, _palTarget, 24);  
  }

  if(_step % 4){ 
    nblendPaletteTowardPalette(_palBgCurrent, _palBgTarget, 24);  
  }


  //See if need to update colors
  if(_step % _updateRate == 0){
    updateLeds(leds, numLeds);
  }

  _step --;
}

#endif //__RUNNING_LIGHS
