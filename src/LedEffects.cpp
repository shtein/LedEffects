//For Platformio specific setup is defined in platformio.ini using build_flags -DXXX option
//Far arduino IDE it needs to be #define XXX explictely in ledeffects.h

#include "LedEffects.h"

#include <AnalogInput.h>
#include <Controls.h>
#include <CtrlSerial.h>
#include <CtrlWiFi.h>
#include <CtrlWebSrv.h>
#include <SoundCapture.h>

#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include "EffectSound.h"
#include "Matrix.h"
#include "Macro.h"
#include "Pins.h"

//Effects
#include "EffectsAll.h"

//Setups
#ifndef SETUP_NAME
  #error "Setup name is not defined"
#endif

#include "Setup/ChristmasTree.h"
#include "Setup/David.h"
#include "Setup/Deck.h"
#include "Setup/Igor.h"
#include "Setup/Matrix16x16.h"
#include "Setup/MediaRoom.h"
#include "Setup/OutageLamp.h"
#include "Setup/Sasha.h"
#include "Setup/SashaTahoe.h"
#include "Setup/SoundMatrix.h"
#include "Setup/Tahoe.h"
#include "Setup/Udalov.h"
#include "Setup/Vadim.h"
#include "Setup/Window.h"
#include "Setup/AllPurpose.h"




#if defined(__TEST)
#pragma message "Compile for Test" 


//Effects   
BEGIN_EFFECTS(EFF_RANDOM_START_EFFECT)
  BEGIN_MODE("Regular")            
    ADD_EFFECT(el_Ripple)
    ADD_EFFECT(el_Plasma, tl_Plasma)        
    ADD_EFFECT(el_Confetti, tl_Confetti)
    ADD_EFFECT(el_Noise, tl_Default)            
    ADD_EFFECT(el_MoodBlobs)
    ADD_EFFECT(el_Juggle)
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
    ADD_EFFECT(el_PacificOcean)
    ADD_EFFECT(el_StaticColor, CHSV(HUE_RED, 0xFF, 0xFF))
  END_MODE()


  BEGIN_MODE("Halloween")      
    ADD_EFFECT(el_Confetti, tl_Autumn )
    ADD_EFFECT(el_Noise, tl_Halloween )  
  END_MODE()


  BEGIN_MODE("Chrsitmas")   
    ADD_EFFECT(el_Confetti, tl_Christmas)
    ADD_EFFECT(el_Plasma, tl_Christmas)
    ADD_EFFECT(el_TwinkleFox, tl_TwinkleFox)
  END_MODE()

/*
  BEGIN_MODE("Valentines", 3)      
    ADD_EFFECT("Confetti-vlnt", EffectConfetti, &ValentinesDay )
    ADD_EFFECT("Noise-vlnt", EffectNoise, &ValentinesDay )
    ADD_EFFECT("Twinkle fox-vlnt", EffectTwinkleFox, &ValentinesDay)
  END_MODE()

  BEGIN_MODE(July4, 2)      
    ADD_EFFECT("Confetti-ind", EffectConfetti, &July4th )
    ADD_EFFECT("Noise-ind", EffectNoise, &July4th )
  END_MODE()

  BEGIN_MODE(Ukraine)     
    ADD_EFFECT("Confetti-ukr", EffectConfetti, &UkraineFlag )
    ADD_EFFECT("Noise-ukr", EffectNoise, &UkraineFlag )
    ADD_EFFECT("Twinkle fox-ukr", EffectTwinkleFox, &UkraineFlag )
  END_MODE()
*/

END_EFFECTS()

  //Effect Engine
BEGIN_EFFECT_ENGINE() 
  //Leds
  BEGIN_LEDS()       
    ADD_STRIP(NEOPIXEL, LED_PIN)
  END_LEDS()

//Control    

  BEGIN_CONTROL_MAP()         

/*
    BEGIN_PUSH_BUTTON(0)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON() 
*/
    //Serial
    SERIAL_INPUT()
    //WiFi
    //WIFI_INPUT() 
    //Web    
    //WEB_INPUT(80)

    //SW2POS_TO_CMD(EEMC_SOUND_LOG, 7)
    //SW2POS_TO_CMD(EEMC_SOUND_USE_MAX, 6)
    //SW2POS_TO_CMD(EEMC_SOUND_USE_MIN, 5)
    //POT_TO_CMD(EEMC_SOUND_LOW, SOUND_LOW_PIN, POT_NOISE_THRESHOLD, 100)
    //POT_TO_CMD(EEMC_SOUND_SENSITIVITY, SOUND_HIGH_PIN, POT_NOISE_THRESHOLD, 300)
    
  
    //PUSH_BUTTON_SA_TO_CMD(EEMC_MODE, MODE_PIN)            //Mode
    //PUSH_BUTTON_SA_TO_CMD(EEMC_EFFECT, EFFECT_PIN)        //Effect
    //POT_TO_CMD(EEMC_NUMLEDS, NUMLEDS_PIN)                 //Number of leds
    //ROTENC_TO_CMD(EEMC_SPEED, SPEED_PIN, SPEED_CLOCK_PIN) //Speed
    
    //POT_TO_CMD(EEMC_MIC_SENS, MIC_SENS_PIN)               //Micrphone sensitivity
    //MIC_TO_CMD(EEMC_MIC, MIC_PIN)                         //Micrphone samples


    //Make sure ir remote receiver is wired and connected properly to arduino otherwise the program misbehaves: stops, slows down, etc
    /*
    BEGIN_REMOTE(REMOTE_PIN)  //Remote
      RMT_BUTTON_TO_CMD(EEMC_MODE, RKEY_OK)                         //Mode
      RMT_BUTTON_PAIR_TO_CMD(EEMC_EFFECT, RKEY_UP, RKEY_DOWN, 0)    //Effect
      RMT_BUTTON_PAIR_TO_CMD(EEMC_SPEED, RKEY_LEFT, RKEY_RIGHT, 5)  //Speed
      RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_HUE, RKEY_1, RKEY_4, 5)     //Color Hue
      RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_SAT, RKEY_2, RKEY_5, 5)     //Color Saturation
      RMT_BUTTON_PAIR_TO_CMD(EEMC_COLOR_VAL, RKEY_3, RKEY_6, 5)     //Color Value
    END_REMOTE()    
    */
    
  END_CONTROL_MAP()

      
END_EFFECT_ENGINE() 

#endif

