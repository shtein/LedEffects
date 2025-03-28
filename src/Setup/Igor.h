#if defined( __IGOR_SETUP)

////////////////////////////////////////
// WS2812B strip, 88 LEDs, remote control

#pragma message "Compile for Igor"

//Effects   
BEGIN_EFFECTS(0)
  BEGIN_MODE("Effects")
    ADD_EFFECT(el_Fire)
    ADD_EFFECT(el_MeteorRain)
    ADD_EFFECT(el_Plasma)
    ADD_EFFECT(el_Confetti)
    ADD_EFFECT(el_PaletteTransform) 
    ADD_EFFECT(el_Blur)
    ADD_EFFECT(el_RainbowMove)
    ADD_EFFECT(el_Noise)            
    ADD_EFFECT(el_MoodBlobs) 
    ADD_EFFECT(el_Rainbow)
    ADD_EFFECT(el_FadeInOut)
    ADD_EFFECT(el_RunningLights)
    ADD_EFFECT(el_ColorWipe) 
    ADD_EFFECT(el_TheaterChaseRainbow) 
  END_MODE()
  BEGIN_MODE("Static")
    ADD_EFFECT(el_StaticColor, CHSV(HUE_RED, 255, 255))
    ADD_EFFECT(el_StaticColor, CHSV(HUE_GREEN, 255, 255))
    ADD_EFFECT(el_StaticColor, CHSV(HUE_BLUE, 255, 255))
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
    //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc      
    BEGIN_REMOTE(REMOTE_PIN)  //Remote
      RMT_BUTTON_TO_CMD(R_DEC_KEY_OK, EEMC_MODE)                                   //Mode
      RMT_BUTTON_PAIR_PREV_NEXT_TO_CMD(EEMC_EFFECT, R_DEC_KEY_UP, R_DEC_KEY_DOWN)  //Effect
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_LEFT, R_DEC_KEY_RIGHT, EEMC_SPEED, 5) //Speed
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_1, R_DEC_KEY_4, EEMC_COLOR_HUE, 5)    //Color Hue
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_2, R_DEC_KEY_5, EEMC_COLOR_SAT, 5)    //Color Saturation
      RMT_BUTTON_PAIR_DELTA_TO_CMD(R_DEC_KEY_3, R_DEC_KEY_6, EEMC_COLOR_VAL, 5)    //Color Value
    END_REMOTE()          
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 

#endif //__IGOR_SETUP
