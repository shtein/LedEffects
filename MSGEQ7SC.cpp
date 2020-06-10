#include "LedEffects.h"

#include "SoundCapture.h"
#include "MSGEQ7SC.h"


///////////////////////////////////////////
// SoundCaptureMSGEQ7

SoundCaptureMSGEQ7::SoundCaptureMSGEQ7(uint8_t pinAnalog, uint8_t pinStrobe, uint8_t pinReset){
  _pinAnalog = pinAnalog;
  _pinStrobe = pinStrobe;
  _pinReset  = pinReset;
}

SoundCaptureMSGEQ7::~SoundCaptureMSGEQ7(){
}

void SoundCaptureMSGEQ7::init(){
  //Setup pins
  pinMode(_pinAnalog, INPUT);
  pinMode(_pinStrobe, OUTPUT);
  pinMode(_pinReset, OUTPUT);

  reset();
}


void SoundCaptureMSGEQ7::reset(){  
  //Initialize digital pins
  digitalWrite(_pinReset, LOW);
  digitalWrite(_pinStrobe, HIGH); 
}

void SoundCaptureMSGEQ7::idle(){
  //Do nothing for now
}

void SoundCaptureMSGEQ7::getData(SoundCaptureData &data) const{
  //Reset
  digitalWrite(_pinReset, HIGH); 
  digitalWrite(_pinReset, LOW);
 
  for(int i = 0; i < MAX_BANDS; i++){
    //Prepare
    digitalWrite(_pinStrobe, HIGH);
    digitalWrite(_pinStrobe, LOW);

    //Allow output to settle
    delayMicroseconds(36);
    
    data.bands[i] = (uint16_t)(analogRead(_pinAnalog) >> 2);

    DBG_OUT("%u ", data.bands[i]);    
  }

  DBG_OUTLN("");
}

