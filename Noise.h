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
    virtual int getPalClrIndex(int ledIndex, int numLeds) const;
    virtual CRGBPalette16 getNewPal() const;
    virtual bool isReadyToBlendPal() const;
    virtual bool isReadyToChangePal() const;
    virtual uint8_t getMaxPaxPalChanges() const;

  protected:
    CRGBPalette16 _palCurrent;
    CRGBPalette16 _palTarget;
    
    int           _step;
};

inline EffectPaletteTransform::EffectPaletteTransform(){
  setSpeedDelay(25);
}

inline EffectPaletteTransform::~EffectPaletteTransform(){
  
}

inline void EffectPaletteTransform::reset(){
  _palCurrent = getNewPal();//CRGBPalette16(CRGB::Black);
  _palTarget  = getNewPal();
  
  _step       = 0;
}


inline int EffectPaletteTransform::getPalClrIndex(int ledIndex, int numLeds) const{
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

inline void EffectPaletteTransform::proceed(CRGB *leds, int numLeds){
  
  //Check if it is to update target palette
  if(isReadyToChangePal()){

    //Reset step
    _step = 0;
    
    //Change target palette
    _palTarget = getNewPal();
  }
  
  
  //Proceed with palette transtion
  if(isReadyToBlendPal()){
    nblendPaletteTowardPalette(_palCurrent, _palTarget, getMaxPaxPalChanges()); 
  }


  //Set colors
  for(int i = 0; i < numLeds; i++){                                    
    leds[i] = ColorFromPalette(_palCurrent, getPalClrIndex(i, numLeds), 255, LINEARBLEND);
  }

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
// class EffectBeatWave
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



#endif //__NOISE_H
