#include "precomp.h"
#include "LedEffects.h"
#include "EffectEngine.h"


void setup()
{
  Serial.begin(57600);  

  //Effects
  EffectRainbow             eRainbow;
  EffectFadeInOut           eFadeInOut;
  EffectRunningLights       eRunningLights;
  EffectColorWipe           eColorWipe;
  EffectTheaterChaseRainbow eTheaterChaseRainbow;


  //Effect Engine
  EffectEngine ee;


  //Add effects
  ee.addEffect(&eTheaterChaseRainbow);
  ee.addEffect(&eFadeInOut);
  ee.addEffect(&eRainbow);
  ee.addEffect(&eColorWipe);
  ee.addEffect(&eRunningLights);

  //Init
  ee.init();

  //Effect Control
  
  EffectEngineCtx ctx;
  ctx.speedDelay = 100;
  ctx.numLeds    = 50;
  ctx.effectNum  = 2;
  ctx.maxEffects = 5;

  EffectControls ec;
  ec.init(ctx);
  
  for( ;; ){
      
      ee.loop(ctx);   
      ec.loop(ctx); 
  }
}



void loop(){
}



/*
////////////////////////////////////
// TwinkleRandom

void twinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}


////////////////////////////////
// Bouncing Colored Balls

byte bbColors[3][3] = { {0xff, 0, 0}, 
                        {0xff, 0xff, 0xff}, 
                        {0   , 0   , 0xff} };


void bouncingColoredBalls() {
  int BallCount = sizeof( bbColors) / sizeof(bbColors[0]);
    
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  
  for (int i = 0 ; i < BallCount ; i++) {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
  
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],bbColors[i][0],bbColors[i][1],bbColors[i][2]);
    }
    
    setAll(0,0,0);
  }
}

*/
/*

// *** REPLACE TO HERE ***

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}


*/
