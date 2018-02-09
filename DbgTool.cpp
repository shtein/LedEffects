#include "dbgtool.h"
#include <stdio.h>
#include <stdarg.h>
#include <arduino.h>


#define BUF_SIZE 64

void _dbgOut(const char *fmt, ...){
  char buf[BUF_SIZE];
  va_list args;
  
  va_start (args, fmt );  
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end (args);
  
  Serial.print(buf);
}

void _dbgOut(const __FlashStringHelper *fmt, ...){
  char buf[BUF_SIZE];
  va_list args;
  
  va_start (args, fmt);
  vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); 
  va_end(args);
  
  Serial.print(buf);
}
