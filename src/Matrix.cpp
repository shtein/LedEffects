#ifdef USE_MATRIX

#include "LEdEffects.h"
#include "Matrix.h"

////////////////////////////////
// XYDraw

XYDraw::XYDraw(CRGB *leds, uint16_t numLeds, uint8_t flags){
  _leds     = leds;
  _numLeds  = numLeds;
  _flags    = flags;
}

CRGB & XYDraw::operator()(int16_t x, int16_t y){
  int16_t index = xy(x, y);

  if( x >= 0 && x < width() && 
      y >= 0 && y < height() && 
      index >= 0 && index < (int16_t)_numLeds 
     )
    return _leds[index];
  else
    return _oob;
}

const CRGB & XYDraw::operator()(int16_t x, int16_t y) const{
  int16_t index = xy(x, y);

  if( x >= 0 && x < width() && 
      y >= 0 && y < height() && 
      index >= 0 && index < (int16_t)_numLeds 
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

void XYDraw::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col) {
  //Bresenham's line algorithm
  int16_t dx = abs(x1 - x0);
  int16_t dy = -abs(y1 - y0);
  int16_t sx = x0 < x1 ? 1 : -1;
  int16_t sy = y0 < y1 ? 1 : -1;
  int16_t err = dx + dy;  // error value e_xy 
  int16_t e2;

  for(;;){  

    pixel(x0, y0, col);

    if (x0 == x1 && y0 == y1) 
      break;
      
    e2 = 2 * err;
    if (e2 >= dy) { // e_xy+e_x > 0 
      err += dy;
      x0 += sx;
    }
    
    if (e2 <= dx) { // e_xy+e_y < 0 
      err += dx;
      y0 += sy;
    }
  }

}

void XYDraw::rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col){
  SWAPIF(x0, x1);
  SWAPIF(y0, y1);

  //Horizontal
  for(int x = x0; x <= x1; x++){
    pixel(x, y0, col);
    pixel(x, y1, col);
  }

  //Vertical
  for(int y = y0 + 1; y <= y1 - 1; y++){
    pixel(x0, y, col);
    pixel(x1, y, col);
  }
}

void XYDraw::fillRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col)
{
  SWAPIF(x0, x1);
  SWAPIF(y0, y1);

  for(int16_t y = y0; y <= y1; y++){
    for(int16_t x = x0; x<= x1; x++){
      pixel(x, y, col);
    }
  }
}

void XYDraw::fadeToBlackRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t fade)
{
  SWAPIF(x0, x1);
  SWAPIF(y0, y1);

  for(int16_t y = y0; y <= y1; y++){
    for(int16_t x = x0; x<= x1; x++){
      (*this)(x, y).fadeToBlackBy(fade);
    }
  }
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

void XYDraw::fillCircle(int16_t xc, int16_t yc, int16_t r, const CRGB &col){
  
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




//Coordinate system is move to (mx, my)
//Each point of the rectangle is translated to new system
//Reflected point is created as (-x, -y) and translaed back to original coordinate system

void XYDraw::mirrorRectangle(int16_t x0, int16_t y0, 
                             uint16_t width, uint16_t height, 
                             int16_t mx, int16_t my,
                             int16_t shiftx, int16_t shifty
                            ){
  for(int16_t x = x0; x < (int16_t)width; x++){
    for(int16_t y = y0; y < (int16_t)height; y++){
      (*this)(MIRROR(x, mx) + shiftx, MIRROR(y, my) + shifty) = (*this)(x, y);
    }
  }
}

void XYDraw::mirrorRectangleHorizontally(int16_t x0, int16_t y0, 
                                         uint16_t width, uint16_t height, 
                                         int16_t mx, 
                                         int16_t shiftx, int16_t shifty){
  for(int16_t x = x0; x < (int16_t)width; x++){
    for(int16_t y = y0; y < (int16_t)height; y++){
      (*this)(MIRROR(x, mx) + shiftx, y + shifty) = (*this)(x, y);
    }
  }
}


void XYDraw::mirrorRectangleVertically(int16_t x0, int16_t y0, 
                                       uint16_t width, uint16_t height, 
                                       int16_t my,
                                       int16_t shiftx, int16_t shifty){
  for(int16_t x = x0; x < (int16_t)width; x++){
    for(int16_t y = y0; y < (int16_t)height; y++){
      (*this)(x + shiftx, MIRROR(y, my) + shifty) = (*this)(x, y);
    }
  }
}


////////////////////////////////////
//Right rectanle shape helper
class RightTrianglePoints{
public:

  inline RightTrianglePoints(int8_t x, int8_t y){

    if(abs(x) >= abs(y)){
      _revert = false;
      _dx = x;
      _dy = y;
    }
    else{
       _revert = true;
      _dx = y;
      _dy = x;
    }

    _p = 0;

    _xs = _dx >= 0 ? 1: -1;
    _ys = _dy >= 0 ? 1: -1;

    _dx = _dx == 0? _dx : _dx - _xs;
    _dy = _dy == 0? _dy : _dy - _ys;

    //Init values    
    int32_t h = (int32_t)_dy << 16 ;
    _a  = _dx != 0 ? -h / (int32_t)_dx : 0; 
    _b  =  h;
  
  };

  inline void first(int8_t &x, int8_t &y){
    _p =  (int16_t)((_b + 32768) >> 16);

    x = 0;
    y = 0;
  };

  inline bool next(int8_t &x, int8_t &y){

    //Check if y is moving coordinate
    if(_revert){
      int8_t tmp = x;
      x = y;
      y = tmp;
    }

    //Move y
    y += _ys;
    if(abs(y) > abs(_p) ){ //Move x
      x+= _xs;
      if(abs(x) > abs(_dx) ){ //Done
        return false;
      }
      else{
        //Recalculate point on Hypotenuse
        _p =  (int8_t)((_a * (int32_t)x + _b + 32768) >> 16);
        y = 0;
      }
    }

    //Check if y is moving coordinate
    if(_revert){
      int8_t tmp = x;
      x = y;
      y = tmp;
    }

    return true;
  };


protected:
  int8_t _dx;  //x coordinate relative to 0
  int8_t _dy;  //y coordinate relative to 0
  int8_t _p;   //current y on the Hypotenuse

  //Hypotenuse is defined as y(x) = -height/width * x + (y0 + height + x0 * heigh / width )
  //Use 32-bit in y scale for higher precision
  int32_t _a;
  int32_t _b;

  //Steps
  int8_t _xs;
  int8_t _ys;

  //x or y moving variable true if dx < dy
  bool _revert; 
};

//Width and hight are number of dots including x0, y0
//Hypotenuse is defined as y = -height/width * x + (y0 + height + x0 * heigh / width )
//Use 32-bit in y scale for higher precision

void XYDraw::fillRightTriangle(int16_t x0, int16_t y0,
                               int16_t dx, int16_t dy,
                               const CRGB & col
                             ){

  RightTrianglePoints en(dx, dy);

  int8_t x, y;
  en.first(x, y); 
  do {

    (*this)(x + x0, y + y0) = col; //CRGB(map(x, 0, dx, 0, 255), map(x, 0, dy, 0, 255), 127); //col + CRGB(x * 10, 0, 0);
  } while(en.next(x, y));

}                 

void XYDraw::fadeToBlackRightTriangle(int16_t x0, int16_t y0,
                                    int16_t dx, int16_t dy,
                                    uint8_t fade
                                   ){

  RightTrianglePoints en(dx, dy);

  int8_t x, y;
  en.first(x, y); 
  do {

    (*this)(x + x0, y + y0).fadeToBlackBy(fade);
  } while(en.next(x, y));

}
  

void XYDraw::mirrorRightTriangle(int16_t x0, int16_t y0,
                                 int16_t dx, int16_t dy,
                                 int16_t mx, int16_t my,   
                                 int16_t shiftx, 
                                 int16_t shifty
                                ){


  RightTrianglePoints en(dx, dy);

  int8_t x, y;
  en.first(x, y); 
  do {

    (*this)(MIRROR(x + x0, mx) + shiftx, MIRROR(y + y0, my) + shifty) = (*this)(x + y0, y + y0);
  } while(en.next(x, y));

}      


void XYDraw::mirrorRightTriangleHorizontally( int16_t x0, int16_t y0,
                                              int16_t dx, int16_t dy,
                                              int16_t mx,   
                                              int16_t shiftx, 
                                              int16_t shifty
                                            ){

  RightTrianglePoints en(dx, dy);

  int8_t x, y;
  en.first(x, y); 
  do {

    (*this)(MIRROR(x + x0, mx) + shiftx, y + y0 + shifty) = (*this)(x + x0, y + y0);
  } while(en.next(x, y));

}                                                                                             


void XYDraw::mirrorRightTriangleVertically( int16_t x0, int16_t y0,
                                            int16_t dx, int16_t dy,
                                            int16_t my,   
                                            int16_t shiftx, 
                                            int16_t shifty
                                          ){

  RightTrianglePoints en(dx, dy);

  int8_t x, y;
  en.first(x, y); 
  do {

    (*this)(x + x0 + shiftx, MIRROR(y + y0, my) + shifty) = (*this)(x + x0, y + y0);
  } while(en.next(x, y));
                                             
}      


void XYDraw::mirrorRightTriangleButterfly(int16_t x0, int16_t y0,  
                                          int16_t dx, int16_t dy, 
                                          int16_t shiftx, 
                                          int16_t shifty ){

  RightTrianglePoints en(dx, dy);

  int8_t k = dx * dy > 0 ? -1 : 1;

  int8_t x, y;
  en.first(x, y); 
  do {  
    (*this)(k * y + x0 + shiftx, k * x + y0 + shifty ) = (*this)(x + x0, y + y0);  

  } while(en.next(x, y));

}


///////////////////////////////////////
// Kaleidoscope
void kaleidoscope(CRGB *leds, uint16_t numLeds){        
  XYDraw xy(leds, numLeds);

  xy.mirrorRightTriangleButterfly(0, xy.height() / 2 - 1, 
                                  xy.width() / 2, -xy.height() / 2,
                                  xy.width() / 2 - 1, -xy.height() / 2 + 1
                                );
  xy.mirrorRectangleHorizontally(0, 0, xy.width() / 2, xy.height() / 2, xy.width() / 2, xy.width() % 2 - 1);
  xy.mirrorRectangleVertically(0, 0, xy.width(), xy.height() / 2, xy.height() / 2, 0, xy.height() % 2 - 1);
};



#endif //USE_MATRIX