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
    uint16_t width() const { return W; };
    uint16_t height() const { return H; };
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
    int xy(int x, int y) const { return (x & 0x01) ? x * H + H - 1 - y :  x * H + y; } 
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
    int xy(int x, int y) const { return  x * H + y; } 
};


typedef XYMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> XY;          //Coordinate calcualtor for matrix 

#endif //__MATRIX_H