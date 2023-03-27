#if defined(__VADIM_SETUP)

////////////////////////////////////////
// 2 WS2811 string, 100 LEDs, 1 push button

#pragma message "Compile for Vadim"

  //Effect Engine
BEGIN_EFFECT_ENGINE(Vadim, 0) 
  
  //Effects   
  BEGIN_EFFECTS()    
        
    BEGIN_MODE("Other", 15)        
      ADD_EFFECT("Ripple", EffectRipple<>)   
      ADD_EFFECT("Meteor rain", EffectMeteorRain<>)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Twinkle fox", EffectTwinkleFox, TwinkleFox)
      ADD_EFFECT("Confetti", EffectConfetti)    
      ADD_EFFECT("Blur", EffectBlur)
      ADD_EFFECT("Rainbow move", EffectRainbowMove)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs) 
      ADD_EFFECT("Rainbow", EffectRainbow)
      ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 
      ADD_EFFECT("Juggle", EffectJuggle)
      ADD_EFFECT("Pacific Ocean", EffectPacificOcean)
      ADD_EFFECT("Fire", EffectFire)       
      ADD_EFFECT("Emergency", EffectEmergencyLights)        
    END_MODE()    
  END_EFFECTS()

  BEGIN_MODE("Halloween", 3)      
    ADD_EFFECT("Confetti-hlw", EffectConfetti, &TransformAutunm )
    ADD_EFFECT("Noise-hlw", EffectNoise, &TransformHalloween )
    ADD_EFFECT("Twinkle fox-hlv", EffectTwinkleFox, &TransformHalloween)
  END_MODE()
    
  BEGIN_MODE("Chrsitmas", 3)      
    ADD_EFFECT("Confetti-crsm", EffectConfetti, &TransformChristmas)
    ADD_EFFECT("Noise-crsm", EffectNoise, &TransformChristmas )
    ADD_EFFECT("Twinkle fox-crsm", EffectTwinkleFox, &SnowAndIce)
  END_MODE()

  BEGIN_MODE("Flags", 9)      
    ADD_EFFECT("Confetti-ukr", EffectConfetti, &UkraineFlag )
    ADD_EFFECT("Noise-ukr", EffectNoise, &UkraineFlag )
    ADD_EFFECT("Twinkle fox-ukr", EffectTwinkleFox, &UkraineFlag)    
    ADD_EFFECT("Confetti-us", EffectConfetti, &July4th )
    ADD_EFFECT("Noise-us", EffectNoise, &July4th )
    ADD_EFFECT("Twinkle fox-ukr", EffectTwinkleFox, &July4th)    
    ADD_EFFECT("Confetti-us", EffectConfetti, &PortugalFlag )
    ADD_EFFECT("Noise-us", EffectNoise, &PortugalFlag )
    ADD_EFFECT("Twinkle fox-ukr", EffectTwinkleFox, &PortugalFlag)    
  END_MODE()
  
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
