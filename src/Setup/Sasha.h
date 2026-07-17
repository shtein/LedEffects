#if defined( __SASHA_SETUP )

////////////////////////////////////////
// WS2812b strip, 212 LEDs, 1 push button, remote control
#pragma message "Compile for Sasha"


//Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Effects")
    ADD_EFFECT(el_StaticColor, CHSV(HUE_BLUE, 0xFF, 0xFF))
    ADD_EFFECT(el_MeteorRain)
    ADD_EFFECT(el_Ripple)   
    ADD_EFFECT(el_Plasma, tl_Plasma)
    ADD_EFFECT(el_Confetti, tl_Confetti)
    ADD_EFFECT(el_Blur)
    ADD_EFFECT(el_TheaterChaseRainbow)
    ADD_EFFECT(el_Noise, tl_Default)            
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_FadeInOut)
    ADD_EFFECT(el_RunningLights)           
    ADD_EFFECT(el_Juggle)
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT(el_PacificOcean)
  END_MODE()
END_EFFECTS()


BEGIN_EFFECT_ENGINE() 
  
  //Leds
  BEGIN_LEDS()
    ADD_STRIP(NEOPIXEL, LED_PIN)
  END_LEDS()

//Control    
  BEGIN_CONTROL_MAP()
    
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(PB_CONTROL_PUSH_LONG, EEMC_STATE)
      PUSH_BUTTON_TO_CMD(PB_CONTROL_CLICK_SHORT, EEMC_EFFECT)      
    END_PUSH_BUTTON()     
  
    
   
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__SASHA_SETUP
