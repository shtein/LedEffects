#if defined(__DECK_SETUP)

////////////////////////////////////////
// WS2812B strip, 267 LEDs, 1 push button - change effect only

#pragma message "Compile for deck"

void setup() {


  DBG_INIT();
  DBG_OUTLN("Led effect started -- deck");  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(EFF_RANDOM_START_EFFECT)    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 8)        
        ADD_EFFECT(Ripple, EffectRipple<>)
        ADD_EFFECT(Meteor rain, EffectMeteorRain<>)   
        ADD_EFFECT(Plazma, EffectPlasma)        
        ADD_EFFECT(Confetti, EffectConfetti)
        ADD_EFFECT(Blur, EffectBlur)
        ADD_EFFECT(Rainbow move, EffectRainbowMove)
        ADD_EFFECT(Noise, EffectNoise)            
        ADD_EFFECT(Mood blobs, EffectMoodBlobs) 
      END_MODE()

      
      BEGIN_MODE(Halloween, 3)      
        ADD_EFFECT(Confetti-hlw, EffectConfetti, &TransformHalloween )
        ADD_EFFECT(Plazma-hlw, EffectPlasma, &TransformAutunm )
        ADD_EFFECT(Noise-hlw, EffectNoise, &TransformHalloween)
      END_MODE()
      
      BEGIN_MODE(Chrsitmas, 3)      
        ADD_EFFECT(Confetti-crsm, EffectConfetti, &TransformChristmas )
        ADD_EFFECT(Plazma-crsm, EffectPlasma, &TransformChristmas )
        ADD_EFFECT(Noise-crsm, EffectNoise, &TransformChristmas )
      END_MODE()

      BEGIN_MODE(Ukraine, 3)      
        ADD_EFFECT(Confetti-ukr, EffectConfetti, &UkraineFlag )
        ADD_EFFECT(Noise-urk, EffectNoise, &UkraineFlag )
        ADD_EFFECT(Twinkle fox-urk, EffectTwinkleFox, &UkraineFlag )
      END_MODE()  

      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS()
      ADD_STRIP(NEOPIXEL, LED_PIN)
      ADD_STRIP(NEOPIXEL, LED_PIN2)
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

#endif //__DECK_SETUP

