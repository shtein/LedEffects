#if defined(__OUTAGE_LAMP_SETUP)

////////////////////////////////////////
// WS2801 strip, 31 LEDs, 2 push buttons
#pragma message "Compile for Outage Lamp"

//Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Effects")
    ADD_EFFECT(el_Plasma)
    ADD_EFFECT(el_Fire)       
    ADD_EFFECT(el_PaletteTransform) 
    ADD_EFFECT(el_RainbowMove)
    ADD_EFFECT(el_Noise)            
    ADD_EFFECT(el_Rainbow)
    ADD_EFFECT(el_FadeInOut)
    ADD_EFFECT(el_ColorWipe)
  END_MODE()
  BEGIN_MODE("Outage")
    ADD_EFFECT(el_EmergencyLights)         
  END_MODE()
END_EFFECTS()


BEGIN_EFFECT_ENGINE() 
//Leds
  BEGIN_LEDS()      
    ADD_STRIP(WS2801, LED_PIN, LED_CLOCK_PIN, RGB)
  END_LEDS()

//Control     
  BEGIN_CONTROL_MAP()
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(PB_CONTROL_CLICK_SHORT, EEMC_MODE, 0, CTF_VAL_ABS)      
      PUSH_BUTTON_TO_CMD(PB_CONTROL_PUSH_LONG, EEMC_EFFECT) 
    END_PUSH_BUTTON() 
    PUSH_BUTTON_SA_TO_CMD( MODE_PIN2, EEMC_MODE, 1, CTF_VAL_ABS)
  END_CONTROL_MAP()       
  
END_EFFECT_ENGINE() 

#endif//__OUTAGE_LAMP_SETUP
