#if defined( __SASHA_TAHOE_SETUP )

////////////////////////////////////////
// WS2811 12v strip, 150 LEDs, 1 push button
#pragma message "Compile for Sasha at Tahoe"


void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started -- Sasha's at Tahoe");  
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Effects, 15)
        ADD_EFFECT(Meteor rain, EffectMeteorRain<>)
        ADD_EFFECT(Ripple, EffectRipple<>)   
        ADD_EFFECT(Plazma, EffectPlasma)
        ADD_EFFECT(Confetti, EffectConfetti)
        ADD_EFFECT(Blur, EffectBlur)
        ADD_EFFECT(Theater rainbow, EffectTheaterChaseRainbow)
        ADD_EFFECT(Rainbow move, EffectRainbowMove)
        ADD_EFFECT(Noise, EffectNoise)            
        ADD_EFFECT(Mood blobs, EffectMoodBlobs) 
        ADD_EFFECT(Rainbow, EffectRainbow)
        ADD_EFFECT(Fade in out, EffectFadeInOut)
        ADD_EFFECT(Running lighs, EffectRunningLights)           
        ADD_EFFECT(Juggle, EffectJuggle)
        ADD_EFFECT(Twinkle fax, EffectTwinkleFox, TwinkleFox)
        ADD_EFFECT(Pacific ocean, EffectPacificOcean)

      END_MODE()
      BEGIN_MODE(Static, 3)
        ADD_STATIC_COLOR(HUE_BLUE)        
        ADD_STATIC_COLOR(HUE_PURPLE)
        ADD_STATIC_COLOR(HUE_ORANGE)
      END_MODE()
      BLACK_MODE()
    END_EFFECTS()
    
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
}

#endif//__SASHA_SETUP


