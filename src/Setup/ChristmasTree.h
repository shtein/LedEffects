#if defined(__CHRISTMAS_TREE_SETUP)

////////////////////////////////////////
// WS2811 string, 150 LEDs, 1 push button, 1 potentiometer, 1 rotary encoder

#pragma message "Compile for christmas tree"

  //Effect Engine
BEGIN_EFFECT_ENGINE(christmas tree, 0) 
  
  //Effects   
  BEGIN_EFFECTS()
    BEGIN_MODE("Christmas", 4)   
      ADD_EFFECT("Confetti-crsm",  EffectConfetti, &TransformChristmas )
      ADD_EFFECT("Plazma-crsm", EffectPlasma, &TransformChristmas )
      ADD_EFFECT("Noise-crsm", EffectNoise, &TransformChristmas )
      ADD_EFFECT("Twinkle fox-crsm", EffectTwinkleFox, &SnowAndIce)
    END_MODE() 
    BEGIN_MODE("Halloween", 3)      
      ADD_EFFECT("Confetty-hlw", EffectConfetti, &TransformHalloween )
      ADD_EFFECT("Plazma-hlw", EffectPlasma, &TransformAutunm )
      ADD_EFFECT("Noise-hlw", EffectNoise, &TransformHalloween )
    END_MODE()
    BEGIN_MODE("Other", 10)        
      ADD_EFFECT("Ripple", EffectRipple<>)   
      ADD_EFFECT("Meteor rain", EffectMeteorRain<>)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Confetti", EffectConfetti)        
      ADD_EFFECT("Blur", EffectBlur)
      ADD_EFFECT("Rainbow move" ,EffectRainbowMove)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs) 
      ADD_EFFECT("Rainbow", EffectRainbow)
      ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 
    END_MODE()    
  END_EFFECTS()
  
  //Leds
  BEGIN_LEDS() 
    ADD_STRIP(WS2811, LED_PIN, RGB)  //First strip for Christmass Tree
    ADD_STRIP(WS2811, LED_PIN2, RGB) //Second strip for Christmass Tree
    ADD_STRIP(WS2811, LED_PIN3, RGB) //Third strip for Christmass Tree
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



#endif // __CHRISTMAS_TREE_STUP
