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
#include "Effects/Rainbow.h"
#include "Effects/FadeInOut.h"
#include "Effects/RunningLights.h"
#include "Effects/ColorWipe.h"
#include "Effects/MoodBlobs.h"
#include "Effects/Static.h"
#include "Effects/Noise.h"
#include "Effects/Blur.h"
#include "Effects/EmergencyLights.h"
#include "Effects/Fire.h"
#include "Effects/Meteor.h"  
#include "Effects/Ripple.h"
#include "Effects/Juggle.h"
#include "Effects/TwinkleFox.h"
#include "Effects/PacificOcean.h"
#include "Effects/MatrixAnimation.h"
#include "Effects/Sound.h"

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
BEGIN_EFFECT_ENGINE(Test, 0) 
  //Effects   
  BEGIN_EFFECTS()
    //BEGIN_MODE(Sound, 3)            
    //  ADD_EFFECT(EffectSoundMatrixSymmetric)
      // ADD_EFFECT(EffectSoundMatrixColumn)
    //END_MODE()
    
    BEGIN_MODE("Effects", 15)
      ADD_EFFECT("Twinkle fox", EffectTwinkleFox)
      ADD_EFFECT("Confetti", EffectConfetti)
      ADD_EFFECT("Meteor rain", EffectMeteorRain<5>)
      //ADD_EFFECT("Plazma", EffectPlasma)        
      ADD_EFFECT("Juggle", EffectJuggle)
      ADD_EFFECT("Pacific Ocean", EffectPacificOcean)        
      ADD_EFFECT("Blur", EffectBlur)
      ADD_EFFECT("Noise", EffectNoise)            
      ADD_EFFECT("Mood blobs", EffectMoodBlobs)         
      ADD_EFFECT("Ripple", EffectRipple<5>)         
      ADD_EFFECT("Fade in out", EffectFadeInOut)
      ADD_EFFECT("Running lights", EffectRunningLights)         //Single color
      ADD_EFFECT("Color wipe", EffectColorWipe)               //Not intersting   
      ADD_EFFECT("Theater rainbow", EffectTheaterChaseRainbow) 

    END_MODE()
    
    BEGIN_MODE("Static", 1)    
      ADD_STATIC_COLOR("Static Color", HUE_BLUE)
    END_MODE()
    
    //BLACK_MODE()
  END_EFFECTS()
  
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
    
    //Web
    ADD_API_REQUEST_HANDLER(HTTP_GET, "/api");
    WEB_INPUT(80)

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

