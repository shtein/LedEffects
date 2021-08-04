#include "LEdEffects.h"

#include <FastLED.h>
#include "Matrix.h"




////////////////////////////////
// XYDraw

XYDraw::XYDraw(CRGB *leds, int16_t numLeds, uint8_t flags){
  _leds     = leds;
  _numLeds  = numLeds;
  _flags    = flags;
}



XYDraw::~XYDraw(){ 
}

void XYDraw::setFlags(uint8_t flags){
  _flags = flags;
}

uint8_t XYDraw::getFlags() const{
  return _flags;
}


CRGB & XYDraw::operator()(int16_t x, int16_t y){

  int16_t index = xy(x, y);

  if( x >= 0 && x < width() && 
      y >= 0 && y < height() && 
      index > 0 && index < _numLeds 
     )
    return _leds[index];
  else
    return _oob;
}

void XYDraw::pixel(int16_t x, int16_t y, const CRGB &col){
  
  if(_flags & XY_DRAW_ADD_COLORS){
    (*this)(x, y) += col;  
  }
  else{
    (*this)(x, y) = col;  
  }
}

void XYDraw::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col){

  int16_t dx = x1 - x0;
  int16_t dy = y1 - y0;
  if (abs(dx) >= abs(dy)){

    int32_t y = ((int32_t)y0 << 16) + 32768;

    if (!dx) {
      pixel(x0, y >> 16, col);
    } else {

      int32_t f = ((int32_t)dy << 16) / (int32_t)abs(dx);

      if (dx >= 0){

        for (; x0<=x1; ++x0,y+=f)
          pixel(x0, y >> 16, col);
      }
      else{
        
        for (; x0>=x1; --x0,y+=f)
          pixel(x0, y >> 16, col);
      }
    }
  }
  else{

    int32_t f = ((int32_t)dx << 16) / (int32_t)abs(dy);
    int32_t x = ((int32_t)x0 << 16) + 32768;

    if (dy >= 0){

      for (; y0<=y1; ++y0,x+=f)      
        pixel(x >> 16, y0, col);
    }
    else{

      for (; y0>=y1; --y0,x+=f)
        pixel(x >> 16, y0, col);
    }
  }
}

void XYDraw::bar(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col){
  line(x0, y0, x0, y1, col);
  line(x0, y1, x1, y1, col);
  line(x1, y1, x1, y0, col);
  line(x1, y0, x0, y0, col);
}

void XYDraw::fillBar(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col)
{
  int16_t y = min(y0, y1);
  for (int16_t c=abs(y1-y0); c>=0; --c,++y)
    line(x0, y, x1, y, col);
}

             
void XYDraw::circle(int16_t xc, int16_t yc, int16_t r, const CRGB &col){
  
  int16_t x = -r;
  int16_t y = 0;
  int16_t p = 2 - (2 * r);

  do
  {
    pixel(xc + x, yc - y, col);
    pixel(xc - x, yc + y, col);
    pixel(xc + y, yc + x, col);
    pixel(xc - y, yc - x, col);
    
    int16_t tmp = p;
    if (tmp <= y)
      p += (++y * 2) + 1;
      
    if (tmp > x || p > y)
      p += (++x * 2) + 1;
  }
  while (x < 0);
  
}

void XYDraw::fillCircle(int16_t xc, int16_t yc, int16_t r, const CRGB &col)
{
  int16_t x = r;
  int16_t y = 0;
  int16_t p = 1 - x;

  while (x >= y){

    line(xc + x, yc + y, xc - x, yc + y, col);
    line(xc + y, yc + x, xc - y, yc + x, col);
    line(xc - x, yc - y, xc + x, yc - y, col);
    line(xc - y, yc - x, xc + y, yc - x, col);

    ++y;
    if (p >= 0){
      --x;
      p += 2 * ((y - x) + 1);
    }
    else{
      p += (2 * y) + 1;
    }
  }
}