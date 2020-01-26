#ifndef __EFFECT_SOUND_H
#define __EFFECT_SOUND_H

#include "SoundCapture.h"

////////////////////////////////////////////
// EffectSound - sound effect template
// This template is intended to instantiated with Effect, EffectColor or classes inhertied from them except for EffectPaletteTransform
// 


template <class T>
class EffectSound: public T{
  public:
    EffectSound();
    ~EffectSound();
    
    virtual void idle();
    
  protected:
    virtual void reset();
    virtual void proceed(CRGB *leds, int numLeds);

    virtual void updateLeds(CRGB *leds, 
                            int numLeds, 
                            const SoundCaptureData &data) = 0;
};


template<class T>
EffectSound<T>::EffectSound(){
  T::setSpeedDelay(10);
}

template<class T>
EffectSound<T>::~EffectSound(){
}


template<class T>
void EffectSound<T>::reset(){

  //Retrive instance
  SoundCapture *sc = SoundCapture::getInstance();
  
  if(sc){
    //Reset instance
    sc->reset();
  }

  T::reset();
}

template<class T>
void EffectSound<T>::proceed(CRGB *leds, int numLeds){

  //Retrive instance
  SoundCapture *sc = SoundCapture::getInstance();

  if(sc){
    
    //Retrieve data
    SoundCaptureData data;
    sc->getData(data);

    //Show leds
    updateLeds(leds, numLeds, data);
  }
}



template<class T>
 void EffectSound<T>::idle(){    

  //Retrive instance
  SoundCapture *sc = SoundCapture::getInstance();

  if(sc){
    //Call idle
    sc->idle();
  }
}

//////////////////////////////////////
// EffectSoundPT - sound effect with palette transformation template
// This template is intended to use with EffectPaletteTransform or classes inherited from it
//


template<class T>
class EffectSoundPT: public EffectSound<T>{
  public:
    EffectSoundPT();
    ~EffectSoundPT();

  
  protected:
    //Need to be overitten
    virtual void proceed(CRGB *leds, int numLeds);
  
};

template<class T>
EffectSoundPT<T>::EffectSoundPT(){
}

template<class T>
EffectSoundPT<T>::~EffectSoundPT(){
}


template<class T>
void EffectSoundPT<T>::proceed(CRGB *leds, int numLeds){
    T::onStep();
    EffectSound<T>::proceed(leds, numLeds);
}



#endif //__EFFECT_SOUND_H


