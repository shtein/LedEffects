#ifndef __RESP_H
#define __RESP_H


///////////////////////
//Nofification structures
struct __attribute__((packed)) EEResp_Version{  
};

struct __attribute__((packed)) EEResp_State{  
  uint8_t state;
};

struct __attribute__((packed)) EEResp_Brightness{  
  uint8_t brightness;
};


struct __attribute__((packed)) EEResp_NumLeds{  
  uint16_t maxLeds;
  uint16_t numLeds;
};

struct __attribute__((packed)) EEResp_EffectList{
};

struct __attribute__((packed)) EEResp_TransformList{
};

struct __attribute__((packed))  EEResp_ModeList{
  uint8_t     numModes;
};

struct __attribute__((packed))  EEResp_Mode{
  uint8_t             modeNum;
  EFFECT_MODE_CONFIG  cfgMode;
};

struct  __attribute__((packed)) EEResp_Effect{
  uint8_t            effectNum;
  EFFECT_CONFIG      cfg;
};

struct __attribute__((packed)) EEResp_EffectTransform{  
  uint8_t transform;
};

struct __attribute__((packed)) EEResp_EffectColor{
  uint8_t rgb[3];
};

struct __attribute__((packed)) EEResp_EffectSpeed{  
  uint16_t speed;
};



#ifdef NTF_ENABLED

void putNtfObject(NtfBase &resp, const EEResp_Version &data);
void putNtfObject(NtfBase &resp, const EEResp_NumLeds &data);
void putNtfObject(NtfBase &resp, const EEResp_TransformList &data);
void putNtfObject(NtfBase &resp, const EEResp_EffectList &data);
void putNtfObject(NtfBase &resp, const EEResp_ModeList &data);
void putNtfObject(NtfBase &resp, const EEResp_Mode &data);
void putNtfObject(NtfBase &resp, const EEResp_Effect &data);
void putNtfObject(NtfBase &resp, const EEResp_EffectColor &data);
void putNtfObject(NtfBase &resp, const EEResp_EffectTransform &data);
void putNtfObject(NtfBase &resp, const EEResp_EffectSpeed &data);

#endif //NTF_ENABLED




#endif //__RESP_H