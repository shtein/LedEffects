#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H

//Leds
#define MAX_LEDS    300

//Speed delay
#define SPEED_DELAY_MIN 1
#define SPEED_DELAY_MAX 1000

//Change flags
#define EEMC_NONE    0x0000   //Nothing changed
#define EEMC_MODE    0x0001   //Mode changed
#define EEMC_EFFECT  0x0002   //Effect changed
#define EEMC_COLOR   0x0004   //Color changed
#define EEMC_SPEED   0x0008   //Speed changed
#define EEMC_NUMLEDS 0x0010   //Number of leds changed

//Modes
#define EEM_OFF      0x0000    //Off
#define EEM_STATIC   0x0001    //static
#define EEM_EFFECT   0x0002    //Effect
#define EEM_RANDOM   0x0003    //Random effect change after random interval bettwen 2 and 5 minutes
#define EEM_SETUP    0x0004    //Setup
#define EEM_MODEMAX  EEM_RANDOM


///////////////////////////////////////
// cr - change flag
// mode - current mode
// delaySpeed - to be passed to the effect
// curEffect  - effect to run
// numLeds    - number of of leds to use

struct EffectEngineCtx {
  int   cf;
  int   mode;
  int   speedDelay;
  int   maxEffects;
  int   effectNum;
  int   numLeds;
  CHSV  hsv;
};

#endif //__EFFECTENGINECTX_H
