#include "LEdEffects.h"

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
      index >= 0 && index < _numLeds 
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

void XYDraw::rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col){
  line(x0, y0, x0, y1, col);
  line(x0, y1, x1, y1, col);
  line(x1, y1, x1, y0, col);
  line(x1, y0, x0, y0, col);
}

void XYDraw::fillRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col)
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
    _a  = _dx != 0? -h / (int32_t)_dx : 0; 
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
    y+= _ys;
    if(abs(y) > abs(_p) ){ //Move x
      x+= _xs;
      if(abs(x) > abs(_dx) ){ //Done
        return false;
      }
      else{
        //Recalculate point on Hypotenuse
        _p =  (int16_t)((_a * (int32_t)x + _b + 32768) >> 16);
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

  int8_t _x;   //current x
  int8_t _y;   //current y
  int8_t _p;   //current on the Hypotenuse

  //Hypotenuse is defined as y = -height/width * x + (y0 + height + x0 * heigh / width )
  //Use 32-bit in y scale for higher precision
  int32_t _a;
  int32_t _b;

  //Steps
  int8_t _xs;
  int8_t _ys;

  //x or y moving variable true if y
  bool _revert; 

};

//Widht and hight are number of dots including x0, y0
//Hypotenuse is defined as y = -height/width * x + (y0 + height + x0 * heigh / width )
//Use 32-bit in y scale for higher precision

void XYDraw::rightTriangle(int16_t x0, int16_t y0,
                           int16_t dx, int16_t dy,
                           const CRGB & col
                          ){

  RightTrianglePoints en(dx, dy);

  int8_t x, y;
  en.first(x, y); 
  do {

    (*this)(x + x0, y + y0) = CRGB(map(x, 0, dx, 0, 255), map(x, 0, dy, 0,255), 127); //col + CRGB(x * 10, 0, 0);
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
  xy.mirrorRectangleHorizontally(0, 0, xy.width()/2, xy.height()/2, xy.width()/2, xy.width() % 2 - 1);
  xy.mirrorRectangleVertically(0, 0, xy.width(), xy.height() / 2, xy.height() / 2, 0, xy.height() % 2 - 1);
};
