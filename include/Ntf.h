#ifndef __NTF_H
#define __NTF_H

#ifdef NTF_ENABLED

#include <Notification.h>

//////////////////////////////////////
// Set of notifiers
#ifndef MAX_NTF
  #define MAX_NTF 2
#endif //MAX_NTF

typedef NtfBaseSet<MAX_NTF> NtfSet;

///////////////////////////////////////
// Serialization for command responses
DECLARE_STR_PROGMEM(rs_Cmd)
DECLARE_STR_PROGMEM(rs_Data)
DECLARE_STR_PROGMEM(rs_Error)
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
  resp.put_F(rs_Cmd, r.cmd);
  resp.put_F(rs_Error, r.error);
}

//Default serialization
template<typename T>
void putNtfObject(NtfBase &resp, const EECmdResponse<T> &r){
  resp.put_F(rs_Cmd, r.cmd);
  resp.put_F(rs_Data, r.data);
}

#else 

typedef void *  NtfSet;

#endif //NTF_ENABLED

#endif //__NTF_H