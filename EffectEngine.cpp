#include "precomp.h"
#include "Effect.h"
#include "EffectEngine.h"

EffectEngine::EffectEngine(){
  _curEffect  = NULL;
  _numEffects = 0;
  ::memset(_effects, 0, sizeof(_effects));

  _numLeds = MAX_LEDS;
  fill_solid(_leds, MAX_LEDS, CRGB::Black);
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

void EffectEngine::init(){
  //init LEDs
  FastLED.addLeds<WS2801, LED_PIN, LED_CLOCK, RGB>(_leds, MAX_LEDS).setCorrection( TypicalLEDStrip );
  showStrip();
}

void EffectEngine::showStrip() {
  FastLED.show();
}

void EffectEngine::checkSetNumLeds(int numLeds){

  //Check if number of leds changed
  if(numLeds == _numLeds)
    return;
    
  //Check bounds
  if(numLeds < 0 || numLeds >= MAX_LEDS)
    return;

  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);
  
  //Init current effect if any and remember number of leds
  _numLeds = numLeds;

  //Reset effect
  checkSetEffect(_curEffect, true);
}


void EffectEngine::checkSetEffect( Effect *effect, bool force){
  
  //Nothing to change?
  if(effect == _curEffect && !force)
    return;

  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);
  
  //Change and init
  _curEffect = effect;

  if(_curEffect){
    _curEffect->init(_leds, _numLeds);
  }
}

void EffectEngine::loop(const struct EffectEngineCtx &ctx){ 
  //Adjust if necessary
  checkSetNumLeds(ctx.numLeds);
  checkSetEffect(( ctx.effectNum < 0 || ctx.effectNum >= _numEffects ) ? NULL : _effects[ctx.effectNum]);
  
  //if new effect is -1, i.e. none, then black the leds
  if(_curEffect != NULL){
    _curEffect->loop(ctx.speedDelay);
  }
 
  showStrip();
}
