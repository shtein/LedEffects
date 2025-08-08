#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H


//Speed delay
#define SPEED_DELAY_MIN 10
#define SPEED_DELAY_MAX 250

#define EEMC_LED         0x80   //Something changed - update LEDs

//Engine commands
#define EEMC_EE                  0x20   //Generic effect engine
#define EEMC_GET_MODE            0x21   //Mode 
#define EEMC_GET_EFFECT          0x22   //Effect 
#define EEMC_GET_NUMLEDS         0x23   //Number of leds 
#define EEMC_GET_MODE_LIST       0x24   //Available modes
#define EEMC_GET_EFFECT_LIST     0x25   //Available effects
#define EEMC_GET_TRANSFORM_LIST  0x26   //Available pallete transforms
#define EEMC_GET_VERSION         0x27   //Get version

#define EEMC_MODE        (EEMC_LED | EEMC_GET_MODE)       //Mode changed
#define EEMC_EFFECT      (EEMC_LED | EEMC_GET_EFFECT)     //Effect changed
#define EEMC_NUMLEDS     (EEMC_LED | EEMC_GET_NUMLEDS)    //Number of leds changed

//Effect commands
#define EEMC_GET_CFG        0x01   //Get effect config
#define EEMC_GET_COLOR_HSV  0x02   //Get effect HSV
#define EEMC_GET_COLOR_HUE  0x03   //Color hue 
#define EEMC_GET_COLOR_SAT  0x04   //Color saturation 
#define EEMC_GET_COLOR_VAL  0x05   //Color value 
#define EEMC_GET_SPEED      0x06   //Speed
#define EEMC_GET_TRANSFORM  0x07   //Paletter transform rutine

#define EEMC_COLOR_HUE   (EEMC_LED | EEMC_GET_COLOR_HUE)  //Color hue changed
#define EEMC_COLOR_SAT   (EEMC_LED | EEMC_GET_COLOR_SAT)  //Color saturation changed
#define EEMC_COLOR_VAL   (EEMC_LED | EEMC_GET_COLOR_VAL)  //Color value changed   
#define EEMC_SPEED       (EEMC_LED | EEMC_GET_SPEED)      //Speed changed
#define EEMC_TRANSFORM   (EEMC_LED | EEMC_GET_TRANSFORM)  //Palette transform

//Effect sound commands
#define EEMC_GET_SOUND                    0x10   //Stat and config
#define EEMC_GET_SOUND_LOW                0x11   //Lower 
#define EEMC_GET_SOUND_HIGH               0x12   //Upper
#define EEMC_GET_SOUND_USE_MAX            0x13   //Min lower
#define EEMC_GET_SOUND_USE_MIN            0x14   //Max upper
#define EEMC_GET_SOUND_LOG                0x15   //Log scale
#define EEMC_GET_SOUND_NOISE              0x16   //Noise threshold


#define EEMC_SOUND_LOW          (EEMC_LED | EEMC_GET_SOUND_LOW)          //Change lower 
#define EEMC_SOUND_HIGH         (EEMC_LED | EEMC_GET_SOUND_HIGH)         //Change upper
#define EEMC_SOUND_USE_MAX      (EEMC_LED | EEMC_GET_SOUND_USE_MAX)      //Use min lower
#define EEMC_SOUND_USE_MIN      (EEMC_LED | EEMC_GET_SOUND_USE_MIN)      //Use max upper
#define EEMC_SOUND_LOG          (EEMC_LED | EEMC_GET_SOUND_LOG)          //Show audio in log scale
#define EEMC_SOUND_NOISE        (EEMC_LED | EEMC_GET_SOUND_NOISE)        //Enable/disable noise threshold

//Errors
#define EEER_SUCCESS    0x00 //No error
#define EEER_INVALID    0x01 //Invalid input
#define EEER_UNHANDLED  0x02 //Unhandled command
#define EEER_DELAYED    0x03 //Delayed response

//Timeout to save config
#define SAVE_CONFIG_TIMEOUT 30000

#ifdef NTF_ENABLED

#include <Notification.h>


///////////////////////////////////////
// Serialization for command responses
DECLARE_STR_PROGMEM(rs_Mode)
DECLARE_STR_PROGMEM(rs_Effect)
DECLARE_STR_PROGMEM(rs_NumLeds)
DECLARE_STR_PROGMEM(rs_MaxLeds)
DECLARE_STR_PROGMEM(rs_Speed)
DECLARE_STR_PROGMEM(rs_Hue)
DECLARE_STR_PROGMEM(rs_Sat)
DECLARE_STR_PROGMEM(rs_Val)
DECLARE_STR_PROGMEM(rs_HSV)
DECLARE_STR_PROGMEM(rs_Transforms)
DECLARE_STR_PROGMEM(rs_Transform)
DECLARE_STR_PROGMEM(rs_Flags)
DECLARE_STR_PROGMEM(rs_Cfg)
DECLARE_STR_PROGMEM(rs_Dsc)

#ifdef USE_SOUND
DECLARE_STR_PROGMEM(rs_SndLower)
DECLARE_STR_PROGMEM(rs_SndUpper)
DECLARE_STR_PROGMEM(rs_SndMin)
DECLARE_STR_PROGMEM(rs_SndMax)
DECLARE_STR_PROGMEM(rs_SndAverage)
DECLARE_STR_PROGMEM(rs_SndStdDev)
DECLARE_STR_PROGMEM(rs_SoundVIM)
#endif  

#else 

#define NtfSet void*

#endif //NTF_ENABLED


#ifdef USE_SOUND

enum SoundVUMType{
  sv_None = 0,
  sv_Symmetric,
  sv_Bar
};

#endif


#endif //__EFFECTENGINECTX_H
