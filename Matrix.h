#ifndef __MATRIX_H
#define __MATRIX_H

//////////////////////////////////////////////////////////////
//Matrix 

//Types of matrixes
enum XYType{
  xyMatrixSerpent = 0,
  xyMatrixStraight,
};


/////////////////////
//Base matrix template class
template<const uint8_t W, const uint8_t H>
class XYMatrixBase{  
  public:  

    int16_t width() const { return W; };
    int16_t height() const { return H; };

    virtual  int16_t xy(int x, int y) const = 0;
    virtual  int x(int16_t index) const = 0;
    virtual  int y(int16_t index) const = 0;
};




///////////////////////
// Matrix template for matrix type, still abstract
template <const uint8_t W, const uint8_t H, XYType T>
class XYMatrix: public XYMatrixBase<W, H>{  
};

///////////////////////////
//      Serpent matrix
//
//     0 >  1 >  2 >  3 >  4
//                         |
//                         |
//     9 <  8 <  7 <  6 <  5
//     |
//     |
//    10 > 11 > 12 > 13 > 14
//                        |
//                        |
//    19 < 18 < 17 < 16 < 15

template <const uint8_t W, const uint8_t H>
class XYMatrix<W, H, xyMatrixSerpent>: public XYMatrixBase<W, H>{  
  public:
    int16_t xy(int x, int y) const { return (x & 0x01) ? x * H + H - 1 - y :  x * H + y; } 
    int x(int16_t index) const { return index / H; }
    int y(int16_t index) const { return (index / H & 0x01) ? H - 1 - index % H : index % H; };
};

//////////////////////////////
//      Straight matrix
//     0 >  1 >  2 >  3 >  4
//                         |
//     .----<----<----<----'
//     |
//     5 >  6 >  7 >  8 >  9
//                         |
//     .----<----<----<----'
//     |
//    10 > 11 > 12 > 13 > 14
//                         |
//     .----<----<----<----'
//     |
//    15 > 16 > 17 > 18 > 19

template <const uint8_t W, const uint8_t H>
class XYMatrix<W, H, xyMatrixStraight>: public XYMatrixBase<W, H>{  
  public:
    int16_t xy(int x, int y) const { return  x * H + y; } 
    int x(int16_t index) const { return index / H; }
    int y(int16_t index) const { return index % H; };
};


typedef XYMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> XY;          //Coordinate calcualtor for matrix 


///////////////////////////////
// XYDraw - drawing matrix

#define XY_DRAW_ADD_COLORS 0x01

class XYDraw: public XY{
public:
  XYDraw(CRGB *leds, int16_t numLeds, uint8_t flags = 0);
  ~XYDraw();

  void setFlags(uint8_t flags);
  uint8_t getFlags() const;

  CRGB &operator()(int16_t x, int16_t y);

  //Drawing functions
  void pixel(int16_t x, int16_t y, const CRGB &col);

  void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col);

  void circle(int16_t xc, int16_t yc, int16_t r, const CRGB &col);
  void fillCircle(int16_t xc, int16_t yc, int16_t r, const CRGB &col);
  
  void bar(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col);
  void fillBar(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB &col);

protected:
  CRGB   *_leds;      //leds array
  CRGB    _oob;       //out of boundary
  int16_t _numLeds;   //max leds
  uint8_t _flags;     //drawing options
};


#endif //__MATRIX_H