#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H


//Data limits
#ifndef MAX_EFFECTS
  #define MAX_EFFECTS 15
#endif //MAX_EFFECTS

#ifndef MAX_MODES
  #define MAX_MODES 5
#endif //MAX_MODES


//Speed delay
#define SPEED_DELAY_MIN 10
#define SPEED_DELAY_MAX 250

#define EEMC_LED         0x80   //Something changed - update LEDs

//Engine commands
#define EEMC_EE             0x20   //Generic effect engine
#define EEMC_GET_MODE       0x21   //Mode 
#define EEMC_GET_EFFECT     0x22   //Effect 
#define EEMC_GET_NUMLEDS    0x23   //Number of leds 
#define EEMC_GET_MODE_LIST  0x24   //Available modes

#define EEMC_MODE        (EEMC_LED | EEMC_GET_MODE)       //Mode changed
#define EEMC_EFFECT      (EEMC_LED | EEMC_GET_EFFECT)     //Effect changed
#define EEMC_NUMLEDS     (EEMC_LED | EEMC_GET_NUMLEDS)    //Number of leds changed


//Effect commands
#define EEMC_GET_COLOR_HSV  0x01   //Get effect HSV
#define EEMC_GET_COLOR_HUE  0x02   //Color hue 
#define EEMC_GET_COLOR_SAT  0x03   //Color saturation 
#define EEMC_GET_COLOR_VAL  0x04   //Color value 
#define EEMC_GET_SPEED      0x05   //Speed changed

#define EEMC_COLOR_HUE   (EEMC_LED | EEMC_GET_COLOR_HUE)  //Color hue changed
#define EEMC_COLOR_SAT   (EEMC_LED | EEMC_GET_COLOR_SAT)  //Color saturation changed
#define EEMC_COLOR_VAL   (EEMC_LED | EEMC_GET_COLOR_VAL)  //Color value changed   
#define EEMC_SPEED       (EEMC_LED | EEMC_GET_SPEED)      //Speed changed

#if defined(ESP8266) || defined(ESP32)
  //Wifi control
  #define EEMC_WIFI             0x40    //Wifi commands
  #define EEMC_WIFI_STATUS      0x40    //WIFI status
  #define EEMC_WIFI_SCAN        0x41    //WIFI status
  #define EEMC_WIFI_AP_ON       0x42    //Enable AP
  #define EEMC_WIFI_AP_OFF      0x43    //Disable AP
  #define EEMC_WIFI_CONNECT     0x44    //Connect WIFI
  #define EEMC_WIFI_DISCONNECT  0x45    //Diconnect WIFI
  #define EEMC_WIFI_CFG_GET     0x46    //Get WIFI configuration
  #define EEMC_WIFI_CFG_SET     0x47    //Set WIFI configuration

#endif

//Effect sound commands
#define EEMC_GET_SOUND_LOW          0x10   //Lower 
#define EEMC_GET_SOUND_HIGH         0x11   //Upper
#define EEMC_GET_SOUND_SENSITIVITY  0x12   //Sensitiviy
#define EEMC_GET_SOUND_USE_MAX      0x13   //Min lower
#define EEMC_GET_SOUND_USE_MIN      0x14   //Max upper
#define EEMC_GET_SOUND_LOG          0x15   //Log scale

#define EEMC_SOUND_LOW          (EEMC_LED | EEMC_GET_SOUND_LOW)          //Change lower 
#define EEMC_SOUND_HIGH         (EEMC_LED | EEMC_GET_SOUND_HIGH)         //Change upper
#define EEMC_SOUND_SENSITIVITY  (EEMC_LED | EEMC_GET_SOUND_SENSITIVITY)  //Enable sensitiviy
#define EEMC_SOUND_USE_MAX      (EEMC_LED | EEMC_GET_SOUND_USE_MAX)      //Use min lower
#define EEMC_SOUND_USE_MIN      (EEMC_LED | EEMC_GET_SOUND_USE_MIN)      //Use max upper
#define EEMC_SOUND_LOG          (EEMC_LED | EEMC_GET_SOUND_LOG)          //Show audio in log scale

//Errors
#define EEER_SUCCESS    0x00 //No error
#define EEER_INVALID    0x01 //Invalid input
#define EEER_UNHANDLED  0x02 //Unhandled command
#define EEER_DELAYED    0x03 //Delayed response

//Timeout to save config
#define SAVE_CONFIG_TIMEOUT 30000

//Config parameters
#define EE_VERSION 0x04 //Config version

//Config storage blocks
#define EE_ENGINE_IDX       0     //Effect Engine index

#if defined(ESP8266) || defined(ESP32)
  #define EE_WIFI_IDX         128  //WiFi configuration
  #define EE_EFFECT_IDX       256   //First Effect index
#else   
  #define EE_EFFECT_IDX       128   //First Effect index
#endif

#define EE_EFFECT_IDX_SIZE  8     //8 bytes per effect



#ifdef NTF_ENABLED
#include "Ntf.h"

////////////////////////////
// Override CtrlQueueItem structure to enable extension
// Ext can be reference to a structure, class or anything
struct CtrlQueueItemEx: public CtrlQueueItem{

  CtrlQueueItemEx(NtfSet &n): ntf(n) {
  }
  
  NtfSet &ntf;
};
#else 

struct CtrlQueueItemEx: public CtrlQueueItem{};

#endif //NTF_ENABLED


#endif //__EFFECTENGINECTX_H
