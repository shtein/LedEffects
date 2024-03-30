#if defined(__WINDOW_SETUP)

////////////////////////////////////////
// WS28101 string, 100 LEDs, 1 push button

#pragma message "Compile for office window"

//Effects   
BEGIN_EFFECTS(0)

  BEGIN_MODE("Themes")        
    ADD_EFFECT(el_Confetti, tl_Autumn)
    ADD_EFFECT(el_Plasma, tl_Autumn)
    ADD_EFFECT(el_Noise, tl_Autumn )
    ADD_EFFECT(el_Confetti, tl_Halloween )
    ADD_EFFECT(el_Plasma, tl_Halloween )
    ADD_EFFECT(el_Noise, tl_Halloween)
    ADD_EFFECT(el_Confetti, tl_Christmas)
    ADD_EFFECT(el_Plasma, tl_Christmas)
    ADD_EFFECT(el_Noise,tl_Christmas )
  END_MODE()      

  BEGIN_MODE("Effects")
    ADD_EFFECT(el_Ripple)
    ADD_EFFECT(el_Fire)
    ADD_EFFECT(el_MeteorRain)
    ADD_EFFECT(el_Plasma, tl_Plasma)
    ADD_EFFECT(el_Confetti, tl_Confetti)
    ADD_EFFECT(el_PaletteTransform, tl_Default) 
    ADD_EFFECT(el_Blur)        
    ADD_EFFECT(el_Noise, tl_Default)            
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_Rainbow) 
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)       
    ADD_EFFECT(el_PacificOcean) 
    ADD_EFFECT(el_Juggle)
    ADD_EFFECT(el_EmergencyLights)
  END_MODE()

  BEGIN_MODE("Old")
    ADD_EFFECT(el_FadeInOut)
    ADD_EFFECT(el_RunningLights)         //Single color
    ADD_EFFECT(el_ColorWipe)             //Not intersting
    ADD_EFFECT(el_TheaterChaseRainbow) 
    ADD_EFFECT(el_RainbowMove)
  END_MODE()
 END_EFFECTS()

//Effect Engine
BEGIN_EFFECT_ENGINE() 
  
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

    SERIAL_INPUT()     

  END_CONTROL_MAP()
    
END_EFFECT_ENGINE() 



#endif //__WINDOW_SETUP
