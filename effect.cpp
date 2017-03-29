#include "precomp.h"
#include "effect.h"

Effect::Effect(){
  _numLeds    = 0;
  _leds       = NULL;
  _millis     = 0;
  _speedDelay = 10;
  _proceeded  = false;
}

Effect::~Effect(){
}

void Effect::init(CRGB *leds, int numLeds){
  _leds      = leds;
  _numLeds   = numLeds;
  _proceeded = false;

  setAll(CRGB::Black);

  
  nextProceedIn(0);

  reset();
}


void Effect::loop(){
  //Reset processing flag 
  _proceeded = false;
  
  //Check timer
  if(!timeToProceed())
    return;

  //proceed
  proceed(_speedDelay);
  
  //set processing flag
  _proceeded = true;

  //if next time to proceed is not set then set it by default
  if(timeToProceed())
    nextProceedIn(_speedDelay);
}

bool Effect::proceeded() const{
  return _proceeded;
}

CRGB Effect::getColor() const{
  return _color;
}

CHSV Effect::getHSV() const{
  return _hsv;
}

void Effect::setHSV(const CHSV &hsv){
  _color = hsv;
  _hsv   = hsv;
}

int Effect::getNumLeds() const{
  return _numLeds;
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
  CHSV hsv(random(256), 0xFF, 0xFF);
  setHSV(hsv);
}

void Effect::setSpeedDelay(int speedDelay){
  _speedDelay = speedDelay;

  Serial.print("New speed delay ");
  Serial.print(_speedDelay);
  Serial.print("\n");
}

int Effect::getSpeedDelay() const{
  return _speedDelay;
}
