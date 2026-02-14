#ifndef __RESP_H
#define __RESP_H

//Notification responses


///////////////////////
//Nofifications
struct __attribute__((packed)) EEResp_Version{  
};

struct __attribute__((packed)) EEResp_EffectSpeed{  
  uint16_t speed;
};

struct __attribute__((packed)) EEResp_EffectList{
};

struct __attribute__((packed)) EEResp_EffectTransform{  
  uint8_t transform;
};

struct __attribute__((packed)) EEResp_TransformList{
};

struct __attribute__((packed))  EEResp_ModeList{
  uint8_t     numModes;
};

struct __attribute__((packed)) EEResp_NumLeds{  
  uint16_t maxLeds;
  uint16_t numLeds;
};

struct __attribute__((packed))  EEResp_Mode{
  uint8_t             modeNum;
  EFFECT_MODE_CONFIG  cfgMode;
};

struct  __attribute__((packed)) EEResp_Effect{
  uint8_t            effectNum;
  EFFECT_CONFIG      cfg;
};




#endif //__RESP_H