#if defined(__VADIM_SETUP)

////////////////////////////////////////
// 2 WS2811 string, 100 LEDs, 1 push button

#pragma message "Compile for Vadim"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - Vadim");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT( Confetti-hlw, EffectConfetti, &TransformAutunm )
        ADD_EFFECT( Noise-hlw, EffectNoise, &TransformHalloween )
        ADD_EFFECT( Plazma-hlw, EffectPlasma, &TransformAutunm )
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT( Confetti-crsm, EffectConfetti, &TransformChristmas)
        ADD_EFFECT( Plazma-crsm, EffectPlasma, &TransformChristmas )
        ADD_EFFECT( Noise-crsm, EffectNoise, &TransformChristmas )
      END_MODE()
      
      BEGIN_MODE(Other, 15)        
        ADD_EFFECT(Ripple, EffectRipple<>)   
        ADD_EFFECT(Meteor rain, EffectMeteorRain<>)
        ADD_EFFECT(Plazma, EffectPlasma)
        ADD_EFFECT(Confetti, EffectConfetti)    
        ADD_EFFECT(Blur, EffectBlur)
        ADD_EFFECT(Rainbow move, EffectRainbowMove)
        ADD_EFFECT(Noise, EffectNoise)            
        ADD_EFFECT(Mood blobs, EffectMoodBlobs) 
        ADD_EFFECT(Rainbow, EffectRainbow)
        ADD_EFFECT(Theater rainbow, EffectTheaterChaseRainbow) 
      END_MODE()    
    END_EFFECTS()
    
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
}


#endif // __VADIM_SETUP
