#ifndef __NTF_H
#define __NTF_H

#include <Notification.h>

//////////////////////////////////////
// Set of notifiers
#ifndef MAX_NTF
  #define MAX_NTF 2
#endif //MAX_NTF

typedef NtfBaseSet<MAX_NTF> NtfSet;

///////////////////////////////////////
// Serialization for command responses

template <typename ...Ts>
struct EECmdResponse{};

template<>
struct EECmdResponse<>{
  uint8_t cmd;
  uint8_t error;
};

template<typename T> 
struct EECmdResponse<T>{
  uint8_t cmd;
  T data;
}; 

inline void putNtfObject(NtfBase &resp, const EECmdResponse<> &r){
  resp.put_F(F("cmd"), r.cmd);
  resp.put_F(F("error"), r.error);
}

//Default serialization
template<typename T>
void putNtfObject(NtfBase &resp, const EECmdResponse<T> &r){
  resp.put_F(F("cmd"), r.cmd);
  resp.put_F(F("data"), r.data);
}


//Getting/setting mode or effect - EEMC_MODE, EEMC_EFFECT...
struct EEResp_ModeEffect{
  uint8_t mode;
  uint8_t effect;
};

inline void putNtfObject(NtfBase &resp, const EEResp_ModeEffect &data){
   resp.put_F(F("mode"), data.mode);
   resp.put_F(F("effect"), data.effect);
}

//Getting/setting numbleds - EEMC_NUMLEDS
struct EEResp_NumLeds{  
  uint16_t maxLeds;
  uint16_t numLeds;
};

inline void putNtfObject(NtfBase &resp, const EEResp_NumLeds &data){
  resp.put_F(F("maxLeds"), data.maxLeds);
  resp.put_F(F("numLeds"), data.numLeds);
}


//Getting/setting effect speed
struct EEResp_EffectSpeed{  
  uint16_t speed;
};

inline void putNtfObject(NtfBase &resp, const EEResp_EffectSpeed &data){
  resp.put_F(F("speed"), data.speed);
}

//Static effect color
struct EEResp_Color{
  CHSV hsv;
}; 

inline void putNtfObject(NtfBase &resp, const CHSV &data){
  resp.put_F(F("hue"), data.h);
  resp.put_F(F("saturation"), data.s);
  resp.put_F(F("value"), data.v);
}

inline void putNtfObject(NtfBase &resp, const EEResp_Color &data){
  resp.put_F(F("hsv"), data.hsv);
}


#endif //__NTF_H