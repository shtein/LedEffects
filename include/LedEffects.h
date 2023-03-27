#ifndef __LEDEFFECTS_H
#define __LEDEFFECTS_H

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
#else
  #ifdef MATRIX_WIDTH
    #undef MATRIX_WIDTH
  #endif
  #define MATRIX_WIDTH MAX_LEDS

  #ifdef MATRIX_HEIGHT
    #undef MATRIX_HEGHT
  #endif
  #define MATRIX_HEIGHT 1

  #ifdef MATRIX_TYPE
    #undef MATRIX_TYPE
  #endif
  #define MATRIX_TYPE xyMatrixStraight
#endif //USE_MATRIX

#ifndef MAX_LEDS 
  #error "MAX_LEDS is not defined"
#endif //MAX_LEDS

#if defined(ESP8266) || defined(ESP32)
//#define FASTLED_INTERRUPT_RETRY_COUNT 1
//#define FASTLED_ALLOW_INTERRUPTS 0
#endif 

#include <Fastled.h>

#endif //__LEDEFFECTS_H
