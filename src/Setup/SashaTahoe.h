#if defined( __SASHA_TAHOE_SETUP )

////////////////////////////////////////
// WS2811 12v strip, 150 LEDs, 1 push button
#pragma message "Compile for Sasha at Tahoe"


  //Effects   
  BEGIN_EFFECTS(0)
    BEGIN_MODE("Effects")
      ADD_EFFECT(el_MeteorRain)
      ADD_EFFECT(el_Ripple)   
      ADD_EFFECT(el_Plasma, tl_Plasma)
      ADD_EFFECT(el_Confetti, tl_Confetti)
      ADD_EFFECT(el_Blur)
      ADD_EFFECT(el_TheaterChaseRainbow)
      ADD_EFFECT(el_RainbowMove)
      ADD_EFFECT(el_Noise, tl_Default)            
      ADD_EFFECT(el_MoodBlobs) 
      ADD_EFFECT(el_Rainbow)
      ADD_EFFECT(el_FadeInOut)
      ADD_EFFECT(el_RunningLights)           
      ADD_EFFECT(el_Juggle)
      ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
      ADD_EFFECT(el_PacificOcean)
    END_MODE()

    BEGIN_MODE("Static")
      ADD_EFFECT(el_StaticColor, CHSV(HUE_BLUE, 0xFF, 0xFF))        
      ADD_EFFECT(el_StaticColor, CHSV(HUE_PURPLE, 0xFF, 0xFF))
      ADD_EFFECT(el_StaticColor, CHSV(HUE_ORANGE, 0xFF, 0xFF))
    END_MODE()

  END_EFFECTS()


//Effect Engine
BEGIN_EFFECT_ENGINE() 
  
  
  //Leds
  BEGIN_LEDS()
    ADD_STRIP(WS2811, LED_PIN, BRG)
  END_LEDS()

//Control    
  BEGIN_CONTROL_MAP()
    
    BEGIN_PUSH_BUTTON(MODE_PIN)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON()     
  
  END_CONTROL_MAP()
      
END_EFFECT_ENGINE() 


#endif//__SASHA_SETUP


