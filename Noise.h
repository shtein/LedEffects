#ifndef __NOISE_H
#define __NOISE_H

#define MAX_PAL_CHANGES 24
#define CHANGE_PAL_STEP 500

/////////////////////////////////////////
// Effect Palette Transform - basic palette transformation
class EffectPaletteTransform: public Effect{
  public: 
    EffectPaletteTransform();
   ~EffectPaletteTransform();

   virtual void proceed(CRGB *leds, int numLeds); 
   virtual void reset();

  protected:
    virtual void updateColors();
    virtual void updateLeds(CRGB *leds, int numLeds);

    virtual int getPalClrIndex(int ledIndex, int numLeds) const;
    virtual CRGBPalette16 getNewPal() const;
    virtual bool isReadyToBlendPal() const;
    virtual bool isReadyToChangePal() const;
    virtual uint8_t getMaxPaxPalChanges() const;
    
  protected:
    CRGBPalette16 &_palCurrent;
    CRGBPalette16 &_palTarget;
    
    int           _step;
};

inline EffectPaletteTransform::EffectPaletteTransform(): 
                                _palCurrent (allocPalette(0)), 
                                _palTarget (allocPalette(1)) {
  setSpeedDelay(25);

  //Allocate palttetes
  
  
}

inline EffectPaletteTransform::~EffectPaletteTransform(){
  
}

inline void EffectPaletteTransform::reset(){
  _palCurrent = getNewPal();
  _palTarget  = getNewPal();
  
  _step       = 0;
}


inline int EffectPaletteTransform::getPalClrIndex(int /*ledIndex*/, int /*numLeds*/) const{
  //First index by default - need to experiment with this
  return 0;
}

inline bool EffectPaletteTransform::isReadyToBlendPal() const{
  return true;
}

bool EffectPaletteTransform::isReadyToChangePal() const{
  return _step == CHANGE_PAL_STEP ? true : false;
}

inline CRGBPalette16 EffectPaletteTransform::getNewPal() const{
  //Random palette
  return CRGBPalette16(CHSV(random8(), 255, random8(128,255)), 
                       CHSV(random8(), 255, random8(128,255)), 
                       CHSV(random8(), 192, random8(128,255)), 
                       CHSV(random8(), 255, random8(128,255))
                      );
}

uint8_t EffectPaletteTransform::getMaxPaxPalChanges() const{
  return MAX_PAL_CHANGES;
}

inline void EffectPaletteTransform::updateColors(){
  //Change target palette
  _palTarget = getNewPal();
}

inline void EffectPaletteTransform::updateLeds(CRGB *leds, int numLeds){  
  for(int i = 0; i < numLeds; i++){                                    
    leds[i] = ColorFromPalette(_palCurrent, getPalClrIndex(i, numLeds), 255, LINEARBLEND);
  }
}

inline void EffectPaletteTransform::proceed(CRGB *leds, int numLeds){
  
  //Check if it is to update target palette
  if(isReadyToChangePal()){

    //Reset step
    _step = 0;       

    //Update palette
    updateColors();
  }
  
  
  //Proceed with palette transtion
  if(isReadyToBlendPal()){
    nblendPaletteTowardPalette(_palCurrent, _palTarget, getMaxPaxPalChanges()); 
  }


  //Set colors
  updateLeds(leds, numLeds);

  //Prepare for the next move                                        
  _step++;
}

////////////////////////////
// Effect EffectPaletteTransformFast - simple transition between paletees
class EffectPaletteTransformFast: public EffectPaletteTransform{
  public:
    EffectPaletteTransformFast();
    ~EffectPaletteTransformFast();

  protected:
    int getPalClrIndex(int ledIndex, int numLeds) const;
    bool isReadyToChangePal() const;
    
};

inline EffectPaletteTransformFast::EffectPaletteTransformFast(){
}

inline EffectPaletteTransformFast::~EffectPaletteTransformFast(){
}

inline int EffectPaletteTransformFast::getPalClrIndex(int ledIndex, int numLeds) const{
  //Full range
  return map(ledIndex, 0, numLeds - 1, 0, 65535);
}

inline bool EffectPaletteTransformFast::isReadyToChangePal() const{
  return _step > MAX_PAL_CHANGES * 3 ? true : false;
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
    int           _dist;
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

    bool isReadyToChangePal() const;

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


inline bool EffectConfetti::isReadyToChangePal() const{
  return _step == 100 ? true : false;
}

void EffectConfetti::updateLeds(CRGB *leds, int numLeds){
  
  fadeToBlackBy(leds, numLeds, _fade);                     
  leds[random16(numLeds)] = ColorFromPalette(_palCurrent, _hue + random8(_hueDiff) / 4 , 255, LINEARBLEND);
  
  _hue += _hueInc;
}


void EffectConfetti::updateColors(){
  
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


/////////////////////////////////////////
// Effect Fire
class EffectFire: public Effect{
  public:
    EffectFire();
    ~EffectFire();
  
    void proceed(CRGB *leds, int numLeds); 
    void reset();
};


inline EffectFire::EffectFire(){ 

  setSpeedDelay(20);
}

inline EffectFire::~EffectFire(){
}

inline void EffectFire::reset(){  
}

#define FIRE_X_SCALE 20 
#define FIRE_Y_SCALE 3

inline void EffectFire::proceed(CRGB *leds, int numLeds){  
  CRGBPalette16 pal = HeatColors_p;
  
  for(int i = 0; i < numLeds; i++) {        
    leds[i] = ColorFromPalette(pal, 
                              min( i * (inoise8(i * FIRE_X_SCALE, millis() * FIRE_Y_SCALE * numLeds / 255)) >> 6, 255),
                              255,
                              LINEARBLEND ); 
  }
}


#endif //__NOISE_H
