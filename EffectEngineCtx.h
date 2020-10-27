#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H

//Speed delay
#define SPEED_DELAY_MIN 10
#define SPEED_DELAY_MAX 1000

#define EEMC_LED         0x80   //Something changed - update LEDs

#define EEMC_MODE        (EEMC_LED | 0x01)   //Mode changed
#define EEMC_EFFECT      (EEMC_LED | 0x02)   //Effect changed
#define EEMC_COLOR_HUE   (EEMC_LED | 0x04)   //Color hue changed
#define EEMC_COLOR_SAT   (EEMC_LED | 0x05)   //Color saturation changed
#define EEMC_COLOR_VAL   (EEMC_LED | 0x06)   //Color value changed   
#define EEMC_SPEED       (EEMC_LED | 0x08)   //Speed changed
#define EEMC_NUMLEDS     (EEMC_LED | 0x10)   //Number of leds changed

#define EEMC_SOUND_LOW          (EEMC_LED | 0x1A)   //Change lower 
#define EEMC_SOUND_HIGH         (EEMC_LED | 0x1B)   //Change upper
#define EEMC_SOUND_SENSITIVITY  (EEMC_LED | 0x1C)   //Enable sensitiviy
#define EEMC_SOUND_USE_MAX      (EEMC_LED | 0x1D)   //Use min lower
#define EEMC_SOUND_USE_MIN      (EEMC_LED | 0x1E)   //Use max upper
#define EEMC_SOUND_LOG          (EEMC_LED | 0x1F)   //Show audio in log scale


//Timeout to save config
#define SAVE_CONFIG_TIMEOUT 30000


#endif //__EFFECTENGINECTX_H
