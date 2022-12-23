#if defined(__FRAME_MATRIX_16x16)

///////////////////////////////////////////////////
// 16x16 Matrix 


#pragma message "Compile for 16x16 matrix"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - 16x16 matrix");  

  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(Bouncing dots, EffectMatrixBounsingDots)
        ADD_EFFECT(Kaledoscope - Confetti, EffectMatrixKaleidoscope<EffectConfetti>)
        ADD_EFFECT(Kaledoscope - Twinkle fox, EffectMatrixKaleidoscope<EffectTwinkleFox>)
        ADD_EFFECT(Kaledoscope - MoodBlobs, EffectMatrixKaleidoscope<EffectMoodBlobs >)
        ADD_EFFECT(Kaledoscope - Plazma, EffectMatrixKaleidoscope<EffectPlasma >)
        ADD_EFFECT(Twinkle Fox, EffectTwinkleFox)
        ADD_EFFECT(Pacific ocean, EffectPacificOcean)        
        ADD_EFFECT(Mood blobs, EffectMoodBlobs) 
        ADD_EFFECT(Drops, EffectMatrixDrops<8>, TwinkleFox)
        ADD_EFFECT(Circles, EffectMatrixCircles<2>)
      END_MODE()
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

  //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 
      
    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

#endif //__FRAME_MATRIX_16x16