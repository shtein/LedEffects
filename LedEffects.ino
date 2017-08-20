#include "precomp.h"
#include "LedEffects.h"
#include "EffectEngine.h"
#include "EffectControls.h"
#include "pins.h"



void setup() {
  DBG_INIT();

  //Effect Engine
  BEGIN_EFFECT_ENGINE() 
    //Effects   
    BEGIN_EFFECTS()
      ADD_EFFECT(EffectNoise)
      ADD_EFFECT(EffectMoodBlobs)
      ADD_EFFECT(EffectRainbow)
      ADD_EFFECT(EffectFadeInOut)
      ADD_EFFECT(EffectRunningLights)
      ADD_EFFECT(EffectColorWipe)
      ADD_EFFECT(EffectTheaterChaseRainbow)     
    END_EFFECTS()

    //Leds
    BEGIN_LEDS(267, EEM_EFFECT) //267 for deck, 88 for Igor, 212 for Sasha, MAX_LEDS, i.e. 300, for Christmass Tree
      ADD_STRIP(NEOPIXEL, LED_PIN)
      ADD_STRIP(NEOPIXEL, LED_PIN2)
      //ADD_STRIP(WS2811, LED_PIN, RGB)
      //ADD_STRIP(WS2801, LED_PIN, LED_CLOCK, RGB)
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



void loop(){
}


