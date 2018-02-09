#ifndef __PRECOMP_H
#define __PRECOMP_H

#include "Arduino.h"
#include "FastLED.h"

//#define DBG_TO_SERIAL
#include "dbgtool.h"



#define CHECK_MEM
#ifdef CHECK_MEM 
  #define _CM static
#else
  #define _CM
#endif
  

#endif //__PRECOMP_H
