#if defined( __SASHA_SETUP )

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
      BEGIN_MODE("Effects", 15)
        ADD_EFFECT("Meteor rain", EffectMeteorRain<>)
        ADD_EFFECT("Plazma", EffectPlasma)
        ADD_EFFECT("Confetti", EffectConfetti)
        ADD_EFFECT("Transform fast", EffectPaletteTransformFast) 
        ADD_EFFECT("Blur", EffectBlur)
        ADD_EFFECT("Rainbow move", EffectRainbowMove)
        ADD_EFFECT("Noise", EffectNoise)            
        ADD_EFFECT("Mood blobs", EffectMoodBlobs) 
        ADD_EFFECT("Rainbow", EffectRainbow)
        ADD_EFFECT("Fade in out", EffectFadeInOut)
        ADD_EFFECT("Running lights", EffectRunningLights)           
        ADD_EFFECT("Color wipe", EffectColorWipe)               
        ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 
      END_MODE()
      BEGIN_MODE("Static", 3)
        ADD_STATIC_COLOR("Static blue", HUE_BLUE)        
        ADD_STATIC_COLOR("Static purple" ,HUE_PURPLE)
        ADD_STATIC_COLOR("Static orange", HUE_ORANGE)
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

#endif //__SASHA_SETUP
