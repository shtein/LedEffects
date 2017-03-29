#include "precomp.h"
#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include "pins.h"

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

void EffectEngine::init(struct EffectEngineCtx &ctx) {
  //Init context
  ctx.maxEffects = _numEffects;
  ctx.effectNum  = 0;
  ctx.speedDelay = SPEED_DELAY_MAX;

  
  //init LEDs
  FastLED.addLeds<NEOPIXEL, LED_PIN>(_leds, MAX_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.addLeds<WS2801, LED_PIN, LED_CLOCK, RGB>(_leds, MAX_LEDS).setCorrection( TypicalLEDStrip );
  
  showStrip();
}

void EffectEngine::showStrip() {
  FastLED.show();
  //delay(1);
}

void EffectEngine::onEffectChange(struct EffectEngineCtx &ctx){
  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

  //Change effect
  _curEffect = (ctx.effectNum < 0 || ctx.effectNum >= _numEffects ) ? NULL : _effects[ctx.effectNum];
}


void EffectEngine::onNumLedsChange(struct EffectEngineCtx &ctx){
  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

  //Remember number of leds
  _numLeds = (ctx.numLeds < 0 || ctx.numLeds >= MAX_LEDS) ? _numLeds : ctx.numLeds;
}

void EffectEngine::onModeChange(struct EffectEngineCtx &ctx){
  //Black the lights
  fill_solid(_leds, MAX_LEDS, CRGB::Black);
  
  switch(ctx.mode){
    case EEM_OFF: //Off
      _curEffect = NULL;
      Serial.print("mode off\n");
    break;
    case EEM_STATIC: //Static light
      _curEffect = &_eStatic;
      Serial.print("mode static\n");
     break;
     case EEM_EFFECT: case EEM_RANDOM:
      onEffectChange(ctx);
      Serial.print("mode effect\n");
     break;
  }
}

void EffectEngine::loop(struct EffectEngineCtx &ctx){

  //Check if mode changed
  if(ctx.cf & EEMC_MODE){
    onModeChange(ctx);
  }
    
  //Check if effect is different
  if(ctx.mode == EEM_EFFECT && ctx.cf & EEMC_EFFECT ){
    onEffectChange(ctx);
  }

  //Number of leds changed
  if(ctx.mode != EEM_OFF && ctx.cf & EEMC_NUMLEDS){
    onNumLedsChange(ctx);
  }

  //process with current effect
  bool updateLeds = ctx.cf != EEMC_NONE;
  
  if(_curEffect != NULL){
    
    //Check if we just changed the effect
    if(ctx.cf & (EEMC_EFFECT | EEMC_NUMLEDS | EEMC_MODE) ){
      //Init
      _curEffect->init(_leds, _numLeds);        
      //Save speed
      ctx.speedDelay = _curEffect->getSpeedDelay();
      //Save color
      ctx.hsv      = _curEffect->getHSV();
    }
    
    //Check if speed has changed
    if(ctx.cf & EEMC_SPEED){
      _curEffect->setSpeedDelay(ctx.speedDelay);
    }

    //Check if color has changed
    if(ctx.cf & EEMC_COLOR){
      _curEffect->setHSV(ctx.hsv);
    }

    _curEffect->loop();

    updateLeds |= _curEffect->proceeded();
  }

  //Stupid optimization as a workaroud for IR Remote conflicting with ws2811, ws2812 and ws2812b
  if(updateLeds){
    showStrip();    
  }
   
}
