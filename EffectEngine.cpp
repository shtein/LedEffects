#include "precomp.h"
#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"


EffectEngine::EffectEngine(){
  _curEffect  = NULL;
  _numEffects = 0;
  _effectNum  = 0;
  ::memset(_effects, 0, sizeof(_effects));

  _numLeds = 0;
  _maxLeds = 0;
  _leds    = 0;

  _mode = EEM_OFF;
}

EffectEngine::~EffectEngine(){
}

void EffectEngine::addEffect(Effect *effect){
  if(!effect)
    return;

  if(_numEffects == sizeof(_effects)/sizeof(_effects[0]) - 1)
    return;

  _effects[_numEffects] = effect;
  _numEffects ++;
}  

void EffectEngine::init(CRGB *leds, int maxLeds, uint8_t mode) {  
 
  //Don't uncomment it if you don;t know what it is 
  //FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);

  _leds    = leds;
  _numLeds = maxLeds;
  _maxLeds = maxLeds;
  fill_solid(_leds, _maxLeds, CRGB::Black);
    
  _mode    = 0xFF; 

  //Set mode
  setMode(mode);

  //Light LEDs
  showStrip();
}

void EffectEngine::showStrip() {
  FastLED.show();
}


void EffectEngine::onModeChange(const struct CtrlQueueData &data){ 
  //Get new mode value  
  uint8_t mode = (uint8_t)data.translate(_mode, EEM_OFF, EEM_MODEMAX);
  
  //Do nothing if did not change
  if(_mode == mode)
    return;

  //Change mode
  setMode(mode);        
}

void EffectEngine::setMode(uint8_t mode){
  _mode = mode;
  
  switch(_mode){
    case EEM_OFF: //Off
      _curEffect = NULL;
      DBG_OUTLN("mode off");
    break;
    case EEM_STATIC: //Static light
      _curEffect = &_eStatic;
      DBG_OUTLN("mode static");
     break;
     case EEM_EFFECT:
      _curEffect =  _effects[_effectNum];
      DBG_OUTLN("mode effect");
     break;
  }  

  //Black the lights
  fill_solid(_leds, _maxLeds, CRGB::Black);

  //Refresh effect
  setEffect(_curEffect);
 
}

void EffectEngine::setEffect(Effect *effect){  
  _curEffect = effect;
  
  if(_curEffect != NULL){
    //Init effect
    _curEffect->init(_leds, _numLeds);

    //Process right away
    _millis = 0;
  }
}

void EffectEngine::onNumLedsChange(const struct CtrlQueueData &data){
  
  //Get new mode value
  int numLeds = data.translate(_numLeds, 0, _maxLeds);

  //Do nothing if did not change
  if(_numLeds == numLeds)
    return;

  //Save new value
  _numLeds = numLeds;

  DBG_OUT("new number of leds: ");
  DBG_OUTLN(_numLeds);
  

 //Black the lights
  fill_solid(_leds + _numLeds, _maxLeds - _numLeds, CRGB::Black);

  //Refresh effect
  //setEffect(_curEffect);
}

void EffectEngine::onEffectChange(const struct CtrlQueueData &data){
  
  //Change effect only if in effect mode
  if(_mode != EEMC_EFFECT)
    return; 

  //Get new mode value
  _effectNum = (uint8_t)data.translate(_effectNum, 0, _numEffects - 1);

  
  //Black the leds
  fill_solid(_leds, _maxLeds, CRGB::Black);

  //Change effect
  setEffect(_effects[_effectNum]);
}

void EffectEngine::onColorChange(int index, const struct CtrlQueueData &data){
  //Check if effect is there
  if(!_curEffect)
    return;

  //Get effect color
  CHSV hsv = _curEffect->getHSV();

  //Update corresponding color value
  hsv.raw[index] = (uint8_t)data.translate( (int)hsv.raw[index], 0, 255);
  
  //Set effect color
  _curEffect->setHSV(hsv);
}


void EffectEngine::onSpeedChange(const struct CtrlQueueData &data){
  //React inly in effect mode
  if(_mode != EEM_EFFECT)
    return;
  
  //Check if effect is there
   if(!_curEffect)
    return;

  //Set new speed delay
  _curEffect->setSpeedDelay(data.translate(_curEffect->getSpeedDelay(), SPEED_DELAY_MIN, SPEED_DELAY_MAX));
}

void EffectEngine::loop(const struct CtrlQueueItem &itm){

  //Process command
  switch(itm.cmd){
    case EEMC_MODE:
      onModeChange(itm.data);
    break;
    case EEMC_NUMLEDS:
      onNumLedsChange(itm.data);
    break;
    case EEMC_EFFECT:
      onEffectChange(itm.data);
    break;
    case EEMC_COLOR_HUE:
    case EEMC_COLOR_SAT:
    case EEMC_COLOR_VAL:
      onColorChange(itm.cmd - EEMC_COLOR_HUE, itm.data);
    break;
    case EEMC_SPEED:
      onSpeedChange(itm.data);
    break;
  };

  //Proceed with current effect
  bool updateLeds = itm.cmd != EEMC_NONE;

  if(_curEffect != NULL){    
    //Is it time to process ?
     if(_millis <= millis()){

        //Proceed
        _curEffect->loop(_leds, _numLeds);
        
        //Remember when proceed next time
        _millis = millis() + _curEffect->getSpeedDelay();
        
        updateLeds = true;
     }
      
  }
  
  //Stupid optimization as a workaroud for IR Remote conflicting with ws2811, ws2812 and ws2812b
  if(updateLeds){
     showStrip();    
  }
}

