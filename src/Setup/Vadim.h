#if defined(__VADIM_SETUP)

////////////////////////////////////////
// 2 WS2811 string, 100 LEDs, 1 push button

#pragma message "Compile for Vadim"

 //Effects   
  BEGIN_EFFECTS(0)    
        
    BEGIN_MODE("Effects")        
      ADD_EFFECT(el_Ripple)   
      ADD_EFFECT(el_MeteorRain)
      ADD_EFFECT(el_Plasma, tl_Plasma)
      ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
      ADD_EFFECT(el_Confetti, tl_Confetti)    
      ADD_EFFECT(el_Blur)
      ADD_EFFECT(el_RainbowMove)
      ADD_EFFECT(el_Noise, tl_Default)            
      ADD_EFFECT(el_MoodBlobs) 
      ADD_EFFECT(el_Rainbow)
      ADD_EFFECT(el_TheaterChaseRainbow) 
      ADD_EFFECT(el_Juggle)
      ADD_EFFECT(el_PacificOcean)
      ADD_EFFECT(el_Fire)       
      ADD_EFFECT(el_EmergencyLights)        
    END_MODE()    
  

  BEGIN_MODE("Hlwn")      
    ADD_EFFECT(el_Confetti, tl_Autumn )
    ADD_EFFECT(el_Noise, tl_Halloween )
    ADD_EFFECT(tl_TwinkleFox, tl_Halloween)
  END_MODE()
    
  BEGIN_MODE("Chrms")      
    ADD_EFFECT(el_Confetti, tl_Christmas)
    ADD_EFFECT(el_Noise, tl_Christmas )
    ADD_EFFECT(el_TwinkleFox, tl_SnowAndIce)
  END_MODE()

  BEGIN_MODE("Flags")      
    ADD_EFFECT(el_Confetti, tl_UkrainianFlag )
    ADD_EFFECT(el_Noise, tl_UkrainianFlag )
    ADD_EFFECT(el_TwinkleFox, tl_UkrainianFlag)    
    ADD_EFFECT(el_Confetti, tl_July_4th )
    ADD_EFFECT(el_Noise, tl_July_4th )
    ADD_EFFECT(el_TwinkleFox, tl_July_4th)    
    ADD_EFFECT(el_Confetti, tl_PortugalFlag )
    ADD_EFFECT(el_Noise, tl_PortugalFlag )
    ADD_EFFECT(el_TwinkleFox, tl_PortugalFlag)    
  END_MODE()
  
END_EFFECTS()

//Effect Engine
BEGIN_EFFECT_ENGINE() 

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



#endif // __VADIM_SETUP
