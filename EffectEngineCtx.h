#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H

#include "Controls.h"

//Speed delay
#define SPEED_DELAY_MIN 10
#define SPEED_DELAY_MAX 1000

//Change commands
#define EEMC_NONE        0x00   //Nothing changed
#define EEMC_MODE        0x01   //Mode changed
#define EEMC_EFFECT      0x02   //Effect changed
#define EEMC_COLOR_HUE   0x04   //Color hue changed
#define EEMC_COLOR_SAT   0x05   //Color saturation changed
#define EEMC_COLOR_VAL   0x06   //Color value changed   
#define EEMC_SPEED       0x08   //Speed changed
#define EEMC_NUMLEDS     0x10   //Number of leds changed


//Timeout to save config
#define SAVE_CONFIG_TIMEOUT 30000



#endif //__EFFECTENGINECTX_H
