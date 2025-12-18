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
    ADD_EFFECT_KALEYDOSCOPE(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT(el_Matrix_Fire)
    ADD_EFFECT(el_Noise, tl_Noise)
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
      PUSH_BUTTON_TO_CMD(PB_CONTROL_PUSH_LONG, EEMC_MODE)
      PUSH_BUTTON_TO_CMD(PB_CONTROL_CLICK_SHORT, EEMC_EFFECT)      
    END_PUSH_BUTTON() 
  
    /*
    SW2POS_TO_CMD(7, EEMC_SOUND_LOG)
    SW2POS_TO_CMD(6, EEMC_SOUND_NOISE)
    SW2POS_TO_CMD(6, EEMC_SOUND_USE_MAX)
    SW2POS_TO_CMD(5, EEMC_SOUND_USE_MIN)
    POT_TO_CMD( SOUND_LOW_PIN, EEMC_SOUND_LOW)
    POT_TO_CMD(SOUND_HIGH_PIN, EEMC_SOUND_HIGH)
    */
    
    
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 


#endif //__SOUND_MATRIX_16x16