#ifndef __PRECOMP_H
#define __PRECOMP_H

#include <Arduino.h>
#include <FastLED.h>

#define DBG_TO_SERIAL
#include <DbgTool.h>

#define CHECK_MEM
#ifdef CHECK_MEM 
  #define _CM static
#else
  #define _CM
#endif


#define qsuba(x, b)  ((x > b) ? x - b : 0) 


#endif //__PRECOMP_H
