#if defined(__FRAME_MATRIX_16x16)

///////////////////////////////////////////////////
// 16x16 Matrix 


#pragma message "Compile for 16x16 matrix"

//Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Effects")
    
    ADD_EFFECT_KALEYDOSCOPE(el_Confetti, tl_Confetti)
    ADD_EFFECT_KALEYDOSCOPE(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT_KALEYDOSCOPE(el_MoodBlobs)
    ADD_EFFECT_KALEYDOSCOPE(el_Plasma, tl_Plasma)
    ADD_EFFECT_KALEYDOSCOPE(el_Blur)
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT(el_PacificOcean)        
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_Maxtrix_Drops, tl_TwinkleFox)
    ADD_EFFECT(el_Matrix_Bouncing_Dots)
    ADD_EFFECT(el_Matrix_Circles, tl_Default)
  END_MODE()

END_EFFECTS()  

//Effect Engine
BEGIN_EFFECT_ENGINE() 
  
  //Leds
  BEGIN_LEDS() 
    ADD_STRIP(NEOPIXEL, LED_PIN)
  END_LEDS()

//Control    

  BEGIN_CONTROL_MAP()
    //SERIAL_INPUT()

    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON() 

  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__FRAME_MATRIX_16x16