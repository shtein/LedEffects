#ifndef __MATRIXUTILS_H
#define __MATRIXUTILS_H

//////////////////////////////
// Swap
template <typename T>
constexpr void swapIf(T &a, T &b) {
    if (a > b) {
        T c = a;
        a = b;
        b = c;
    }
}

#define SWAPIF(a, b) swapIf(a, b)

template <typename T>
uint8_t random8_ab(T a, T b){
  swapIf(a, b);

  return random8(a, b + 1);
}

#define RANDOM8_AB(a, b) random8_ab((int8_t)(a), (int8_t)(b))
  
//////////////////////////////
// Mirror

constexpr int mirror(int p, int m) {
  return 2 * m - p;
}

#define MIRROR(p, m) mirror(p, m)

///////////////////////////////////////
// Point
template <typename T>
struct Pnt {
  T x;
  T y;

  
  inline Pnt() __attribute__((always_inline)){
    x = 0;
    y = 0;
  };
  
  inline Pnt(T x0, T y0) __attribute__((always_inline)): x(x0), y(y0){
  };

  inline Pnt& operator= (const Pnt& pnt) __attribute__((always_inline)) = default;

  inline Pnt mirror(const Pnt &m) const __attribute__((always_inline)) {
    return Pnt( MIRROR(x, m.x), MIRROR(y, m.y));
  }  

  inline Pnt ortogonal() const {
    return Pnt(y, -x);
  }

};

template <typename T>
inline Pnt<T> operator+ (const Pnt<T>& pnt1, const Pnt<T>& pnt2)  {
  return Pnt<T>(pnt1.x + pnt2.x, pnt1.y + pnt2.y);
};

template <typename T>
inline Pnt<T> operator- (const Pnt<T>& pnt1, const Pnt<T>& pnt2){
  return Pnt<T>(pnt1.x - pnt2.x, pnt1.y - pnt2.y);
};

template <typename T>
inline int16_t operator* (const Pnt<T>& pnt1, const Pnt<T>& pnt2){
  return pnt1.x * pnt2.x + pnt1.y * pnt2.y;
}


typedef Pnt<int8_t>  Pnt8_t;
typedef Pnt<int16_t> Pnt16_t;
typedef Pnt<int32_t> Pnt32_t;



///////////////////////////////////////
// Object
template <typename T>
struct Obj {
  Pnt<T> pos;
  Pnt<T> vel;

  inline Obj()  __attribute__((always_inline)) = default;
  inline Obj (const Pnt<T> &pos0, const Pnt<T> &vel0)  __attribute__((always_inline)): 
    pos(pos0), vel(vel0){
  }
  inline Obj (T x0, T y0, T vx0, T vy0)  __attribute__((always_inline)):
    pos(x0, y0), vel(vx0, vy0)
  {
  }

  inline Obj& operator= (const Obj &)  __attribute__((always_inline)) = default;
 
  inline bool movesAwayLeft(T x) const __attribute__((always_inline)) {
    if(pos.x > x || vel * Pnt<T>(1, 0) >= 0)
      return false;
      
    return true;
  }

   inline bool movesAwayRight(T x) const __attribute__((always_inline)) {
    if(pos.x < x || vel * Pnt<T>(-1, 0) >= 0)
      return false;

    return true;
  }

  inline bool movesAwayUp(T y) const __attribute__((always_inline)) {
    if(pos.y > y || vel * Pnt<T>(0, 1) >= 0)
      return false;
      
    return true;
  }

  inline bool movesAwayDown(T y) const __attribute__((always_inline)) {
    if(pos.y < y || vel * Pnt<T>(0, -1) >= 0)
      return false;

    return true;
  }
};

typedef Obj<int8_t>  Obj8_t;
typedef Obj<int16_t> Obj16_t;
typedef Obj<int32_t> Obj32_t;


//////////////////////////////////////
//Segment
template <typename T>
struct Segment{
  Pnt<T> p1;
  Pnt<T> p2;

  inline Segment()  __attribute__((always_inline)) = default;
  inline Segment (const Pnt<T> &q1, const Pnt<T> &q2)  __attribute__((always_inline)): 
    p1(q1), p2(q2){
  };
  inline Segment (T x1, T y1, T x2, T y2)  __attribute__((always_inline)):
    p1(x1, y1), p2(x2, y2){
  };

  inline Segment& operator= (const Segment &)  __attribute__((always_inline)) = default;

  inline Pnt<T> normal() const{
    return Pnt<T>(p2.y - p1.y, p1.x - p2.x);
  }
};


typedef Segment<int8_t>  Segment8_t;
typedef Segment<int16_t> Segment16_t;
typedef Segment<int32_t> Segment32_t;


//////////////////////////////////////
//Right Triangle
template <typename T>
struct RightTriangle{
  T x;      //X of the right angle
  T y;      //Y of the right angle
  T dx;     //Width - negative for x meanus right angle is on the right
  T dy;     //Height - negative for y means right angle is on the bottom

  inline RightTriangle()  __attribute__((always_inline)) = default;

  inline RightTriangle (T cx, T cy, T w, T h)  __attribute__((always_inline)):
    x(cx), y(cy), dx(w > 0 ? w - 1 : w  + 1), dy(h > 0 ? h - 1 : h + 1){
  };

  inline RightTriangle& operator= (const RightTriangle &)  __attribute__((always_inline)) = default;

  inline T width() const __attribute__((always_inline)) {
    return dx > 0 ? dx + 1 : dx - 1;
  }

  inline T height() const __attribute__((always_inline)) {
    return dy > 0 ? dy + 1: dy - 1;
  }

  inline bool topSided() const __attribute__((always_inline)) {
    return dy > 0;
  }
  inline bool bottomSided() const __attribute__((always_inline)) {
    return dy < 0;
  }
  inline bool leftSided() const __attribute__((always_inline)) {
    return dx > 0;
  }

  inline bool rightSided() const __attribute__((always_inline)) {
    return dx < 0;
  }
  
  inline T cornerX() const __attribute__((always_inline)) {  
    return  x + dx;
  }

  inline T cornerY() const __attribute__((always_inline)) {  
    return  y + dy;
  }

  inline T hypotenuseX(T ry) const __attribute__((always_inline)) {
    return -(ry - cornerY()) * dx / dy + x;
  }

  inline T hypotenuseY(T rx) const __attribute__((always_inline)) {
    return -(rx - cornerX()) * dy / dx + y;
  }  

  inline void randomPointY(T &rx, T &ry, T offsC = 0, T offsH = 0) const __attribute__((always_inline)){
    rx = RANDOM8_AB(x + (leftSided() ? offsC : - offsC), cornerX() + (leftSided() ? - offsH : offsH));  
    ry = RANDOM8_AB(hypotenuseY(rx), y);
  }

};

typedef RightTriangle<int8_t>  RightTriangle8_t;
typedef RightTriangle<int16_t> RightTriangle16_t;
typedef RightTriangle<int32_t> RightTriangle32_t;


#endif //__MATRIXUTILS_H