#ifndef __MOODBLOBS_H
#define __MOODBLOBS_H


#define BLOB_SIZE  10

class EffectMoodBlobs: public Effect{
  public:
    EffectMoodBlobs();
    ~EffectMoodBlobs();

protected:
    void reset();
    void proceed(int speedDelay); 

protected:
  uint8_t _blobPhase;
  int     _step;
};


inline EffectMoodBlobs::EffectMoodBlobs(){
  setSpeedDelay(100);
  _blobPhase  = 0;
  _step       = 0;
  setHSV(CHSV(HUE_RED, 0xFF, 0xFF));
}

inline EffectMoodBlobs::~EffectMoodBlobs(){
}


void EffectMoodBlobs::reset(){
  _step       = 0;
  _blobPhase   = 0;
}

void EffectMoodBlobs::proceed(int speedDelay){

  //Blob led increment
  float valueInc = 255. / BLOB_SIZE;

  //Color increment
  float colorInc = 255. / getNumLeds() ;

  for(int i = 0; i < getNumLeds(); i++){
    //Get base color
    CHSV hsv = getHSV();      

    //Calculate color value
    int val = (int)sin8(- _blobPhase + (i % BLOB_SIZE) * valueInc) - 128;    
    //Everything below x axis is 0
    hsv.value = val < 0 ? 0 : val * 2; 

    //Calculate hue
    int index = (i + _step / 2) % getNumLeds();
    int hue   = ((int)sin8( index * colorInc) - 128 ) / 4;   
    hsv.hue += hue; 
   
    //Set color
    setPixel(i , hsv);
  }

  //Increment blob phase
  _blobPhase += 2;

  //Increment color rotation step
  _step = (_step + 1) % ( getNumLeds() * 2);

}


#endif //__MOODBLOBS_H
