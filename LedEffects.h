#ifndef __LEDEFFECTS_H
#define __LEDEFFECTS_H

//#define CHECK_MEM
//#define __OUTAGE_LAMP_SETUP
//#define __SASHA_SETUP



//To enable debug log via serail or IR Remote when compile in Arduino IDE, use arduinolib.h 

#include <Arduino.h>
#include <arduinolib.h>
#include <DbgTool.h>


#ifdef USE_MATRIX
  #ifndef MATRIX_WIDTH
    #error "MATRIX_WIDTH is not defined"
  #endif //MATRIX_WIDTH

  #ifndef MATRIX_HEIGHT
    #error "MATRIX_HEIGHT is not defined"
  #endif //MATRIX_HEIGHT

  #ifdef MAX_LEDS
     #undef MAX_LEDS
  #endif //MAX_LEDS
  #define MAX_LEDS MATRIX_WIDTH * MATRIX_HEIGHT

  #ifndef MATRIX_TYPE
    #error "MATRIX_TYPE is not defined"
  #endif //MATRIX_TYPE

#endif //USE_MATRIX

#ifndef MAX_LEDS 
  #error "MAX_LEDS is not defined"
#endif


#endif //__LEDEFFECTS_H
