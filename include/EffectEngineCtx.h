#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H


//Speed delay
#define SPEED_DELAY_MIN 10
#define SPEED_DELAY_MAX 250

#define EEMC_LED         0x80   //Something changed - update LEDs

//On/Off command


//Engine commands
#define EEMC_EE                  0x20   //Generic effect engine
#define EEMC_GET_MODE            0x21   //Mode 
#define EEMC_GET_EFFECT          0x22   //Effect 
#define EEMC_GET_NUMLEDS         0x23   //Number of leds 
#define EEMC_GET_MODE_LIST       0x24   //Available modes
#define EEMC_GET_EFFECT_LIST     0x25   //Available effects
#define EEMC_GET_TRANSFORM_LIST  0x26   //Available pallete transforms
#define EEMC_GET_VERSION         0x27   //Get version
#define EEMC_GET_BRIGHTNESS      0x28   //Get brightness
#define EEMC_GET_STATE           0x29   //On/Off command

#define EEMC_MODE        (EEMC_LED | EEMC_GET_MODE)       //Mode changed
#define EEMC_EFFECT      (EEMC_LED | EEMC_GET_EFFECT)     //Effect changed
#define EEMC_NUMLEDS     (EEMC_LED | EEMC_GET_NUMLEDS)    //Number of leds changed
#define EEMC_BRIGHTNESS  (EEMC_LED | EEMC_GET_BRIGHTNESS) //Brightness changed
#define EEMC_STATE       (EEMC_LED | EEMC_GET_STATE)      //On/Off changed

//Effect commands
#define EEMC_GET_CFG        0x01   //Get effect config
#define EEMC_GET_COLOR      0x02   //Get effect RGB
#define EEMC_GET_SPEED      0x06   //Speed
#define EEMC_GET_TRANSFORM  0x07   //Palette transform rutine

#define EEMC_COLOR       (EEMC_LED | EEMC_GET_COLOR)      //Color changed
#define EEMC_SPEED       (EEMC_LED | EEMC_GET_SPEED)      //Speed changed
#define EEMC_TRANSFORM   (EEMC_LED | EEMC_GET_TRANSFORM)  //Palette transform

#ifdef USE_SOUND

enum SoundVUMType{
  sv_None = 0,
  sv_Symmetric,
  sv_Bar
};

#endif

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
DECLARE_STR_PROGMEM(rs_State)
DECLARE_STR_PROGMEM(rs_Brightness)
DECLARE_STR_PROGMEM(rs_Speed)
DECLARE_STR_PROGMEM(rs_Red)
DECLARE_STR_PROGMEM(rs_Green)
DECLARE_STR_PROGMEM(rs_Blue)
DECLARE_STR_PROGMEM(rs_RGB)
DECLARE_STR_PROGMEM(rs_Transforms)
DECLARE_STR_PROGMEM(rs_Transform)
DECLARE_STR_PROGMEM(rs_Flags)
DECLARE_STR_PROGMEM(rs_Cfg)
DECLARE_STR_PROGMEM(rs_Dsc)

#endif //NTF_ENABLED



#endif //__EFFECTENGINECTX_H
