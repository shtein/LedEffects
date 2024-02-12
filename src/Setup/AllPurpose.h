#if defined( __ALL_PURPOSE_SETUP)

////////////////////////////////////////////////////////
// WS2811 strip, 300 LEDs, 1 push button, 1 potentiometer
#pragma message "Compile for All Purpose"
  
  
BEGIN_EFFECT_ENGINE(All purpose, 0) 
  
  //Effects   
  BEGIN_EFFECTS()
    BEGIN_MODE("Effects", 15)      
      ADD_EFFECT("Meteor Rain", EffectMeteorRain<>)
      ADD_EFFECT("Plazma", EffectPlasma)
      ADD_EFFECT("Confetti", EffectConfetti)
      ADD_EFFECT("Blur", EffectBlur)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs)  
      ADD_EFFECT("Twinke fox", EffectTwinkleFox)    
    END_MODE()
    BEGIN_MODE("Chrsitmas", 3)      
      ADD_EFFECT("Confetti-crsm", EffectConfetti, &TransformChristmas)
      ADD_EFFECT("Twinke fox-crsms", EffectTwinkleFox, &SnowAndIce)    
    END_MODE()
    BEGIN_MODE("Halloween", 3)      
      ADD_EFFECT("Confetti-hlw", EffectConfetti, &TransformAutunm )
      ADD_EFFECT("Noise-hlw", EffectNoise, &TransformHalloween )      
    END_MODE()  
    BEGIN_MODE("Emergency", 2)
      ADD_EFFECT("Fire", EffectFire)
      ADD_EFFECT("Emergency lights", EffectEmergencyLights)         
    END_MODE()
    BEGIN_MODE("Flags", 3)
      ADD_EFFECT("Twinke fox-Isrl",  EffectTwinkleFox, &IsraelFlag )
      ADD_EFFECT("Twinkle fox-urk", EffectTwinkleFox, &UkraineFlag )
      ADD_EFFECT("Twinkle fox-us", EffectTwinkleFox, &July4th)
    END_MODE()
    
  END_EFFECTS()
  
  //Leds
  BEGIN_LEDS()
    ADD_STRIP(WS2811, LED_PIN, BRG)
    ADD_STRIP(WS2811, LED_PIN2, BRG)
  END_LEDS()

//Control     
  BEGIN_CONTROL_MAP()
    
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON()
    
    POT_TO_CMD(EEMC_NUMLEDS, NUMLEDS_PIN, POT_NOISE_THRESHOLD)  
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__ALL_PURPOSE_SETUP
