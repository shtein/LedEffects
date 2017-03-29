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

  //Effect engine context
  //Define only number of leds 
  EffectEngineCtx ctx; 
  ctx.numLeds    = 50;

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
  ee.init(ctx);

  //Effect Control
  EffectControls ec;
  ec.init(ctx);
  
  for( ;; ){
      
      ee.loop(ctx);   
      ec.loop(ctx); 
  }
}



void loop(){
}


