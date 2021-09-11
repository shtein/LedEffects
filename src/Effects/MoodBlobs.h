#ifndef __MOODBLOBS_H
#define __MOODBLOBS_H


#define BLOB_SIZE  10

class EffectMoodBlobs: public Effect{
  public:
    EffectMoodBlobs();
    ~EffectMoodBlobs();

protected:
    void reset();
    void proceed(CRGB *leds, uint16_t numLeds); 

protected:
  byte _blobPhase;
};


inline EffectMoodBlobs::EffectMoodBlobs(){
  setSpeedDelay(100);
  _blobPhase  = 0;
}

inline EffectMoodBlobs::~EffectMoodBlobs(){
}


void EffectMoodBlobs::reset(){
  _ctx.step  = 0;
  _blobPhase = 0;
}

void EffectMoodBlobs::proceed(CRGB *leds, uint16_t numLeds){

  for(uint16_t i = 0; i < numLeds; i++){
    //Get base color
    CHSV hsv = CHSV(HUE_RED, 0xFF, 0xFF);
    
    //Calculate color value
    int val = (int)sin8(-_blobPhase + (i % BLOB_SIZE) * 255 / BLOB_SIZE) - 128;    
    //Everything below x axis is 0
    hsv.value = val < 0 ? 0 : val * 2; 

    //Calculate hue
    int index = (i + _ctx.step / 2) % numLeds;
    int hue   = ((int)sin8( index * 255 / numLeds) - 128 ) ;   
    hsv.hue += hue; 
   
    //Set color
    setPixel(leds[i] , hsv);
  }

  //Increment blob phase
  _blobPhase += 2;

  //Increment color rotation step
  _ctx.step = (_ctx.step + 1) % ( numLeds * 2 );

}


#endif //__MOODBLOBS_H
