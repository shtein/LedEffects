#if defined(__TAHOE_SETUP)

////////////////////////////////////////
// WS2811 string, 12v 417 LEDs, remote with separate buttons for effect and mode change, one button for effect and mode change
#pragma message "Compile for Tahoe"

//Effects   
BEGIN_EFFECTS(EFF_RANDOM_START_EFFECT)
  BEGIN_MODE("Effects")        
    ADD_EFFECT(el_Ripple)
    ADD_EFFECT(el_Plasma, tl_Plasma)        
    ADD_EFFECT(el_Confetti, tl_Confetti)
    ADD_EFFECT(el_Noise, tl_Default)            
    ADD_EFFECT(el_MoodBlobs)
    ADD_EFFECT(el_Juggle)
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT(el_PacificOcean)
  END_MODE()

  BEGIN_MODE("Hlwn")      
    ADD_EFFECT(el_Confetti, tl_Autumn )
    ADD_EFFECT(el_Noise, tl_Halloween)
    ADD_EFFECT(el_TwinkleFox, tl_Halloween)
  END_MODE()

  BEGIN_MODE("Chrms")      
    ADD_EFFECT(el_Confetti,tl_Christmas)
    ADD_EFFECT(el_Plasma, tl_Christmas )
    ADD_EFFECT(el_TwinkleFox, tl_SnowAndIce)
  END_MODE()

  BEGIN_MODE("Vlntns")      
    ADD_EFFECT(el_Confetti, tl_Valentines)
    ADD_EFFECT(el_Noise, tl_Valentines )
    ADD_EFFECT(el_TwinkleFox, tl_Valentines)
  END_MODE()

  BEGIN_MODE("July4")      
    ADD_EFFECT(el_Confetti, tl_July_4th )
    ADD_EFFECT(el_Noise, tl_July_4th )
    ADD_EFFECT(el_TwinkleFox, tl_July_4th)
  END_MODE()

/*
  BEGIN_MODE("Ukraine")      
    ADD_EFFECT(el_Confetti, tl_UkrainianFlag )
    ADD_EFFECT(el_Noise, tl_UkrainianFlag )
    ADD_EFFECT(el_TwinkleFox, tl_UkrainianFlag )
  END_MODE()

  BEGIN_MODE("Israel")      
    ADD_EFFECT(el_Confetti, tl_IsraelFlag )
    ADD_EFFECT(el_Noise, tl_IsraelFlag )
    ADD_EFFECT(el_TwinkleFox, tl_IsraelFlag )
  END_MODE()
*/    
END_EFFECTS()


//Effect Engine
BEGIN_EFFECT_ENGINE()

  //Leds
  BEGIN_LEDS()
    ADD_STRIP(WS2811, LED_PIN2)
    ADD_STRIP(WS2811, LED_PIN3)
  END_LEDS()

//Control    
  BEGIN_CONTROL_MAP()
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON()

    PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN3)
    PUSH_BUTTON_SA_TO_CMD(EEMC_EFFECT, EFFECT_PIN)      

    //SERIAL_INPUT()
    
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 


#endif //__TAHOE_SETUP