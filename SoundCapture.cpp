#include "LedEffects.h"
//#include "Effect.h"
#include "SoundCapture.h"

////////////////////////////
//SoundCapture

SoundCapture *SoundCapture::_instance = NULL;

SoundCapture::SoundCapture(){
}


SoundCapture::~SoundCapture(){
}


void SoundCapture::initInstance(SoundCapture *instance){
  //Remember instance
  _instance = instance;

  //Init instance
  if(_instance){
     _instance->init();
  }
}

SoundCapture *SoundCapture::getInstance(){
  return _instance;
}

