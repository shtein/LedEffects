#if defined( __SASHA_SETUP )

////////////////////////////////////////
// WS2812b strip, 212 LEDs, 1 push button, remote control
#pragma message "Compile for Sasha"


//Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Effects")
    ADD_EFFECT(el_MeteorRain)
    ADD_EFFECT(el_Plasma)
    ADD_EFFECT(el_Confetti)  
    ADD_EFFECT(el_Blur)
    ADD_EFFECT(el_RainbowMove)
    ADD_EFFECT(el_Noise)            
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_Rainbow)
    ADD_EFFECT(el_FadeInOut)
    ADD_EFFECT(el_RunningLights)           
    ADD_EFFECT(el_ColorWipe)               
    ADD_EFFECT(el_TheaterChaseRainbow) 
    ADD_EFFECT(el_TwinkleFox) 
  END_MODE()

  BEGIN_MODE("Static")    
    ADD_EFFECT(el_StaticColor, CHSV(HUE_PURPLE, 0xFF, 0xFF))
  END_MODE()

END_EFFECTS()


BEGIN_EFFECT_ENGINE() 
  
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
  
  
    //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc      
    BEGIN_REMOTE(REMOTE_PIN)  //Remote
      RMT_BUTTON_TO_CMD(R_DEC_KEY_OK, EEMC_MODE)                                  //Mode                       
      RMT_BUTTON_PAIR_PREV_NEXT_TO_CMD(R_DEC_KEY_UP, R_DEC_KEY_DOWN, EEMC_EFFECT)      //Effect
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_RIGHT, R_DEC_KEY_LEFT, EEMC_SPEED, 5)     //Speed
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_1, R_DEC_KEY_4, EEMC_COLOR_HUE, 5)          //Color Hue
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_2, R_DEC_KEY_5, EEMC_COLOR_SAT , 5)       //Color Saturation    
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_3, R_DEC_KEY_6, EEMC_COLOR_VAL, 5)        //Color Value
      
    END_REMOTE()    
   
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__SASHA_SETUP
