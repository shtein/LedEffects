#include "precomp.h"
#include "effect.h"

Effect::Effect(){
  _numLeds    = 0;
  _leds       = NULL;
  _millis     = 0;
  _speedDelay = 10;
}

Effect::~Effect(){
}

void Effect::init(CRGB *leds, int numLeds){
  _leds    = leds;
  _numLeds = numLeds;

  setAll(CRGB::Black);
  nextProceedIn(0);

  reset();
}


void Effect::loop(){
  //Check timer
  if(!timeToProceed())
    return;
    
  proceed(_speedDelay);

  //if next time to proceed is not set then set it by default
  if(timeToProceed())
    nextProceedIn(_speedDelay);
}

CRGB Effect::getColor() const{
  return _color;
}

void Effect::setColor(const CRGB &color){
  _color = color;
}

void Effect::nextProceedIn(int delta){
  _millis = millis() + delta;
}

bool Effect::timeToProceed() const{ 
  return _millis <= millis() ? true: false;
}
    
void Effect::setPixel(int led, byte red, byte green, byte blue) {
   _leds[led].r = red;
   _leds[led].g = green;
   _leds[led].b = blue;
}

void Effect::setAll( byte red, byte green, byte blue) {
  CRGB color(red, green, blue);
  fill_solid(_leds, _numLeds, color);
}

void Effect::setPixel(int led, const CRGB &color) {
   _leds[led] = color;
}

void Effect::setAll(const CRGB &color) {
    fill_solid(_leds, _numLeds, color);
}


void Effect::setRandomColor(){
  CRGB color(0xFF00FF);
  color.setHue(random(256));
   
  _color = color;
}

void Effect::setSpeedDelay(int speedDelay){
  _speedDelay = speedDelay;
}

int Effect::getSpeedDelay() const{
  return _speedDelay;
}
