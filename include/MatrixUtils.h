#ifndef __MATRIXUTILS_H
#define __MATRIXUTILS_H

//////////////////////////////
// Mirror
#define MIRROR(p, m) (2 * (m) - (p))

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
  inline Pnt(int8_t x0, int8_t y0) __attribute__((always_inline)): x(x0), y(y0)
  {
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


#endif //__MATRIXUTILS_H