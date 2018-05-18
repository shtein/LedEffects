#include "precomp.h"
#include "LedEffects.h"
#include "EffectEngine.h"
#include "Controls.h"
#include "Macro.h"
#include "Pins.h"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started");  

  //Effect Engine
  BEGIN_EFFECT_ENGINE() 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        //ADD_EFFECT(EffectFire)
        //ADD_EFFECT(EffectEmergencyLights) //For David
        ADD_EFFECT(EffectConfetti)
        //ADD_EFFECT(EffectMatrix) 
        ADD_EFFECT(EffectPaletteTransformFast) 
        ADD_EFFECT(EffectBlur)
        //ADD_EFFECT(EffectBeatWave)              //Ugly or incomplete
        ADD_EFFECT(EffectRainbowMove)
        ADD_EFFECT(EffectNoise)            
        ADD_EFFECT(EffectMoodBlobs) 
        ADD_EFFECT(EffectRainbow)
        ADD_EFFECT(EffectFadeInOut)
        ADD_EFFECT(EffectRunningLights)        //Single color
        ADD_EFFECT(EffectColorWipe)            //Not intersting
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
    BEGIN_LEDS(100) //50 for test or David, 267 for deck, 88 for Igor, 212 for Sasha, 300 for Christmass Tree (150 for new Christmass Tree), 100 for old 2801 leds
      //ADD_STRIP(NEOPIXEL, LED_PIN)
      //ADD_STRIP(NEOPIXEL, LED_PIN2)   //Second strip for deck
      
      //ADD_STRIP(WS2811, LED_PIN, RGB)  //Christmas Tree or David
      //ADD_STRIP(WS2811, LED_PIN2, RGB) //Second strip for Christmass Tree
      //ADD_STRIP(WS2811, LED_PIN3, RGB) //Third strip for Christmass Tree
      
      ADD_STRIP(WS2801, LED_PIN, LED_CLOCK_PIN, RGB) //Old 2801 strip
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


void loop(){
}



