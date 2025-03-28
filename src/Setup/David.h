#if defined( __DAVID_SETUP)

////////////////////////////////////////
// WS2811 strip, 50 LEDs, 1 push button
#pragma message "Compile for David"
  
BEGIN_EFFECTS(0)
  BEGIN_MODE("Effects")
    ADD_EFFECT(el_Fire)
    ADD_EFFECT(el_MeteorRain)
    ADD_EFFECT(el_Plasma)
    ADD_EFFECT(el_Confetti)
    ADD_EFFECT(el_PaletteTransform) 
    ADD_EFFECT(el_Blur)
    ADD_EFFECT(el_RainbowMove)
    ADD_EFFECT(el_Noise)            
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_Rainbow)
    ADD_EFFECT(el_FadeInOut)
    ADD_EFFECT(el_RunningLights)  
    ADD_EFFECT(el_ColorWipe)           
    ADD_EFFECT(el_TheaterChaseRainbow) 
  END_MODE()
  BEGIN_MODE("Chrsitmas")      
    ADD_EFFECT(el_Confetti, tl_Christmas)
    ADD_EFFECT(el_Plasma, tl_Christmas )
    ADD_EFFECT(el_Noise, tl_Christmas )
  END_MODE()
  BEGIN_MODE("Halloween")      
    ADD_EFFECT(el_Confetti, tl_Autumn)
    ADD_EFFECT(el_Noise, tl_Halloween )
    ADD_EFFECT(el_Plasma, tl_Halloween )
  END_MODE()
  BEGIN_MODE("Emergency")
    ADD_EFFECT(el_EmergencyLights)         
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
      PUSH_BUTTON_TO_CMD(PB_CONTROL_PUSH_LONG, EEMC_MODE)
      PUSH_BUTTON_TO_CMD(PB_CONTROL_CLICK_SHORT, EEMC_EFFECT)      
    END_PUSH_BUTTON()
    
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__DAVID_SETUP
