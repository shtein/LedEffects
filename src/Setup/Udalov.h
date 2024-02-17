#if defined(__UDALOV_SETUP)

////////////////////////////////////////
// WS2811 string, 200 LEDs, 1 push button, 1 Potentiometer

#pragma message "Compile for Udalov"

//Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Chrms")   
    ADD_EFFECT(el_Confetti, tl_Christmas )
    ADD_EFFECT(el_Plasma, tl_Christmas )
    ADD_EFFECT(el_Noise, tl_Christmas )
    ADD_EFFECT(el_TwinkleFox, tl_SnowAndIce)
  END_MODE() 
  BEGIN_MODE("Hlwn")      
    ADD_EFFECT(el_Confetti, tl_Halloween )
    ADD_EFFECT(el_Plasma, tl_Autumn )
    ADD_EFFECT(el_Noise, tl_Halloween )
  END_MODE()
  BEGIN_MODE("Effects")        
    ADD_EFFECT(el_Ripple)   
    ADD_EFFECT(el_MeteorRain)
    ADD_EFFECT(el_Plasma, tl_Plasma)
    ADD_EFFECT(el_Confetti, tl_Confetti)        
    ADD_EFFECT(el_Blur)
    ADD_EFFECT(el_RainbowMove)
    ADD_EFFECT(el_Noise, tl_Default)            
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
  END_MODE()    
END_EFFECTS()
  

BEGIN_EFFECT_ENGINE()   
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



#endif// __UDALOV
