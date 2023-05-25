#if defined(__OUTAGE_LAMP_SETUP)

////////////////////////////////////////
// WS2801 strip, 31 LEDs, 2 push buttons
#pragma message "Compile for Outage Lamp"


BEGIN_EFFECT_ENGINE(Outage lamp, 0) 
  
  //Effects   
  BEGIN_EFFECTS()
    BEGIN_MODE("Effects", 15)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Fire", EffectFire)       
      ADD_EFFECT("Transform fast", EffectPaletteTransformFast) 
      ADD_EFFECT("Rainbow move", EffectRainbowMove)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Rainbow", EffectRainbow)
      ADD_EFFECT("Fade in out", EffectFadeInOut)
      ADD_EFFECT("Color wipe", EffectColorWipe)
    END_MODE()
    BEGIN_MODE("Outage", 1)
      ADD_EFFECT("Emergancy lighs", EffectEmergencyLights)         
    END_MODE()
  END_EFFECTS()
  
  //Leds
  BEGIN_LEDS()      
    ADD_STRIP(WS2801, LED_PIN, LED_CLOCK_PIN, RGB) //Old 2801 strip or EK outage lamp
  END_LEDS()

//Control     
  BEGIN_CONTROL_MAP()
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_CLICK_SHORT, CTF_VAL_ABS, 0)      
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_PUSH_LONG) 
    END_PUSH_BUTTON() 
    PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN2, CTF_VAL_ABS, 1)
  END_CONTROL_MAP()       
  
END_EFFECT_ENGINE() 

#endif//__OUTAGE_LAMP_SETUP
