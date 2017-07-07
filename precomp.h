#ifndef __PRECOMP_H
#define __PRECOMP_H

#include "Arduino.h"
#include "FastLED.h"

//#define LEDBG

#ifdef LEDBG
  #define DBG_INIT()      Serial.begin(57600)  
  #define DBG_OUT(...)    Serial.print(__VA_ARGS__)
  #define DBG_OUTLN(...)  Serial.println(__VA_ARGS__)
#else
  #define DBG_INIT() 
  #define DBG_OUT(...)
  #define DBG_OUTLN(...)
#endif


#endif //__PRECOMP_H
