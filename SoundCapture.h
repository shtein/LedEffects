#ifndef __SOUNDCAPTURE_H
#define __SOUNDCAPTURE_H

////////////////////////////
// Sound capture base interface

#define MAX_BANDS 7
typedef struct {
  uint8_t bands[MAX_BANDS]; //8-bit data for each band
} SoundCaptureData;

class SoundCapture{
  public:
    SoundCapture();
    ~SoundCapture();

    //Single instance operations
    static void initInstance(SoundCapture *instance); 
    static SoundCapture* getInstance(); 

    //Interface functions
    virtual void init()                                = 0;  //Initialization
    virtual void reset()                               = 0;  //Resetting
    virtual void idle()                                = 0;  //Read data from source and process it
    virtual void getData(SoundCaptureData &data) const = 0;  //Retrieve the data

  private:
    static SoundCapture *_instance;
};

////////////////////////////////////////////
// EffectSound - sound effect template
template <class T>
class EffectSound: public T {
  public:
    EffectSound();
    ~EffectSound();
    
    virtual void idle();
    
  protected:
    virtual void reset();
    virtual void proceed(CRGB *leds, int numLeds);

    virtual void updateLeds(CRGB *leds, int numLeds, const SoundCaptureData &data) = 0;
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
  SoundCapture *sc = SoundCapture::getInstance();

  if(sc){
    sc->reset();
  }

  T::reset();
}

template<class T>
void EffectSound<T>::proceed(CRGB *leds, int numLeds){
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
  SoundCapture *sc = SoundCapture::getInstance();

  if(sc){
    sc->idle();
  }
}

//////////////////////////////////////
// EffectSoundPaletteTransform - sound effect with palette transformation template

template<class T>
class EffectSoundPaletteTransform: public EffectSound<T>{
  public:
    EffectSoundPaletteTransform();
    ~EffectSoundPaletteTransform();

  
  protected:
    virtual void proceed(CRGB *leds, int numLeds);
  
};

template<class T>
EffectSoundPaletteTransform<T>::EffectSoundPaletteTransform(){
}

template<class T>
EffectSoundPaletteTransform<T>::~EffectSoundPaletteTransform(){
}


template<class T>
void EffectSoundPaletteTransform<T>::proceed(CRGB *leds, int numLeds){
    T::onStep();
    EffectSound<T>::proceed(leds, numLeds);
}


#endif //__SOUNDCAPTURE_H
