#if defined(__WINDOW_SETUP)

////////////////////////////////////////
// WS28101 string, 100 LEDs, 1 push button



#pragma message "Compile for office window"

  
//Effect Engine
BEGIN_EFFECT_ENGINE(Window, 0) 
  
  //Effects   
  BEGIN_EFFECTS()

    BEGIN_MODE("Themes", 15)        
      ADD_EFFECT("Confetti-autumn",  EffectConfetti, &TransformAutunm )
      ADD_EFFECT("Plazma-autumn",  EffectPlasma, &TransformAutunm)
      ADD_EFFECT("Noise-autumn",  EffectNoise, &TransformAutunm )
      ADD_EFFECT("Confetti-hlw", EffectConfetti, &TransformHalloween )
      ADD_EFFECT("Plazma-hlw", EffectPlasma, &TransformHalloween )
      ADD_EFFECT("Noise-hlw", EffectNoise, &TransformHalloween)
      ADD_EFFECT("Confetti-crsm", EffectConfetti, &TransformChristmas)
      ADD_EFFECT("Plazma-crsm", EffectPlasma, &TransformChristmas)
      ADD_EFFECT("Noise-crsm", EffectNoise, &TransformChristmas )
    END_MODE()      

    BEGIN_MODE("Effects", 15)
      ADD_EFFECT("Ripple", EffectRipple<>)
      ADD_EFFECT("Fire", EffectFire)
      ADD_EFFECT("Meteor rain", EffectMeteorRain<>)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Confetti", EffectConfetti)
      ADD_EFFECT("Palette transform", EffectPaletteTransformFast) 
      ADD_EFFECT("Blur", EffectBlur)        
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs) 
      ADD_EFFECT("Rainbow", EffectRainbow) 
      ADD_EFFECT("Twinkle fox", EffectTwinkleFox)       
      ADD_EFFECT("Pacific ocean", EffectPacificOcean) 
      ADD_EFFECT("Juggle", EffectJuggle)
      ADD_EFFECT("Emergency lights", EffectEmergencyLights)
    END_MODE()

    BEGIN_MODE("Old Effects", 8)
      //ADD_EFFECT("Fade in/out", EffectFadeInOut)
      //ADD_EFFECT("Running lighs", EffectRunningLights)         //Single color
      //ADD_EFFECT("Color wipe", EffectColorWipe)                //Not intersting
      ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 
      ADD_EFFECT("Rainbow move", EffectRainbowMove)
    END_MODE()

  END_EFFECTS()
  
  //Leds
  BEGIN_LEDS() 
    ADD_STRIP(WS2801, LED_PIN, LED_CLOCK_PIN, RGB) //Old 2801 strip
  END_LEDS()

//Control    
  BEGIN_CONTROL_MAP()    
 
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON()

    SERIAL_INPUT()     

  END_CONTROL_MAP()
    
END_EFFECT_ENGINE() 



#endif //__WINDOW_SETUP
