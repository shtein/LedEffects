#if defined(__SOUND_MATRIX_16x16)

///////////////////////////////////////////////////
// 16x16 Matrix, with MSGEQ07 

#pragma message "Compile for 16x16 matrix with MSGEQ7 sound capture"

 //Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Sound")            
    ADD_EFFECT_SOUND(el_SoundVUM, sv_Symmetric)
    ADD_EFFECT_SOUND(el_SoundVUM, sv_Bar)
    ADD_EFFECT_SOUND(el_SoundMatrixRGB)
    ADD_EFFECT_SOUND(el_Confetti, tl_Confetti)
    ADD_EFFECT_SOUND(el_Maxtrix_Drops, tl_TwinkleFox)    
    ADD_EFFECT_SOUND(el_Fire)
    ADD_EFFECT_KALEYDOSCOPE(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT(el_Noise, tl_Noise)
    ADD_EFFECT(el_Matrix_Bouncing_Dots, tl_TwinkleFox)
  END_MODE()
END_EFFECTS()

  
  //Effect Engine
BEGIN_EFFECT_ENGINE() 
  
  INIT_SOUND_CAPTURE(SoundCaptureMSGEQ7, MSGEQ7_ANALOG_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_RESET_PIN)    

  //Leds
  BEGIN_LEDS()   
    ADD_STRIP(NEOPIXEL, LED_PIN)
  END_LEDS()

//Control    
  BEGIN_CONTROL_MAP()
 
  
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(PB_CONTROL_PUSH_LONG, EEMC_STATE)
      PUSH_BUTTON_TO_CMD(PB_CONTROL_CLICK_SHORT, EEMC_EFFECT)      
    END_PUSH_BUTTON() 
    
    
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 


#endif //__SOUND_MATRIX_16x16