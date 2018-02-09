#ifndef __DBGTOOL_H
#define __DBGTOOL_H

#include <wstring.h>



#ifdef DBG_TO_SERIAL

  #define DBG_USE_FLASH
  void _dbgOut(const char *fmt, ...);
  void _dbgOut(const __FlashStringHelper *fmt, ...);

  #define DBG_INIT()      Serial.begin(57600)  

  #ifdef DBG_USE_FLASH
    #define DBG_OUT(FORMAT, ...)  _dbgOut(F(FORMAT), ##  __VA_ARGS__);   
  #else
    #define DBG_OUT(FORMAT, ...)  _dbgOut(FORMAT, ##  __VA_ARGS__);   
  #endif
  
  #define DBG_OUTLN(FORMAT, ...)  DBG_OUT(FORMAT "\n", ##  __VA_ARGS__); 
  
#else
  #define DBG_INIT() 
  #define DBG_OUT(...)
  #define DBG_OUTLN(...)
#endif




#endif //__DBGTOOL_H
