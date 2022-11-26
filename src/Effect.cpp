#include "LedEffects.h"

#include <Fastled.h>
#include <Controls.h>
#include <EEPROMCfg.h>

#include "effectenginectx.h"
#include "effect.h"

#include "Matrix.h"



/////////////////////////////////
// Effect

Effect::EffectContext Effect::_ctx;

Effect::Effect(){
}

Effect::~Effect(){
}

void Effect::init(CRGB *leds, uint16_t numLeds){  

  //Black all leds
  setAll(leds, numLeds, CRGB::Black);

  //reset data
  reset();
}


void Effect::loop(CRGB *leds, uint16_t numLeds){    
  proceed(leds, numLeds);
}

bool Effect::onCmd(struct CtrlQueueItemEx &itm){

  switch(itm.cmd){    
    case EEMC_SPEED:
      setSpeedDelay(itm.data.translate(getSpeedDelay(), SPEED_DELAY_MIN, SPEED_DELAY_MAX));
#ifdef NTF_ENABLED
    { itm.ntf.put_F(NULL, EECmdResponse<EEResp_EffectSpeed> {itm.cmd, {getSpeedDelay()}}); }
#endif      
    break;
#ifdef NTF_ENABLED
    case EEMC_GET_SPEED:
    { itm.ntf.put_F(NULL, EECmdResponse<EEResp_EffectSpeed> {itm.cmd, {getSpeedDelay()}}); }
    break;
#endif      
    default:
    return false;
  }    

  return true;
}

void Effect::setSpeedDelay(uint8_t speedDelay){  
  _ctx.speedDelay = speedDelay < SPEED_DELAY_MIN ? SPEED_DELAY_MIN : speedDelay > SPEED_DELAY_MAX ? SPEED_DELAY_MAX : speedDelay;
}

uint8_t Effect::getSpeedDelay() const{  
  return _ctx.speedDelay;
}

void Effect::setPixel(CRGB &led,  byte red, byte green, byte blue) {
   led.r = red;
   led.g = green;
   led.b = blue;
}

void Effect::setPixel(CRGB &led, const CRGB &color) {
   led = color;
}

void Effect::setAll(CRGB *leds, uint16_t numLeds, const CRGB &color) {
  fill_solid(leds, numLeds, color);
}

void Effect::setAll(CRGB *leds, uint16_t numLeds, byte red, byte green, byte blue) {  
  fill_solid(leds, numLeds, CRGB (red, green, blue));
}

bool Effect::config(EEPROMCfg &cfg, bool read){
  if(read){
    //Version, speed  
    uint8_t version;
    uint8_t speedDelay;

    cfg >> version >> speedDelay;

    //Check if version is correct 
    if(version != EE_VERSION)
      return false;

    //Set speed delay
    setSpeedDelay(speedDelay);
  }
  else{
    //Write version and speed
    cfg << (uint8_t)EE_VERSION << getSpeedDelay();
  }

  return true; 
}

//////////////////////////////////////
// EffectColor
EffectColor::EffectColor(){
}

EffectColor::~EffectColor(){  
}

const CHSV &EffectColor::getHSV() const{
  return _ctx.hsv;
}

void EffectColor::setHSV(const CHSV &hsv){
  _ctx.hsv   = hsv;
}

CRGB EffectColor::getColor() const{
  return _ctx.hsv;
}
    
void EffectColor::setRandomColor(){
  setHSV(CHSV(random(256), 0xFF, 0xFF));
}


bool EffectColor::onCmd(struct CtrlQueueItemEx &itm){  
  switch(itm.cmd){
    case EEMC_COLOR_HUE: 
    case EEMC_COLOR_SAT: 
    case EEMC_COLOR_VAL: {
      //Get effect color
      CHSV hsv = getHSV();

      //Update corresponding color value
      hsv.raw[itm.cmd - EEMC_COLOR_HUE] = (uint8_t)itm.data.translate( (int)hsv.raw[itm.cmd - EEMC_COLOR_HUE], 0, 255);
  
      //Set effect color
      setHSV(hsv);       

#ifdef NTF_ENABLED
      itm.ntf.put_F(NULL, EECmdResponse<EEResp_Color>{ itm.cmd, { { getHSV() } }});       
#endif
    }   
    break;
#ifdef NTF_ENABLED
    case EEMC_GET_COLOR_HSV:
      {itm.ntf.put_F(NULL, EECmdResponse<EEResp_Color>{ itm.cmd, { { getHSV() } }}); }
    break;
#endif
    default:
    return Effect::onCmd(itm);    
  }    
  
  return true;
}


bool EffectColor::config(EEPROMCfg &cfg, bool read){
  
  if(!Effect::config(cfg, read))
      return false;

  if(read){    
    //Read color
    CHSV hsv;
    cfg >> hsv;

    setHSV(hsv);
  }
  else{
  //Write color
    cfg << getHSV();
  }

  return true;
}



//////////////////////////////////////
// EffectPaletteTransform

void FuncGetPal_Default(CRGBPalette16 &pal){
   pal = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), 
                       CHSV(random8(), 255, random8(128,255)), 
                       CHSV(random8(), 192, random8(128,255)), 
                       CHSV(random8(), 255, random8(128,255))
                      );
}


EffectPaletteTransform::EffectPaletteTransform(FuncGetPalette_t getPal){  
  _getPal = getPal;
}

EffectPaletteTransform::~EffectPaletteTransform(){
  
}

void EffectPaletteTransform::reset(){
  //Init target pallete
  updateColors();

  //Safe target into current  
  _ctx.palCurrent = _ctx.palTarget;
                      
   //Update target palette again
   updateColors();

  //Reset step
  _ctx.step = getMaxStep();

  //Speed
  setSpeedDelay(25);  
}

int EffectPaletteTransform::getMaxStep() const{
  return CHANGE_PAL_STEP;
}


bool EffectPaletteTransform::isReadyToBlendPal() const{
  return true;
}

bool EffectPaletteTransform::isReadyToChangePal() const{
  return _ctx.step == 0;
}

void EffectPaletteTransform::updateColors(){
  //Change target palette
  if(_getPal)
    _getPal(_ctx.palTarget);
}

void EffectPaletteTransform::updateLeds(CRGB *leds, uint16_t numLeds){  
  //Default implementation
  for(uint16_t i = 0; i < numLeds; i++){     
    leds[i] = getCurrentPalColor(0);
  }
}

void EffectPaletteTransform::onStep(){
  
  //Check if it is to update target palette
  if(isReadyToChangePal()){
    //Update palette
    updateColors();

    //Reset step
    _ctx.step = getMaxStep(); 
  }
  
  //Proceed with palette transtion
  if(isReadyToBlendPal()){
    nblendPaletteTowardPalette(_ctx.palCurrent, _ctx.palTarget, MAX_PAL_CHANGES); 
  }

  //Prepare for the next move                                        
  _ctx.step--;
}

void EffectPaletteTransform::proceed(CRGB *leds, uint16_t numLeds){
  //Process step
  onStep();
  
  //Update Leds
  updateLeds(leds, numLeds); 
}

CRGB EffectPaletteTransform::getCurrentPalColor(uint8_t index, uint8_t brightness, TBlendType blendType) const{
  return ColorFromPalette(_ctx.palCurrent, index, brightness, blendType);
}
