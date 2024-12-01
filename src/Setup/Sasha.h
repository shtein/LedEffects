#if defined( __SASHA_SETUP )

////////////////////////////////////////
// WS2812b strip, 212 LEDs, 1 push button, remote control
#pragma message "Compile for Sasha"


//Effect Engine
BEGIN_EFFECT_ENGINE(Sasha, 0) 
  
  //Effects   
  BEGIN_EFFECTS()
    BEGIN_MODE("Effects", 15)
      ADD_EFFECT("Meteor rain", EffectMeteorRain<>)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Confetti", EffectConfetti)  
      ADD_EFFECT("Blur", EffectBlur)
      ADD_EFFECT("Rainbow move", EffectRainbowMove)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs) 
      ADD_EFFECT("Rainbow", EffectRainbow)
      ADD_EFFECT("Fade in out", EffectFadeInOut)
      ADD_EFFECT("Running lights", EffectRunningLights)           
      ADD_EFFECT("Color wipe", EffectColorWipe)               
      ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 
      ADD_EFFECT("Twinkle fox", EffectTwinkleFox) 
    END_MODE()
    BEGIN_MODE("Flags", 3)
      ADD_EFFECT("Twinke fox-Isrl",  EffectTwinkleFox, &IsraelFlag )
      ADD_EFFECT("Twinkle fox-urk", EffectTwinkleFox, &UkraineFlag )
      ADD_EFFECT("Twinkle fox-us", EffectTwinkleFox, &July4th)
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
      PUSH_BUTTON_TO_CMD(PB_CONTROL_PUSH_LONG, EEMC_MODE)
      PUSH_BUTTON_TO_CMD(PB_CONTROL_CLICK_SHORT, EEMC_EFFECT)      
    END_PUSH_BUTTON()     
  
  
    //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc      
    BEGIN_REMOTE(REMOTE_PIN)  //Remote
      RMT_BUTTON_TO_CMD(RKEY_OK, EEMC_MODE)                            //Mode
      RMT_BUTTON_PAIR_TO_CMD(RKEY_UP, EEMC_EFFECT, 0, CTF_VAL_NEXT)    //Effect
      RMT_BUTTON_PAIR_TO_CMD(RKEY_DONW, EEMC_EFFECT, 0, CTF_VAL_PREV)  //Effect
      RMT_BUTTON_PAIR_TO_CMD(RKEY_LEFT, EEMC_SPEED, 5)                 //Speed
      RMT_BUTTON_PAIR_TO_CMD(RKEY_RIGHT, EEMC_SPEED, -5)               //Speed
      RMT_BUTTON_PAIR_TO_CMD(RKEY_1, EEMC_COLOR_HUE, 5)                //Color Hue
      RMT_BUTTON_PAIR_TO_CMD(RKEY_4, EEMC_COLOR_HUE, -5)               //Color Hue
      RMT_BUTTON_PAIR_TO_CMD(RKEY_2, EEMC_COLOR_SAT , 5)               //Color Saturation
      RMT_BUTTON_PAIR_TO_CMD(RKEY_5, EEMC_COLOR_SAT ,-5)               //Color Saturation
      RMT_BUTTON_PAIR_TO_CMD(RKEY_3, EEMC_COLOR_VAL, 5)                //Color Value
      RMT_BUTTON_PAIR_TO_CMD(RKEY_6, EEMC_COLOR_VAL, -5)               //Color Value
    END_REMOTE()    
   
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__SASHA_SETUP
