#if defined( __DAVID_SETUP)

////////////////////////////////////////
// WS2811 strip, 50 LEDs, 1 push button
#pragma message "Compile for David"
  
  
BEGIN_EFFECT_ENGINE(David, 0) 
  
  //Effects   
  BEGIN_EFFECTS()
    BEGIN_MODE("Effects", 15)
      ADD_EFFECT("Fire", EffectFire)
      ADD_EFFECT("Meteor Rain", EffectMeteorRain<>)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Confetti", EffectConfetti)
      ADD_EFFECT("Transform fast", EffectPaletteTransformFast) 
      ADD_EFFECT("Blur", EffectBlur)
      ADD_EFFECT("Rainbow move", EffectRainbowMove)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs) 
      ADD_EFFECT("Rainbow", EffectRainbow)
      ADD_EFFECT("Fade in out", EffectFadeInOut)
      ADD_EFFECT("Running lights", EffectRunningLights)  
      ADD_EFFECT("Color wipe", EffectColorWipe)           
      ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 
    END_MODE()
    BEGIN_MODE("Chrsitmas", 3)      
      ADD_EFFECT("Confetti-crsm", EffectConfetti, &TransformChristmas)
      ADD_EFFECT("Plazma-crsm", EffectPlasma, &TransformChristmas )
      ADD_EFFECT("Noise-crsm", EffectNoise, &TransformChristmas )
    END_MODE()
    BEGIN_MODE("Halloween", 3)      
      ADD_EFFECT("Confetti-hlw", EffectConfetti, &TransformAutunm )
      ADD_EFFECT("Noise-hlw", EffectNoise, &TransformHalloween )
      ADD_EFFECT("Plazma-hlw", EffectPlasma, &TransformAutunm )
    END_MODE()
    BEGIN_MODE("Emergency", 1)
      ADD_EFFECT("Emergency lights", EffectEmergencyLights)         
    END_MODE()
  END_EFFECTS()
  
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
    
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__DAVID_SETUP
