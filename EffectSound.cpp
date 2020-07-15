#include "LedEffects.h"

#include <Fastled.h>
#include <Controls.h>

#include "EffectEngineCtx.h"
#include "Effect.h"
#include "EffectSound.h"

#include <SoundCapture.h>



////////////////////////////////////
// EffectSound

#define SOUND_LOWER_MIN       0
#define SOUND_UPPER_MAX     255

SoundCapture *EffectSound::_sc = NULL;   

uint8_t EffectSound::_lower    = SOUND_LOWER_MIN; 
uint8_t EffectSound::_upper    = SOUND_UPPER_MAX;
uint8_t EffectSound::_max      = 0;
uint8_t EffectSound::_min      = 255;


EffectSound::EffectSound(){
  setSpeedDelay(10);
}

EffectSound::~EffectSound(){
}


void EffectSound::initSoundCapture(SoundCapture *sc){
    //Remember instances
    _sc = sc;
    //Init instance
    _sc->init();
}

void EffectSound::reset(){

    //Reset instance
    _sc->reset();

    Effect::reset();
}

#define SMOOTH_C 0.95

void EffectSound::getBands(band8_visual &output){
    uint8_t max     = 0;
    uint8_t min     = 255;
 
    //Get band data
    _sc->getData(output.bands, MAX_BANDS);

    //Process data
    for(int i = 0; i < MAX_BANDS; i++){

        //Calculate first
        if( output.bands[i] > max)
            max = output.bands[i];

        if(output.bands[i] < min)
            min = output.bands[i];

        //DBG_OUT("%u ", output.bands[i]);     

        //Apply boundary 
        output.bands[i] = output.bands[i] <= _lower ? 0 : map(output.bands[i], _lower, _upper, 0, 255);                
    }
    
    //Max
    _max = _max * SMOOTH_C + max * (1 - SMOOTH_C);

    //Min
    _min = _min * SMOOTH_C + min * (1 - SMOOTH_C);

    //Translae other parameters
    output.lower   = _lower;
    output.upper   = _upper;
    output.max     = _max;
    output.min     = _min;

    DBG_OUTLN(" - %d, %d, %d,  %d", output.lower, output.upper, output.min, output.max);     
}

void EffectSound::proceed(CRGB *leds, int numLeds){    
    
    //Retrieve data
    band8_visual output;    
    getBands(output);

    //Show leds
    updateLeds(leds, numLeds, output);  
}


 void EffectSound::idle(){   
    //Call idle
    _sc->idle();
}


void EffectSound::onCmd(const struct CtrlQueueItem &itm){  
  switch(itm.cmd){    
    case EEMC_SOUND_LOW:
      //Remember lower
      _lower = itm.data.translate(_lower, SOUND_LOWER_MIN, _upper);
    break;
    case EEMC_SOUND_HIGH:
      //Recalcualte upper
      _upper = itm.data.translate(_upper, _lower, SOUND_UPPER_MAX);
    break;
    default:
      Effect::onCmd(itm);
    break;
  }    
}


