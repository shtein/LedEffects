#include "precomp.h"
#include "LedEffects.h"
#include "EffectEngine.h"
#include "pins.h"

void setup() {
  DBG_INIT();

  //Effect Engine
  EffectEngine ee;
  BEGIN_EFFECTS(ee)
    ADD_EFFECT(EffectNoise)
    ADD_EFFECT(EffectMoodBlobs)
    ADD_EFFECT(EffectRainbow)
    ADD_EFFECT(EffectFadeInOut)
    ADD_EFFECT(EffectRunningLights)
    ADD_EFFECT(EffectColorWipe)
  END_EFFECTS()
  
  //Init
  ee.init(50, EEM_STATIC); //88 for Igor

  //Control panel
  EffectControlPanel cp;
  BEGIN_CONTROL_MAP(cp)
    PUSH_BUTTON_TO_CMD(Mode, EEMC_MODE, MODE_PIN)  
    PUSH_BUTTON_TO_CMD(Effect, EEMC_EFFECT, EFFECT_PIN)  
    POT_TO_CMD(Leds, EEMC_NUMLEDS, NUMLEDS_PIN)
    ROTENC_TO_CMD(Speed, EEMC_SPEED, SPEED_PIN, SPEED_CLOCK_PIN)
    
    BEGIN_REMOTE(Remote, REMOTE_PIN)
      RMT_BUTTON_TO_CMD(Mode, EEMC_MODE, RKEY_OK)
      RMT_BUTTON_PAIR_TO_CMD(Effect, EEMC_EFFECT, RKEY_UP, RKEY_DOWN, 0) 
      RMT_BUTTON_PAIR_TO_CMD(Speed, EEMC_SPEED, RKEY_LEFT, RKEY_RIGHT, 5) 
      RMT_BUTTON_PAIR_TO_CMD(ColorHue, EEMC_COLOR_HUE, RKEY_1, RKEY_4, 5) 
      RMT_BUTTON_PAIR_TO_CMD(ColorSat, EEMC_COLOR_SAT, RKEY_2, RKEY_5, 5) 
      RMT_BUTTON_PAIR_TO_CMD(ColorVal, EEMC_COLOR_VAL, RKEY_3, RKEY_6, 5) 
    END_REMOTE()    
    
  END_CONTROL_MAP()

  //Main loop
  CtrlQueueItem itm;

  for( ;; ){      
     cp.loop(itm);
     ee.loop(itm);   
  }
}



void loop(){
}


