//For Platformio specific setup is defined in platformio.ini using build_flags -DXXX option
//Far arduino IDE it needs to be #define XXX explictely in ledeffects.h

#include "LedEffects.h"

#include <AnalogInput.h>
#include <Controls.h>
#include <CtrlSerial.h>
#include <CtrlWiFi.h>
#include <CtrlWebSrv.h>
#include <SoundCapture.h>

#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include "Effect.h"
#include "EffectSound.h"

#include "Matrix.h"

#include "Macro.h"
#include "Pins.h"

//Effects
#include "EffectsAll.h"

//Setups
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


#if defined(__TEST)
#pragma message "Compile for default"

  //Effect Engine
BEGIN_EFFECT_ENGINE("Test", 0) 
  //Effects   
/*
  BEGIN_EFFECTS()    
    BEGIN_MODE("Effects", 16)
      ADD_EFFECT(el_StaticColor)
      ADD_EFFECT(el_Blur)
      ADD_EFFECT(el_ColorWipe)
    END_MODE()
        
    //BLACK_MODE()
  END_EFFECTS()
 */  

  //BEGIN_EFFECTS()
  if(1){ //check if need to rewrite effects config
    EE_CFG_EFFECT_ENGINE eeCfg;
    memset(&eeCfg, 0, sizeof(eeCfg));

    //BEGIN_MODE(modeName)



  //END_EFFECTS()
  }

 
  //Leds
  BEGIN_LEDS()       
    ADD_STRIP(NEOPIXEL, 2)
  END_LEDS()

//Control    

  BEGIN_CONTROL_MAP()         
    BEGIN_PUSH_BUTTON(0)    
      PUSH_BUTTON_TO_CMD(EEMC_MODE, PB_CONTROL_PUSH_LONG)
      PUSH_BUTTON_TO_CMD(EEMC_EFFECT, PB_CONTROL_CLICK_SHORT)      
    END_PUSH_BUTTON() 

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

