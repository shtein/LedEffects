#include "precomp.h"
#include "EffectControls.h"
#include "EffectEngineCtx.h"

EffectControls::EffectControls(): _testPot(0, 0, 300), _testBut(7), _testBut2(6), _remote(2) {
  _maxEffects = 0;
  _effectNum  = 0;
  _speedDelay = 0;
  _numLeds    = 0;
}

EffectControls::~EffectControls(){
}

void EffectControls::init(struct EffectEngineCtx &ctx){    

  ctx.cf      = EEMC_MODE | EEMC_NUMLEDS;
  ctx.mode    = EEM_STATIC;

   //This is where to read from EEPROM
  _maxEffects = ctx.maxEffects;
  _effectNum  = ctx.effectNum;
  _numLeds    = ctx.numLeds;
  _speedDelay = ctx.speedDelay;
  _mode       = ctx.mode;
}

void EffectControls::loop(struct EffectEngineCtx &ctx){
  //Read controls
  readControls();
  
  //First see if we need to cleanup anything
  if(ctx.cf != EEMC_NONE){
     //Proceed with cleanup, remember what's to be remembered

     //Effect was changed, remember current speed and color 
     if(ctx.cf & (EEMC_EFFECT | EEMC_MODE)){
        setSpeedDelay(ctx.speedDelay);
        setColor(ctx.color);
     }

     //Reset flag
     ctx.cf = EEMC_NONE;
  }

  _mode = getMode();
  if(_mode != ctx.mode){
    ctx.cf  |= EEMC_MODE;
    ctx.mode = _mode;
  }
  
  
  //Current effect
  _effectNum = getCurrentEffect();
  if(_effectNum != ctx.effectNum){
    ctx.cf       |= EEMC_EFFECT; 
    ctx.effectNum = _effectNum;
  }

  //Speed Delay
  _speedDelay = getSpeedDelay();
  if(_speedDelay != ctx.speedDelay){
    ctx.cf         |= EEMC_SPEED;
    ctx.speedDelay  = _speedDelay; 
  }

  //Color
  _color = getColor();
  if(_color != ctx.color){
    ctx.cf    |= EEMC_COLOR;
    ctx.color  = _color; 
  }

  //Number of leds
  _numLeds = getNumLeds();
  if(_numLeds != ctx.numLeds){
    ctx.cf      |= EEMC_NUMLEDS;
    ctx.numLeds = _numLeds;
  }
}


int EffectControls::getSpeedDelay() const{
  return _speedDelay;
}

void EffectControls::setSpeedDelay(int speedDelay){
  _speedDelay = speedDelay;
}


int EffectControls::getCurrentEffect() const{
  //Not effect mode - nothing to change
   if(_mode != EEM_EFFECT){
      return _effectNum; 
   }

   int effectNum = _effectNum;
   
   if(_remote.pushed(RKEY_LEFT)){
      effectNum = (effectNum + _maxEffects - 1) % _maxEffects;
   }
  
   if(_remote.pushed(RKEY_RIGHT)){
      effectNum = (effectNum + 1) % _maxEffects;
   }

   return effectNum;
}

void EffectControls::setCurrentEffect(int curEffect){
  _effectNum = curEffect;
}


CRGB EffectControls::getColor() const{
  return _color;
}

void EffectControls::setColor(const CRGB color){
  _color = color;
}


int EffectControls::getNumLeds() const {
  return _numLeds;
}

void EffectControls::setNumLeds(int numLeds)  {
  _numLeds = numLeds;
}

int EffectControls::getMode() const{
  return _remote.pushed(RKEY_OK) ? (_mode + 1) % EEM_MODEMAX : _mode;
}

void EffectControls::readControls(){
  _testPot.read();
  _testBut.read();
  _testBut2.read();
  _remote.read();
}

