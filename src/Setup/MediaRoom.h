#if defined(__MEDIA_ROOM_SETUP)

//////////////////////////////////////////
// 

#include <SoundCapture.h>

#pragma message "Compile for media room"

void setup() {
  DBG_INIT();
  DBG_OUTLN("Led effect started - media room");  

  INIT_SOUND_CAPTURE(SoundCaptureMSGEQ7, MSGEQ7_ANALOG_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_RESET_PIN)    
  
  //Effect Engine
  BEGIN_EFFECT_ENGINE(0) 
    //Effects   
    BEGIN_EFFECTS()
      BEGIN_MODE(Sound, 2)
        ADD_EFFECT(Sound, EffectSound)
      END_MODE()
      BEGIN_MODE(Effects, 10)                        
        ADD_EFFECT(Juggle, EffectJuggle)
        ADD_EFFECT(Twinkle fox, EffectTwinkleFox, TwinkleFox)        
        ADD_EFFECT(Pacific ocean, EffectPacificOcean)         
      END_MODE()      
    END_EFFECTS()
    
    //Leds
    BEGIN_LEDS() 
      ADD_STRIP(NEOPIXEL, LED_PIN)
    END_LEDS()

    //Control    
 
    BEGIN_CONTROL_MAP()
     
      BEGIN_PUSH_BUTTON(MODE_PIN)    
        PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
        PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
      END_PUSH_BUTTON() 

      SW2POS_TO_CMD(EEMC_SOUND_LOG, 7)
      SW2POS_TO_CMD(EEMC_SOUND_USE_MAX, 6)
      SW2POS_TO_CMD(EEMC_SOUND_USE_MIN, 5)
      POT_TO_CMD(EEMC_SOUND_LOW, SOUND_LOW_PIN, POT_NOISE_THRESHOLD, 100)
      POT_TO_CMD(EEMC_SOUND_SENSITIVITY, SOUND_HIGH_PIN, POT_NOISE_THRESHOLD, 300)
     

    END_CONTROL_MAP()
       
  END_EFFECT_ENGINE() 
}

#endif //__MEDIA_ROOM_SETUP