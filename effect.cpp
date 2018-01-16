#include "precomp.h"
#include "effect.h"
#include "effectenginectx.h"

CRGBPalette16 Effect::_pals[MAX_PAL];


Effect::Effect(){
  _speedDelay = 25;
}

Effect::~Effect(){
}


void Effect::init(CRGB *leds, int numLeds){  

  setAll(leds, numLeds, CRGB::Black);
  
  reset();
}


void Effect::loop(CRGB *leds, int numLeds){  
  
  //proceed
  proceed(leds, numLeds);
 
}


CRGB Effect::getColor() const{
  return getHSV();
}
    
void Effect::setRandomColor(){
  setHSV(CHSV(random(256), 0xFF, 0xFF));
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


CRGBPalette16 &Effect::allocPalette(int index){
  return _pals[index];
}

CHSV Effect::getHSV() const{
  return _hsv;
}

void Effect::setHSV(const CHSV &hsv){
  _hsv   = hsv;
}


