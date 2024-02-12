#if defined(__DECK_SETUP)

////////////////////////////////////////
// WS2812B strip, 267 LEDs, 1 push button - change effect only

#pragma message "Compile for deck"


//Effects   
BEGIN_EFFECTS(EFF_RANDOM_START_EFFECT)
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

  BEGIN_MODE("Hlwn")      
    ADD_EFFECT(el_Confetti, tl_Halloween )
    ADD_EFFECT(el_Plasma, tl_Autumn )
    ADD_EFFECT(el_Noise, tl_Halloween)
  END_MODE()
  
  BEGIN_MODE("Chrms")      
    ADD_EFFECT(el_Confetti, tl_Christmas )
    ADD_EFFECT(el_Plasma, tl_Christmas )
    ADD_EFFECT(el_Noise,tl_Christmas )
  END_MODE()

  BEGIN_MODE("Ukraine")      
    ADD_EFFECT(el_Confetti, tl_UkrainianFlag )
    ADD_EFFECT(el_Noise, tl_UkrainianFlag )
    ADD_EFFECT(el_TwinkleFox, tl_UkrainianFlag )
  END_MODE()  

END_EFFECTS()
  

BEGIN_EFFECT_ENGINE("Deck")      
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


#endif //__DECK_SETUP

