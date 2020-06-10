
//Define configuration
//#define __OUTAGE_LAMP_SETUP
//#define __SASHA_SETUP
//#define __DAVID_SETUP
//#define __IGOR_SETUP
//#define __DECK_SETUP
//#define __CHRISTMAS_TREE_SETUP
//#define __WINDOW_SETUP
//#define __VADIM_SETUP
//#define __TAHOE_SETUP


#include "LedEffects.h"

#include <FastLED.h>

#include <AnalogInput.h>
#include <Controls.h>

#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include "Effect.h"

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
#include "Effects/Sound.h"

#if defined(__OUTAGE_LAMP_SETUP)
////////////////////////////////////////
// WS2801 strip, 31 LEDs, 2 push buttons
#pragma message "Compile for Outage Lamp"

void setup(){  
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Outage Lamp");    

  //Effect Engine
  BEGIN_EFFECT_ENGINE() 
    
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
    BEGIN_LEDS(31)      
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
  BEGIN_EFFECT_ENGINE() 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectMeteorRain)
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
        ADD_STATIC_COLOR(CHSV(HUE_BLUE, 0xFF, 0xFF))        
        ADD_STATIC_COLOR(CHSV(HUE_PURPLE, 0xFF, 0xFF))
        ADD_STATIC_COLOR(CHSV(HUE_ORANGE, 0xFF, 0xFF))
      END_MODE()
      BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(212)
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

#elif defined( __DAVID_SETUP)
////////////////////////////////////////
// WS2811 strip, 50 LEDs, 1 push button
#pragma message "Compile for David"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- David");  
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE() 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectFire)
        ADD_EFFECT(EffectMeteorRain)
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
        ADD_EFFECT_PARAM( EffectConfetti, &TransformChristmas)
        ADD_EFFECT_PARAM( EffectPlasma, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectNoise, &TransformChristmas )
      END_MODE()
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectNoise, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformAutunm )
      END_MODE()
      BEGIN_MODE(Emergancy, 1)
        ADD_EFFECT(EffectEmergencyLights)         
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(50)
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
  BEGIN_EFFECT_ENGINE() 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectFire)
        ADD_EFFECT(EffectMeteorRain)
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
        ADD_STATIC_COLOR(CHSV(HUE_RED, 0xFF, 0xFF))
        ADD_STATIC_COLOR(CHSV(HUE_GREEN, 0xFF, 0xFF))
        ADD_STATIC_COLOR(CHSV(HUE_BLUE, 0xFF, 0xFF))
      END_MODE()
      BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(88) 
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
  BEGIN_EFFECT_ENGINE()    
    EFFECT_ENGION_FLAG(EFF_RANDOM_START_EFFECT)
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 8)        
        ADD_EFFECT(EffectRipple)
        ADD_EFFECT(EffectMeteorRain)   
        ADD_EFFECT(EffectPlasma)        
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
      END_MODE()
      /*
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectNoise, &TransformHalloween)
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectNoise, &TransformChristmas )
      END_MODE()
      */
      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(267)
      ADD_STRIP(NEOPIXEL, LED_PIN)
      ADD_STRIP(NEOPIXEL, LED_PIN2)   //Second strip for deck
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
  BEGIN_EFFECT_ENGINE() 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Christmas, 3)   
        ADD_EFFECT_PARAM( EffectConfetti, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectNoise, &TransformChristmas )
      END_MODE() 
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectNoise, &TransformHalloween )
      END_MODE()
      BEGIN_MODE(Other, 15)        
        ADD_EFFECT(EffectRipple)   
        ADD_EFFECT(EffectMeteorRain)
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
    BEGIN_LEDS(150) 
      ADD_STRIP(WS2811, LED_PIN, RGB)  //Christmas Tree or David
      ADD_STRIP(WS2811, LED_PIN2, RGB) //Second strip for Christmass Tree
      ADD_STRIP(WS2811, LED_PIN3, RGB) //Third strip for Christmass Tree
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
     END_PUSH_BUTTON()
     POT_TO_CMD(EEMC_NUMLEDS, NUMLEDS_PIN)
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
  BEGIN_EFFECT_ENGINE() 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Themes, 15)        
        ADD_EFFECT_PARAM( EffectConfetti, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectNoise, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectConfetti, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectNoise, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectConfetti, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectNoise, &TransformChristmas )
      END_MODE()      
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(EffectRipple)
        ADD_EFFECT(EffectFire)
        ADD_EFFECT(EffectMeteorRain)
        ADD_EFFECT(EffectPlasma)
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)        
      END_MODE()
      BEGIN_MODE(OldEffects, 15)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)           //Single color
        ADD_EFFECT(EffectColorWipe)               //Not intersting
        ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(100) 
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
  BEGIN_EFFECT_ENGINE() 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectNoise, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformAutunm )
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformChristmas)
        ADD_EFFECT_PARAM( EffectPlasma, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectNoise, &TransformChristmas )
      END_MODE()
      
      BEGIN_MODE(Other, 15)        
        ADD_EFFECT(EffectRipple)   
        ADD_EFFECT(EffectMeteorRain)
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
    BEGIN_LEDS(100) 
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
// WS2811 string, 12v 300 LEDs, 1 push button - change effect only
#pragma message "Compile Tahoe"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Tahoe");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE() 
    EFFECT_ENGION_FLAG(EFF_RANDOM_START_EFFECT)
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 7)        
        ADD_EFFECT(EffectRipple)
        ADD_EFFECT(EffectMeteorRain)   
        ADD_EFFECT(EffectPlasma)        
        ADD_EFFECT(EffectConfetti)
        ADD_EFFECT(EffectBlur)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
      END_MODE()
      
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformAutunm )
        ADD_EFFECT_PARAM( EffectNoise, &TransformHalloween )
        ADD_EFFECT_PARAM( EffectPlasma, &TransformAutunm )
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT_PARAM( EffectConfetti, &TransformChristmas)
        ADD_EFFECT_PARAM( EffectPlasma, &TransformChristmas )
        ADD_EFFECT_PARAM( EffectNoise, &TransformChristmas )
      END_MODE()
      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(406)
      ADD_STRIP(WS2811, LED_PIN2)
    END_LEDS()

  //Control    
    BEGIN_CONTROL_MAP()
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON()

      PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN3)
      PUSH_BUTTON_SA_TO_CMD(EEMC_EFFECT, EFFECT_PIN)
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

// __TAHOE_SETUP

#else 
////////////////////////////////////////
// Everything else
#include <SoundCapture.h>

#pragma message "Compile for default"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - default");  

  INIT_SOUND_CAPTURE(SoundCaptureMSGEQ7, MSGEQ7_ANALOG_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_RESET_PIN)    
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE() 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Sound, 3)            
        ADD_EFFECT(EffectSoundSimple)
      END_MODE()
      BEGIN_MODE(Effects, 15)
        //ADD_EFFECT(EffectFire)
        //ADD_EFFECT(EffectMeteorRain)
        //ADD_EFFECT(EffectPlasma)
        //ADD_EFFECT(EffectConfetti)
        //ADD_EFFECT(EffectPaletteTransformFast) 
        //ADD_EFFECT(EffectBlur)
        //ADD_EFFECT(EffectRainbowMove)
        //ADD_EFFECT(EffectNoise)            
        //ADD_EFFECT(EffectMoodBlobs) 
        //ADD_EFFECT(EffectRainbow)
        //ADD_EFFECT(EffectFadeInOut)
        //ADD_EFFECT(EffectRunningLights)           //Single color
        //ADD_EFFECT(EffectColorWipe)               //Not intersting
        //ADD_EFFECT(EffectTheaterChaseRainbow) 
      END_MODE()
      //BEGIN_MODE(Static, 3)
        //ADD_STATIC_COLOR(CHSV(HUE_RED, 0xFF, 0xFF))
        //ADD_STATIC_COLOR(CHSV(HUE_GREEN, 0xFF, 0xFF))
        //ADD_STATIC_COLOR(CHSV(HUE_BLUE, 0xFF, 0xFF))
      //END_MODE()
      //BLACK_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS(33) //31 for EK outage lamp, 50 for test or David, 267 for deck, 88 for Igor, 212 for Sasha, 300 for Christmass Tree (150 for new Christmass Tree), 100 for old 2801 leds
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 
     
    
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
