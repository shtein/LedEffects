#include "precomp.h"
#include "LedEffects.h"
#include "EffectEngine.h"


void setup()
{
  Serial.begin(57600);  

  //Effects
  EffectMoodBlobs           eMoodBlobs;
  EffectRainbow             eRainbow;
  EffectFadeInOut           eFadeInOut;
  EffectRunningLights       eRunningLights;
  EffectColorWipe           eColorWipe;
  EffectTheaterChaseRainbow eTheaterChaseRainbow;


  //Effect Engine
  EffectEngine ee;

  //Add effects
  ee.addEffect(&eMoodBlobs);
  ee.addEffect(&eTheaterChaseRainbow);
  ee.addEffect(&eFadeInOut);
  ee.addEffect(&eRainbow);
  ee.addEffect(&eColorWipe);
  ee.addEffect(&eRunningLights);

  //Init
  ee.init(50, EEM_STATIC); //88 for Igor

  //Analog inputs
  PushButton btnMode(2);        //Mode
  PushButton btnEffect(3);      //Effect
  Potentiometer ptmHue(0);  //Hue
  //Potentiometer ptmSpeed(0);  //Speed
  //Potentiometer ptmLeds(0);     //Number of LEDs
  

  //Controls
  EffectControlPb ecMode(EEMC_MODE, &btnMode);
  EffectControlPb ecEffect(EEMC_EFFECT, &btnEffect);
  EffectControlPtmtr ecHue(EEMC_COLOR_VAL, &ptmHue);
  //EffectControlPtmtr ecSpeed(EEMC_SPEED, &ptmSpeed);
  //EffectControlPtmtr ecLeds(EEMC_NUMLEDS, &ptmLeds);

  //Control panel
  EffectControlPanel cp;
  cp.addControl(&ecMode);
  cp.addControl(&ecEffect);
  cp.addControl(&ecHue);
  //cp.addControl(&ecSpeed);
  //cp.addControl(&ecLeds);
  

  CtrlQueueItem itm;
  
  for( ;; ){      
     cp.loop(itm);
     ee.loop(itm); 
  }
}



void loop(){
}


