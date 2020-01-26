#include "precomp.h"
#include "effectenginectx.h"
#include "effect.h"


/////////////////////////////////
// Effect

Effect::Effect(){
  _speedDelay = 25;
}

Effect::~Effect(){
}


void Effect::init(CRGB *leds, int numLeds){  

  //Black all leds
  setAll(leds, numLeds, CRGB::Black);

  //reset data
  reset();
}

void Effect::reset(){
}

void Effect::loop(CRGB *leds, int numLeds){  
  
  //proceed
  proceed(leds, numLeds);
 
}

void Effect::proceed(CRGB * /*leds*/, int /*numLeds */){
}


void Effect::onCmd(const struct CtrlQueueItem &itm){
  
  switch(itm.cmd){    
    case EEMC_SPEED:
      setSpeedDelay(itm.data.translate(getSpeedDelay(), SPEED_DELAY_MIN, SPEED_DELAY_MAX));
    break;
  }    
}

void Effect::idle(){
}


void Effect::setSpeedDelay(uint16_t speedDelay){
  //Scale down
  _speedDelay = (uint8_t)map(speedDelay, SPEED_DELAY_MIN, SPEED_DELAY_MAX, 0, 255);
}

uint16_t Effect::getSpeedDelay() const{
  //Scale Up
  return (uint16_t)map(_speedDelay, 0, 255, SPEED_DELAY_MIN, SPEED_DELAY_MAX);
}

void Effect::setPixel(CRGB &led,  byte red, byte green, byte blue) {
   led.r = red;
   led.g = green;
   led.b = blue;
}

void Effect::setPixel(CRGB &led, const CRGB &color) {
   led = color;
}

void Effect::setAll(CRGB *leds, int numLeds, const CRGB &color) {
  fill_solid(leds, numLeds, color);
}

void Effect::setAll(CRGB *leds, int numLeds, byte red, byte green, byte blue) {  
  fill_solid(leds, numLeds, CRGB (red, green, blue));
}



//////////////////////////////////////
// EffectColor
EffectColor::EffectColor(const CHSV &hsv){
  _hsv = hsv;
}

EffectColor::~EffectColor(){  
}

const CHSV &EffectColor::getHSV() const{
  return _hsv;
}

void EffectColor::setHSV(const CHSV &hsv){
  _hsv   = hsv;
}

CRGB EffectColor::getColor() const{
  return _hsv;
}
    
void EffectColor::setRandomColor(){
  setHSV(CHSV(random(256), 0xFF, 0xFF));
}


void EffectColor::onCmd(const struct CtrlQueueItem &itm){
  switch(itm.cmd){
    case EEMC_COLOR_HUE:case EEMC_COLOR_SAT: case EEMC_COLOR_VAL: {
      //Get effect color
      CHSV hsv = getHSV();

      //Update corresponding color value
      hsv.raw[itm.cmd - EEMC_COLOR_HUE] = (uint8_t)itm.data.translate( (int)hsv.raw[itm.cmd - EEMC_COLOR_HUE], 0, 255);
  
      //Set effect color
      setHSV(hsv);   
    }   
    break;
    default:
      Effect::onCmd(itm);
    break;
  }    
  
}


//////////////////////////////////////
// EffectPaletteTransform

CRGBPalette16 EffectPaletteTransform::_palCurrent;
CRGBPalette16 EffectPaletteTransform::_palTarget;
    
int EffectPaletteTransform::_step;


EffectPaletteTransform::EffectPaletteTransform() {
  setSpeedDelay(25);  
}

EffectPaletteTransform::~EffectPaletteTransform(){
  
}

void EffectPaletteTransform::reset(){
  //Init target pallete
  updateColors();

  //Safe target into current  
  _palCurrent = _palTarget;
                      
   //Update target palette again
   updateColors();

  //Reset step
  _step = getMaxStep();
}

int EffectPaletteTransform::getMaxStep() const{
  return CHANGE_PAL_STEP;
}


bool EffectPaletteTransform::isReadyToBlendPal() const{
  return true;
}

bool EffectPaletteTransform::isReadyToChangePal() const{
  return _step == 0;
}

void EffectPaletteTransform::updateColors(){
  //Change target palette
  _palTarget = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), 
                       CHSV(random8(), 255, random8(128,255)), 
                       CHSV(random8(), 192, random8(128,255)), 
                       CHSV(random8(), 255, random8(128,255))
                      );
}

void EffectPaletteTransform::updateLeds(CRGB *leds, int numLeds){  
  //Default implementation
  for(int i = 0; i < numLeds; i++){     
    leds[i] = ColorFromPalette(_palCurrent, 0, 255, LINEARBLEND);
  }
}

void EffectPaletteTransform::onStep(){
  
  //Check if it is to update target palette
  if(isReadyToChangePal()){
    //Update palette
    updateColors();

    //Reset step
    _step = getMaxStep();       
  }
  
  
  //Proceed with palette transtion
  if(isReadyToBlendPal()){
    nblendPaletteTowardPalette(_palCurrent, _palTarget, MAX_PAL_CHANGES); 
  }

  //Prepare for the next move                                        
  _step--;
}

void EffectPaletteTransform::proceed(CRGB *leds, int numLeds){
  //Process step
  onStep();
  
  //Update Leds
  updateLeds(leds, numLeds); 
}






