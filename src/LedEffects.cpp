//For Platformio specific setup is defined in platformio.ini using build_flags -DXXX option
//Far arduino IDE it needs to be #define XXX explictely in ledeffects.h



#include "LedEffects.h"

#include <FastLED.h>

#include <AnalogInput.h>
#include <Controls.h>
#include <CtrlSerial.h>
#include <SoundCapture.h>

#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include "Effect.h"
#include "EffectSound.h"

#include "Matrix.h"

#include "Macro.h"
#include "Pins.h"

//Effects
#include "Effects/Rainbow.h"
#include "Effects/FadeInOut.h"
#include "Effects/RunningLights.h"
#include "Effects/ColorWipe.h"
#include "Effects/MoodBlobs.h"
#include "Effects/Static.h"
#include "Effects/Noise.h"
#include "Effects/Blur.h"
#include "Effects/EmergencyLights.h"
#include "Effects/Fire.h"
#include "Effects/Meteor.h"  
#include "Effects/Ripple.h"
#include "Effects/Juggle.h"
#include "Effects/TwinkleFox.h"
#include "Effects/PacificOcean.h"
#include "Effects/MatrixAnimation.h"
#include "Effects/Sound.h"

#if defined(__OUTAGE_LAMP_SETUP)
////////////////////////////////////////
// WS2801 strip, 31 LEDs, 2 push buttons
#pragma message "Compile for Outage Lamp"

void setup(){  
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Outage Lamp");    

  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectFire)       
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectColorWipe)
      END_MODE()
      BEGIN_MODE(Outage, 1)
        ADD_EFFECT(EffectEmergencyLights)         
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()      
      ADD_STRIP(WS2801, LED_PIN, LED_CLOCK_PIN, RGB) //Old 2801 strip or EK outage lamp
    END_LEDS()

  //Control     
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_CLICK_SHORT, CTF_VAL_ABS, 0)      
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_PUSH_LONG) 
      END_PUSH_BUTTON() 
      PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN2, CTF_VAL_ABS, 1)
    END_CONTROL_MAP()       
    
  END_EFFECT_ENGINE() 
}

//__OUTAGE_LAMP_SETUP

#elif defined( __SASHA_SETUP )
////////////////////////////////////////
// WS2812b strip, 212 LEDs, 1 push button, remote control
#pragma message "Compile for Sasha"



void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Sasha");  
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)           
        ADD_EFFECT(EffectColorWipe)               
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
      BEGIN_MODE(Static, 3)
        ADD_STATIC_COLOR(HUE_BLUE)        
        ADD_STATIC_COLOR(HUE_PURPLE)
        ADD_STATIC_COLOR(HUE_ORANGE)
      END_MODE()
      BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()     
    
      //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc      
      BEGIN_REMOTE(REMOTE_PIN)  //Remote
        RMT_BUTTON_TO_CMD(EEMC_MODE, RKEY_OK)                         //Mode
        RMT_BUTTON_PAIR_TO_CMD(EEMC_EFFECT, RKEY_UP, RKEY_DOWN, 0)    //Effect
        RMT_BUTTON_PAIR_TO_CMD(EEMC_SPEED, RKEY_LEFT, RKEY_RIGHT, 5)  //Speed
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_HUE, RKEY_1, RKEY_4, 5)     //Color Hue
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_SAT, RKEY_2, RKEY_5, 5)     //Color Saturation
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_VAL, RKEY_3, RKEY_6, 5)     //Color Value
      END_REMOTE()    
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

//__SASHA_SETUP
#elif defined( __SASHA_TAHOE_SETUP )
////////////////////////////////////////
// WS2811 12v strip, 150 LEDs, 1 push button
#pragma message "Compile for Sasha at Tahoe"


void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Sasha's at Tahoe");  
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectRipple<>)   
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectTheaterChaseRainbow)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)           
        ADD_EFFECT(EffectJuggle)
        ADD_EFFECT(EffectTwinkleFox, TwinkleFox)
        ADD_EFFECT(EffectPacificOcean)

      END_MODE()
      BEGIN_MODE(Static, 3)
        ADD_STATIC_COLOR(HUE_BLUE)        
        ADD_STATIC_COLOR(HUE_PURPLE)
        ADD_STATIC_COLOR(HUE_ORANGE)
      END_MODE()
      BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()
      ADD_STRIP(WS2811, LED_PIN, BRG)
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()     
    
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

//__SASHA_SETUP



#elif defined(__UDALOV_SETUP)
////////////////////////////////////////
// WS2811 string, 200 LEDs, 1 push buttone, 1 Potentiometer
#pragma message "Compile for Udalov"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - Udalov");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Christmas, 4)   
        ADD_EFFECT( EffectConfetti, &TransformChristmas )
        ADD_EFFECT( EffectPlasma, &TransformChristmas )
        ADD_EFFECT( EffectNoise, &TransformChristmas )
        ADD_EFFECT(EffectTwinkleFox, &SnowAndIce)
      END_MODE() 
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT( EffectConfetti, &TransformHalloween )
        ADD_EFFECT( EffectPlasma, &TransformAutunm )
        ADD_EFFECT( EffectNoise, &TransformHalloween )
      END_MODE()
      BEGIN_MODE(Other, 10)        
        ADD_EFFECT(EffectRipple<>)   
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)        
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectTwinkleFox)
      END_MODE()    
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(WS2811, LED_PIN, RGB)  
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
     END_PUSH_BUTTON()
     POT_TO_CMD(EEMC_NUMLEDS, NUMLEDS_PIN, POT_NOISE_THRESHOLD, 100, 900)
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}


// __UDALOV

#elif defined( __DAVID_SETUP)
////////////////////////////////////////
// WS2811 strip, 50 LEDs, 1 push button
#pragma message "Compile for David"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- David");  
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectFire)
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)  
        ADD_EFFECT(EffectColorWipe)           
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT( EffectConfetti, &TransformChristmas)
        ADD_EFFECT( EffectPlasma, &TransformChristmas )
        ADD_EFFECT( EffectNoise, &TransformChristmas )
      END_MODE()
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT( EffectConfetti, &TransformAutunm )
        ADD_EFFECT( EffectNoise, &TransformHalloween )
        ADD_EFFECT( EffectPlasma, &TransformAutunm )
      END_MODE()
      BEGIN_MODE(Emergancy, 1)
        ADD_EFFECT(EffectEmergencyLights)         
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()
      ADD_STRIP(WS2811, LED_PIN, RGB)
    END_LEDS()

  //Control     
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

//__DAVID_SETUP

#elif defined( __IGOR_SETUP)
////////////////////////////////////////
// WS2812B strip, 88 LEDs, remote control
#pragma message "Compile for Igor"

void setup() {
  
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Igor");  
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectFire)
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)           //Single color
        ADD_EFFECT(EffectColorWipe)               //Not intersting
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
      BEGIN_MODE(Static, 3)
        ADD_STATIC_COLOR(HUE_RED)
        ADD_STATIC_COLOR(HUE_GREEN)
        ADD_STATIC_COLOR(HUE_BLUE)
      END_MODE()
      BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc      
      BEGIN_REMOTE(REMOTE_PIN)  //Remote
        RMT_BUTTON_TO_CMD(EEMC_MODE, RKEY_OK)                         //Mode
        RMT_BUTTON_PAIR_TO_CMD(EEMC_EFFECT, RKEY_UP, RKEY_DOWN, 0)    //Effect
        RMT_BUTTON_PAIR_TO_CMD(EEMC_SPEED, RKEY_LEFT, RKEY_RIGHT, 5)  //Speed
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_HUE, RKEY_1, RKEY_4, 5)     //Color Hue
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_SAT, RKEY_2, RKEY_5, 5)     //Color Saturation
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_VAL, RKEY_3, RKEY_6, 5)     //Color Value
      END_REMOTE()          
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

//__IGOR_SETUP

#elif defined(__DECK_SETUP)
////////////////////////////////////////
// WS2812B strip, 267 LEDs, 1 push button - change effect only
#pragma message "Compile for deck"

void setup() {


  DBG_INIT();
  DBG_OUTLN("Led effect started -- deck");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(EFF_RANDOM_START_EFFECT)    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 8)        
        ADD_EFFECT(EffectRipple<>)
        ADD_EFFECT(EffectMeteorRain<>)   
        ADD_EFFECT(EffectPlasma)        
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
      END_MODE()

      
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT( EffectConfetti, &TransformHalloween )
        ADD_EFFECT( EffectPlasma, &TransformAutunm )
        ADD_EFFECT( EffectNoise, &TransformHalloween)
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT( EffectConfetti, &TransformChristmas )
        ADD_EFFECT( EffectPlasma, &TransformChristmas )
        ADD_EFFECT( EffectNoise, &TransformChristmas )
      END_MODE()

      BEGIN_MODE(Ukraine, 3)      
        ADD_EFFECT(EffectConfetti, &UkraineFlag )
        ADD_EFFECT(EffectNoise, &UkraineFlag )
        ADD_EFFECT(EffectTwinkleFox, &UkraineFlag )
      END_MODE()  

      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()
      ADD_STRIP(NEOPIXEL, LED_PIN)
      ADD_STRIP(NEOPIXEL, LED_PIN2)
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()
    END_CONTROL_MAP()

  END_EFFECT_ENGINE() 
}

//__DECK_SETUP

#elif defined(__CHRISTMAS_TREE_SETUP)
////////////////////////////////////////
// WS2811 string, 150 LEDs, 1 push button, 1 potentiometer, 1 rotary encoder
#pragma message "Compile for christmas tree"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - christmas tree");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Christmas, 4)   
        ADD_EFFECT( EffectConfetti, &TransformChristmas )
        ADD_EFFECT( EffectPlasma, &TransformChristmas )
        ADD_EFFECT( EffectNoise, &TransformChristmas )
        ADD_EFFECT(EffectTwinkleFox, &SnowAndIce)
      END_MODE() 
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT( EffectConfetti, &TransformHalloween )
        ADD_EFFECT( EffectPlasma, &TransformAutunm )
        ADD_EFFECT( EffectNoise, &TransformHalloween )
      END_MODE()
      BEGIN_MODE(Other, 10)        
        ADD_EFFECT(EffectRipple<>)   
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)        
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()    
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(WS2811, LED_PIN, RGB)  //First strip for Christmass Tree
      ADD_STRIP(WS2811, LED_PIN2, RGB) //Second strip for Christmass Tree
      ADD_STRIP(WS2811, LED_PIN3, RGB) //Third strip for Christmass Tree
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
     END_PUSH_BUTTON()
     POT_TO_CMD(EEMC_NUMLEDS, NUMLEDS_PIN, POT_NOISE_THRESHOLD, 100, 900)
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}


// __CHRISTMAS_TREE_STUP

#elif defined(__WINDOW_SETUP)
////////////////////////////////////////
// WS28101 string, 100 LEDs, 1 push button
#pragma message "Compile for office window"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - window");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Themes, 15)        
        ADD_EFFECT( EffectConfetti, &TransformAutunm )
        ADD_EFFECT( EffectPlasma, &TransformAutunm )
        ADD_EFFECT( EffectNoise, &TransformAutunm )
        ADD_EFFECT( EffectConfetti, &TransformHalloween )
        ADD_EFFECT( EffectPlasma, &TransformHalloween )
        ADD_EFFECT( EffectNoise, &TransformHalloween )
        ADD_EFFECT( EffectConfetti, &TransformChristmas )
        ADD_EFFECT( EffectPlasma, &TransformChristmas )
        ADD_EFFECT( EffectNoise, &TransformChristmas )
      END_MODE()      
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectRipple<>)
        ADD_EFFECT(EffectFire)
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow) 
        ADD_EFFECT(EffectTwinkleFox)       
        ADD_EFFECT(EffectPacificOcean)       
      END_MODE()
      BEGIN_MODE(OldEffects, 15)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)           //Single color
        ADD_EFFECT(EffectColorWipe)               //Not intersting
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(WS2801, LED_PIN, LED_CLOCK_PIN, RGB) //Old 2801 strip
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}


//__WINDOW_SETUP

#elif defined(__VADIM_SETUP)
////////////////////////////////////////
// 2 WS2811 string, 100 LEDs, 1 push button
#pragma message "Compile for Vadim"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - Vadim");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT( EffectConfetti, &TransformAutunm )
        ADD_EFFECT( EffectNoise, &TransformHalloween )
        ADD_EFFECT( EffectPlasma, &TransformAutunm )
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT( EffectConfetti, &TransformChristmas)
        ADD_EFFECT( EffectPlasma, &TransformChristmas )
        ADD_EFFECT( EffectNoise, &TransformChristmas )
      END_MODE()
      
      BEGIN_MODE(Other, 15)        
        ADD_EFFECT(EffectRipple<>)   
        ADD_EFFECT(EffectMeteorRain<>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        //ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        //ADD_EFFECT(EffectFadeInOut)
        //ADD_EFFECT(EffectRunningLights)       
        //ADD_EFFECT(EffectColorWipe)       
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()    
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(WS2811, LED_PIN, RGB)  
      ADD_STRIP(WS2811, LED_PIN2, RGB) 
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
     END_PUSH_BUTTON()
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}


// __VADIM_SETUP

#elif defined(__TAHOE_SETUP)

////////////////////////////////////////
// WS2811 string, 12v 417 LEDs, remote with separate buttons for effect and mode change, one button for effect and mode change
#pragma message "Compile Tahoe"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Tahoe");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(EFF_RANDOM_START_EFFECT)
    //Effects   

    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 8)        
        ADD_EFFECT(EffectRipple<10>)
        ADD_EFFECT(EffectPlasma)        
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs)
        ADD_EFFECT(EffectJuggle)
        ADD_EFFECT(EffectTwinkleFox, TwinkleFox)
        ADD_EFFECT(EffectPacificOcean)
      END_MODE()

      BEGIN_MODE(Halloween, 2)      
        ADD_EFFECT(EffectConfetti, &TransformAutunm )
        ADD_EFFECT(EffectNoise, &TransformHalloween )
      END_MODE()

      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT(EffectConfetti, &TransformChristmas)
        ADD_EFFECT(EffectPlasma, &TransformChristmas )
        ADD_EFFECT(EffectTwinkleFox, &SnowAndIce)
      END_MODE()

      BEGIN_MODE(Valentines, 3)      
        ADD_EFFECT(EffectConfetti, &ValentinesDay )
        ADD_EFFECT(EffectNoise, &ValentinesDay )
        ADD_EFFECT(EffectTwinkleFox, &ValentinesDay)
      END_MODE()

      BEGIN_MODE(July4, 2)      
        ADD_EFFECT(EffectConfetti, &July4th )
        ADD_EFFECT(EffectNoise, &July4th )
      END_MODE()

      BEGIN_MODE(Ukraine, 2)      
        ADD_EFFECT(EffectConfetti, &UkraineFlag )
        ADD_EFFECT(EffectNoise, &UkraineFlag )
        ADD_EFFECT(EffectTwinkleFox, &UkraineFlag )
      END_MODE()

    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()
      ADD_STRIP(WS2811, LED_PIN2)
      ADD_STRIP(WS2811, LED_PIN3)
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()

      PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN3)
      PUSH_BUTTON_SA_TO_CMD(EEMC_EFFECT, EFFECT_PIN)      

      //SERIAL_INPUT()
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

#elif defined(__MEDIA_ROOM_SETUP)

///////////////////////////////////////////////////
// 16x16 Matrix, with MSGEQ07 
#include <SoundCapture.h>

#pragma message "Compile for media room"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - 16x16 sound matrix");  

  INIT_SOUND_CAPTURE(SoundCaptureMSGEQ7, MSGEQ7_ANALOG_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_RESET_PIN)    
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Sound, 2)
        ADD_EFFECT(EffectSound)
      END_MODE()
      BEGIN_MODE(Effects, 10)                        
        ADD_EFFECT(EffectJuggle)
        ADD_EFFECT(EffectTwinkleFox, TwinkleFox)        
        ADD_EFFECT(EffectPacificOcean)         
      END_MODE()      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 

      SW2POS_TO_CMD(EEMC_SOUND_LOG, 7)
      SW2POS_TO_CMD(EEMC_SOUND_USE_MAX, 6)
      SW2POS_TO_CMD(EEMC_SOUND_USE_MIN, 5)
      POT_TO_CMD(EEMC_SOUND_LOW, SOUND_LOW_PIN, POT_NOISE_THRESHOLD, 100)
      POT_TO_CMD(EEMC_SOUND_SENSITIVITY, SOUND_HIGH_PIN, POT_NOISE_THRESHOLD, 300)
     

    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

#elif defined(__SOUND_MATRIX_16x16)

///////////////////////////////////////////////////
// 16x16 Matrix, with MSGEQ07 
#include <SoundCapture.h>

#pragma message "Compile for 16x16 matrix with MSGEQ7 sound capture"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - 16x16 sound matrix");  

  INIT_SOUND_CAPTURE(SoundCaptureMSGEQ7, MSGEQ7_ANALOG_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_RESET_PIN)    
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Sound, 3)            
        ADD_EFFECT(EffectSoundRGB)
        ADD_EFFECT(EffectSoundMatrixSymmetric)
        ADD_EFFECT(EffectSoundMatrixColumn)
      END_MODE()
      BEGIN_MODE(Effects, 10)                
        ADD_EFFECT(EffectJuggle)
        ADD_EFFECT(EffectTwinkleFox, TwinkleFox)        
        ADD_EFFECT(EffectPacificOcean)         
      END_MODE()      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 

      SW2POS_TO_CMD(EEMC_SOUND_LOG, 7)
      SW2POS_TO_CMD(EEMC_SOUND_USE_MAX, 6)
      SW2POS_TO_CMD(EEMC_SOUND_USE_MIN, 5)
      POT_TO_CMD(EEMC_SOUND_LOW, SOUND_LOW_PIN, POT_NOISE_THRESHOLD, 100)
      POT_TO_CMD(EEMC_SOUND_SENSITIVITY, SOUND_HIGH_PIN, POT_NOISE_THRESHOLD, 300)
     

    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

// __SOUND_MATRIX_16x16
#elif defined(__FRAME_MATRIX_16x16)

#pragma message "Compile for 16x16 matrix in a frame"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - 16x16 matrix in frame");  

  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectMatrixBounsingDots)
        ADD_EFFECT(EffectMatrixKaleidoscope<EffectConfetti>)
        ADD_EFFECT(EffectMatrixKaleidoscope<EffectTwinkleFox>)
        ADD_EFFECT(EffectMatrixKaleidoscope<EffectMoodBlobs >)
        ADD_EFFECT(EffectMatrixKaleidoscope<EffectPlasma >)
        //ADD_EFFECT(EffectConfetti)
        //ADD_EFFECT(EffectMeteorRain<5>)
        //ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectTwinkleFox)
        //ADD_EFFECT(EffectJuggle)
        ADD_EFFECT(EffectPacificOcean)        
        //ADD_EFFECT(EffectBlur)
        //ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectMatrixDrops<8>, TwinkleFox)
        ADD_EFFECT(EffectMatrixCircles<2>)
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

//__FRAME_MATRIX_16x16
#else 
////////////////////////////////////////
// Everything else
//#include <SoundCapture.h>

#pragma message "Compile for default"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - default");  

  //INIT_SOUND_CAPTURE(SoundCaptureMSGEQ7, MSGEQ7_ANALOG_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_RESET_PIN)    
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    //Effects   
    BEGIN_EFFECTS()
      //BEGIN_MODE(Sound, 3)            
      //  ADD_EFFECT(EffectSoundMatrixSymmetric)
       // ADD_EFFECT(EffectSoundMatrixColumn)
      //END_MODE()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectMeteorRain<5>)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectTwinkleFox)
        ADD_EFFECT(EffectJuggle)
        ADD_EFFECT(EffectPacificOcean)        
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectMatrixDrops<8>, TwinkleFox)
        ADD_EFFECT(EffectMatrixCircles<1>)
        //ADD_EFFECT(EffectRipple<5>)   
        //ADD_EFFECT(EffectFadeInOut)
        //ADD_EFFECT(EffectRunningLights)           //Single color
        //ADD_EFFECT(EffectColorWipe)               //Not intersting
        //ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
      BEGIN_MODE(Static, 3)
        ADD_STATIC_COLOR(HUE_RED)
      END_MODE()
      //BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 

      SERIAL_INPUT()

      //SW2POS_TO_CMD(EEMC_SOUND_LOG, 7)
      //SW2POS_TO_CMD(EEMC_SOUND_USE_MAX, 6)
      //SW2POS_TO_CMD(EEMC_SOUND_USE_MIN, 5)
      //POT_TO_CMD(EEMC_SOUND_LOW, SOUND_LOW_PIN, POT_NOISE_THRESHOLD, 100)
      //POT_TO_CMD(EEMC_SOUND_SENSITIVITY, SOUND_HIGH_PIN, POT_NOISE_THRESHOLD, 300)
     
    
      //PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN)            //Mode
      //PUSH_BUTTON_SA_TO_CMD(EEMC_EFFECT, EFFECT_PIN)        //Effect
      //POT_TO_CMD(EEMC_NUMLEDS, NUMLEDS_PIN)                 //Number of leds
      //ROTENC_TO_CMD(EEMC_SPEED, SPEED_PIN, SPEED_CLOCK_PIN) //Speed
      
      //POT_TO_CMD(EEMC_MIC_SENS, MIC_SENS_PIN)               //Micrphone sensitivity
      //MIC_TO_CMD(EEMC_MIC, MIC_PIN)                         //Micrphone samples


      //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc
      /*
      BEGIN_REMOTE(REMOTE_PIN)  //Remote
        RMT_BUTTON_TO_CMD(EEMC_MODE, RKEY_OK)                         //Mode
        RMT_BUTTON_PAIR_TO_CMD(EEMC_EFFECT, RKEY_UP, RKEY_DOWN, 0)    //Effect
        RMT_BUTTON_PAIR_TO_CMD(EEMC_SPEED, RKEY_LEFT, RKEY_RIGHT, 5)  //Speed
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_HUE, RKEY_1, RKEY_4, 5)     //Color Hue
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_SAT, RKEY_2, RKEY_5, 5)     //Color Saturation
        RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_VAL, RKEY_3, RKEY_6, 5)     //Color Value
      END_REMOTE()    
      */
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

#endif

void loop(){
}
