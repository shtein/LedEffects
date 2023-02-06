#if defined(__TAHOE_SETUP)

////////////////////////////////////////
// WS2811 string, 12v 417 LEDs, remote with separate buttons for effect and mode change, one button for effect and mode change
#pragma message "Compile Tahoe"


//Effect Engine
BEGIN_EFFECT_ENGINE(Tahoe, EFF_RANDOM_START_EFFECT)
  //Effects   

  BEGIN_EFFECTS()
    BEGIN_MODE("Effects", 8)        
      ADD_EFFECT("Ripple", EffectRipple<10>)
      ADD_EFFECT("Plazma", EffectPlasma)        
      ADD_EFFECT("Confetti", EffectConfetti)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs)
      ADD_EFFECT("Juggle", EffectJuggle)
      ADD_EFFECT("Twinkle fox", EffectTwinkleFox, TwinkleFox)
      ADD_EFFECT("Pcific ocean", EffectPacificOcean)
    END_MODE()

    BEGIN_MODE("Halloween", 2)      
      ADD_EFFECT("Confetti-hlw", EffectConfetti, &TransformAutunm )
      ADD_EFFECT("Noise-hlw", EffectNoise, &TransformHalloween )
    END_MODE()

    BEGIN_MODE("Chrsitmas", 3)      
      ADD_EFFECT("Confetti-crsm", EffectConfetti, &TransformChristmas)
      ADD_EFFECT("Plazma-crsm", EffectPlasma, &TransformChristmas )
      ADD_EFFECT("Twinkle fox-crsm", EffectTwinkleFox, &SnowAndIce)
    END_MODE()

    BEGIN_MODE("Valentines", 3)      
      ADD_EFFECT("Confetti-vlnt", EffectConfetti, &ValentinesDay )
      ADD_EFFECT("Noise-vlnt", EffectNoise, &ValentinesDay )
      ADD_EFFECT("Twinkle fox-vlnt", EffectTwinkleFox, &ValentinesDay)
    END_MODE()

    BEGIN_MODE(July4, 2)      
      ADD_EFFECT("Confetti-ind", EffectConfetti, &July4th )
      ADD_EFFECT("Noise-ind", EffectNoise, &July4th )
    END_MODE()

    BEGIN_MODE(Ukraine, 3)      
      ADD_EFFECT("Confetti-ukr", EffectConfetti, &UkraineFlag )
      ADD_EFFECT("Noise-ukr", EffectNoise, &UkraineFlag )
      ADD_EFFECT("Twinkle fox-ukr", EffectTwinkleFox, &UkraineFlag )
    END_MODE()

  END_EFFECTS()
  
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