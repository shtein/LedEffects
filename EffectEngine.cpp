#include "precomp.h"
#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include "pins.h"


EffectEngine::EffectEngine(){
  _curEffect  = NULL;
  _numEffects = 0;
  _effectNum  = 0;
  ::memset(_effects, 0, sizeof(_effects));

  _numLeds = MAX_LEDS;
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

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

void EffectEngine::init(int numLeds, uint8_t mode) {  
  //Init LEDs
  FastLED.addLeds<WS2812B, LED_PIN>(_leds, MAX_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.addLeds<NEOPIXEL, LED_PIN>(_leds, MAX_LEDS).setCorrection( TypicalLEDStrip );  
  //FastLED.addLeds<WS2801, LED_PIN, LED_CLOCK, RGB>(_leds, MAX_LEDS).setCorrection( TypicalLEDStrip );
  

  //Save initial values
  _numLeds = numLeds;
  _mode    = 0xFF; 

  //Set mode
  setMode(mode);

  //Light LEDs
  showStrip();
}

void EffectEngine::showStrip() {
  FastLED.show();
  //delay(1);
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
      Serial.print("mode off\n");
    break;
    case EEM_STATIC: //Static light
      _curEffect = &_eStatic;
      Serial.print("mode static\n");
     break;
     case EEM_EFFECT:
      _curEffect =  _effects[_effectNum];
      Serial.print("mode effect\n");
     break;
  }  

  //Black the lights
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

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
  int numLeds = data.translate(_numLeds, 0, MAX_LEDS);

  //Do nothing if did not change
  if(_numLeds == numLeds)
    return;

  //Save new value
  _numLeds = numLeds;
  

 //Black the lights
  fill_solid(_leds + _numLeds, MAX_LEDS - _numLeds, CRGB::Black);

  //Refresh effect
  //setEffect(_curEffect);

  Serial.println("EffectEngine::onNumLedsChange");
}

void EffectEngine::onEffectChange(const struct CtrlQueueData &data){
  
  //Change effect only if in effect mode
  if(_mode != EEMC_EFFECT)
    return; 

  //Get new mode value
  _effectNum = (uint8_t)data.translate(_effectNum, 0, _numEffects - 1);

  
  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

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

