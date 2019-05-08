#ifndef __SOUND_H
#define __SOUND_H

#include "SoundCapture.h"

/////////////////////////////////////////
//SoundCaptureStub - for testing

class SoundCaptureStub:public SoundCapture{
public:
    void init();
    void reset(); 
    void idle(); 
    void getData(SoundCaptureData &data) const;
};


inline void SoundCaptureStub::init(){
}

inline void SoundCaptureStub::reset(){
}

inline void SoundCaptureStub::idle(){
}

inline void SoundCaptureStub::getData(SoundCaptureData &data) const{

  for(int i = 0; i < MAX_BANDS; i++){
    data.bands[i] = random8();
  }
}


////////////////////////////////
// EffectSoundSimple - very basic sound effect

class EffectSoundSimple: public EffectSound<Effect>{

  protected:
    virtual void updateLeds(CRGB *leds, int numLeds, const SoundCaptureData &data); 
};

void EffectSoundSimple::updateLeds(CRGB *leds, int numLeds, const SoundCaptureData &data){

  int inc = numLeds / MAX_BANDS; 

  for(int i = 0; i < MAX_BANDS; i++){
    leds[i*inc] = CHSV(HUE_RED, 0xFF, data.bands[i]); 
  }
}

#endif//__SOUND_H
