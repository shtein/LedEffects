#include "LedEffects.h"
/*
#include <Fastled.h>
#include <Controls.h>

#include "EffectEngineCtx.h"
#include "Effect.h"
#include "EffectSound.h"

#include <utils.h>
#include <SoundCapture.h>


////////////////////////////////////
// SoundStat

#define SOUNDSTAT_MAX_COUNT 35

Stats::Stats(){
  reset();
}

void Stats::reset(){
  _minCur = 255;
  _maxCur = 255;

  _mins.reset();
  _maxs.reset();
}



void Stats::process(uint8_t val){

  //Save only if less then current  
  if(val < _minCur)
    _minCur = val;

  //Save only if more then current  
  if(val > _maxCur)
    _maxCur = val;  

  //Check if it time to save current
  _count ++;

  if(_count == SOUNDSTAT_MAX_COUNT){

      
    //Add latest values
    _mins.add(_minCur);
    _maxs.add(_maxCur);

    //Reset counter
    _count    = 0;

    //Remember the latest value
    _minCur   = 255;
    _maxCur   = 0;
  }

}


uint8_t Stats::getMax() const{

  if(_maxs.count() == 0)
    return _maxCur;

  uint8_t m = _maxs[0];
  for(int i = 1; i < _maxs.count(); i++){
    if(m < _maxs[i])
      m = _maxs[i];
  }

  return m;
}

uint8_t Stats::getMin() const{

  if(_mins.count() == 0)
    return _minCur;

  uint8_t m = _mins[0];
  for(int i = 1; i < _mins.count(); i++){
    if(m < _mins[i])
      m = _mins[i];
  }

  return m;
}


////////////////////////////////////
// EffectSound

#define SOUND_LOWER_MIN           0
#define SOUND_UPPER_MAX           255
#define SOUND_SENSITIVITY_DEFAULT 32

SoundCapture *EffectSound::_sc = NULL;   

uint8_t EffectSound::_lower       = SOUND_LOWER_MIN; 
uint8_t EffectSound::_upper       = SOUND_UPPER_MAX;
uint8_t EffectSound::_sensitivity = SOUND_SENSITIVITY_DEFAULT;
uint8_t EffectSound::_flags       = SC_MAP_ABOVE_NOISE | SC_MAP_SENSITIVITY | SC_MAP_USE_MAX | SC_MAP_USE_MIN| SC_MAP_LOG;

Stats EffectSound::_stats;


EffectSound::EffectSound(){  
}

EffectSound::~EffectSound(){
}


void EffectSound::initSoundCapture(SoundCapture *sc){
    //Remember instances
    _sc = sc;
    //Init instance
    _sc->init();
}

//Default palette
DEFINE_GRADIENT_PALETTE( sound_bands_gp ) {
      0,   0xBF,   0,    0,   // red
      //16,   0xFF,   0,    0,   // red
      128,    0, 0xBF,   0,   // green   
      //219,    0,   0,  0xFF,   // blue
      255,    0,   0,  0xBF,   // blue
}; 

void EffectSound::reset(){

    //Reset instance
    _sc->reset();

    //Remember pallette
    _ctx.palCurrent = sound_bands_gp;
    
    //Speed
    setSpeedDelay(10);
}


void EffectSound::getBands(band8_visual &output){
  //Get band data
  _sc->getData(output.bands, SC_MAX_BANDS);

  //Process data
  for(int i = 0; i < SC_MAX_BANDS; i++){      
      //Calculate stat
      _stats.process(output.bands[i]);

      //DBG_OUT("%u ", output.bands[i]);     
  }


 //Translae other parameters  
  output.max          = _stats.getMax();  
  output.min          = _stats.getMin();
  output.sensitivity  = _sensitivity;
  
  output.upper        = _flags & SC_MAP_USE_MAX ? SOUND_UPPER_MAX : _upper;
  output.lower        = _flags & SC_MAP_USE_MIN ? SOUND_LOWER_MIN : _lower < output.upper ? _lower : output.upper;
  
  output.flags        = _flags;

  //DBG_OUTLN(" %d, %d / %d, %d / %d", output.lower, output.upper, output.min, output.max, output.sensitivity);     
}

void EffectSound::proceed(CRGB *leds, uint16_t numLeds){    
    
    //Retrieve data
    band8_visual output;    
    getBands(output);

    //Show leds
    updateLeds(leds, numLeds, output);  
}

bool EffectSound::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){  
  switch(itm.cmd){    
    case EEMC_SOUND_LOW:
      _lower = itm.data.translate(_lower, SOUND_LOWER_MIN, SOUND_UPPER_MAX);
    break;

    case EEMC_SOUND_HIGH:
      _upper = itm.data.translate(_upper, SOUND_LOWER_MIN, SOUND_UPPER_MAX);
    break;

    case EEMC_SOUND_SENSITIVITY:
      _sensitivity = itm.data.translate(_sensitivity, SOUND_LOWER_MIN, SOUND_UPPER_MAX);
    break;

    case EEMC_SOUND_LOG:
      _flags = itm.data.value == 0 ? _flags & ~SC_MAP_LOG : _flags | SC_MAP_LOG; 
    break;

    case EEMC_SOUND_USE_MAX:
      _flags = itm.data.value == 0 ? _flags & ~SC_MAP_USE_MAX : _flags | SC_MAP_USE_MAX; 
    break;

    case EEMC_SOUND_USE_MIN:
      _flags = itm.data.value == 0 ? _flags & ~SC_MAP_USE_MIN : _flags | SC_MAP_USE_MIN; 
    break;

    default:
    return Effect::onCmd(itm, ntf);
  }   

  return true;
}


void EffectSound::updateLeds(CRGB *leds, uint16_t numLeds, const band8_visual &data){

  //Number of sections
  int bandLeds = numLeds / SC_MAX_BANDS;
    
 
 for(int i = 0; i < SC_MAX_BANDS; i++){
  drawBand(leds + i * bandLeds, 
           bandLeds, 
           //i % 2 == 0 ? bandLeds - 2 : 1,
           //bandLeds - 2,
           //1,
           bandLeds / 2,
           //0,
           data.scale(i),            
           map(i, 0, SC_MAX_BANDS, 0, 255 ),
           map(i + 1, 0, SC_MAX_BANDS, 0, 255 )
          );
 }

}

//Primitives
#define SS_FADE_FACTOR_MIN 20
#define SS_FADE_FACTOR_MAX 65

void EffectSound::drawBand(CRGB *leds,
                           uint16_t numLeds,
                           uint16_t center,
                           uint8_t volume, 
                           uint8_t colorIndexFirst,
                           uint8_t colorIndexLast
                          ) {

  //Draw center
  if(volume > 0)
    leds[center] = ColorFromPalette(_ctx.palCurrent, colorIndexFirst, 255, NOBLEND);
  else
    leds[center].fadeToBlackBy(SS_FADE_FACTOR_MIN);    
  
  //Calculate parameters for bottom part and top part
  uint8_t firstStep  = 255 / max(center + 1, numLeds - center);
  
  //Draw bottom
  uint8_t step       = center == 0 ? 0 : (255 - firstStep) / center;
  uint8_t colorInc   = center == 0 ? 0 : (colorIndexLast - colorIndexFirst) / center;
  uint8_t fadeFactor = (SS_FADE_FACTOR_MAX - SS_FADE_FACTOR_MIN) / ( center + 1 );
  
  for(uint16_t i = 0; i < center; i++){    
    if(volume > firstStep + step * (center - i - 1))
      leds[i] = ColorFromPalette(_ctx.palCurrent, colorIndexLast - i * colorInc, 255, NOBLEND);
    else
      leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (center - i) * fadeFactor);    
  }

  
  //Draw top
  step       = numLeds - center + 1 == 0 ? 0 : (255 - firstStep) / (numLeds - center + 1);
  colorInc   = numLeds - center + 1 == 0 ? 0 : (colorIndexLast - colorIndexFirst) / (numLeds - center + 1);
  fadeFactor = (SS_FADE_FACTOR_MAX - SS_FADE_FACTOR_MIN) /  (numLeds - center);
  
  for(uint16_t i = center + 1; i < numLeds; i++){     
  
    if(volume > firstStep + step * (i - center - 1))
      leds[i] = ColorFromPalette(_ctx.palCurrent, colorIndexFirst + i * colorInc, 255, NOBLEND);
    else
      leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (i - center) * fadeFactor);
  }  

}

void EffectSound::drawBandSymmetric(CRGB *leds,
                                    uint16_t numLeds,
                                    uint8_t volume,
                                    uint8_t colorIndexFirst, 
                                    uint8_t colorIndexLast
                                  ){  
  //First led is in the middle
  CRGB *first = leds +  numLeds / 2 + numLeds % 2;
  //Draw first half      
  drawBand(first, 
           numLeds / 2, 
           0,
           volume,
           colorIndexFirst,
           colorIndexLast
         );

  //Copy the rest
  int offs =  1 - numLeds % 2;
  for(uint16_t i = 0;  i < numLeds / 2; i++)
      first[-i - offs] = first[i];
} 

void EffectSound::drawBoundary(CRGB *leds, 
                               uint16_t numLeds, 
                               uint8_t bottom, 
                               uint8_t lower, 
                               uint8_t upper,
                               const CRGB &clrLower,
                               const CRGB &clrUpper,
                               bool eraseBackground
                              ){

  if(eraseBackground)
    fill_solid(leds, numLeds, CRGB::Black);    

  leds[mapEx(lower, 0, 255, bottom, numLeds - 1 - bottom)] += clrLower;
  leds[mapEx(upper, 0, 255, bottom, numLeds - 1 - bottom)] += clrUpper;    
}

*/