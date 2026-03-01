#ifndef __MATRIXUTILS_H
#define __MATRIXUTILS_H

#include <alutils.h>

//////////////////////////////
// Mirror
template<typename T>
inline constexpr T mirror(T p, T m) {
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

  inline Pnt ortogonal() const  __attribute__((always_inline)) {
    return Pnt(y, -x);
  }

  inline bool operator== (const Pnt &pnt) const __attribute__((always_inline)) {
    return (x == pnt.x) && (y == pnt.y);
  }

  inline bool operator!= (const Pnt &pnt) const __attribute__((always_inline)) {
    return (x != pnt.x) || (y != pnt.y);
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
inline upper_type_t<T> operator* (const Pnt<T>& pnt1, const Pnt<T>& pnt2){
  return (upper_type_t<T>)pnt1.x * (upper_type_t<T>)pnt2.x + (upper_type_t<T>)pnt1.y * (upper_type_t<T>)pnt2.y;
}

template <typename T>
inline Pnt<T> operator* (const Pnt<T>& pnt, T v){
  return Pnt<T>(pnt.x * v, pnt.y * v);
}

template<typename T>
inline Pnt<T> operator* ( T v, const Pnt<T>& pnt){
  return pnt * v;
}


template <typename T>
inline Pnt<T> operator / (const Pnt<T>& pnt, T v){
  using U = upper_type_t<T>;
  return Pnt<T>((T)divRound<U>((U)pnt.x, (U)v), (T)divRound<U>((U)pnt.y, (U)v));
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
    if(pos.x > x || vel.x >= 0)
      return false;
      
    return true;
  }

   inline bool movesAwayRight(T x) const __attribute__((always_inline)) {
    if(pos.x < x || vel.x <= 0)
      return false;

    return true;
  }

  inline bool movesAwayUp(T y) const __attribute__((always_inline)) {
    if(pos.y > y || vel.y >= 0)
      return false;
      
    return true;
  }

  inline bool movesAwayDown(T y) const __attribute__((always_inline)) {
    if(pos.y < y || vel.y <= 0)
      return false;

    return true;
  }

  inline bool collides(const Obj<T> &obj, T dist) const __attribute__((always_inline)) {
    Pnt<T> posRel = obj.pos - pos;
    Pnt<T> velRel = obj.vel - vel;

    //If the same coordinates, move relaive coordintes a bit according to relative velocity
    if(posRel.x == 0 && posRel.y == 0){
      posRel.x = (velRel.x > 0) ? -dist/2 : (velRel.x < 0) ? dist/2 : 0;
      posRel.y = (velRel.y > 0) ? -dist/2 : (velRel.y < 0) ? dist/2 : 0;
    }      

    //Check if moving towards each other
    if(posRel * velRel >= 0){
      return false;
    }
      
    //Check distance
    if((upper_type_t<T>)dist * dist < posRel * posRel){      
      return false;
    }
    
    return true;
  }
};


using Obj8_t = Obj<int8_t>;
using Obj16_t = Obj<int16_t>;
using Obj32_t = Obj<int32_t>;


//////////////////////////////
// Bouncing in 1D
template<typename T>
void bounce1d(T &v1, uint8_t m1, T &v2, uint8_t m2){
  //Total mass
  T M = (T)m1 + (T)m2;

  //New speeds
  T u1 = divRound<T>((m1 - m2) * v1 + 2 * m2 * v2, M);
  T u2 = divRound<T>((m2 - m1) * v2 + 2 * m1 * v1, M);

  //Update speeds
  v1 = u1;
  v2 = u2;
}


template<typename T>
void bounce2d(Obj<T> &obj1, uint8_t m1, Obj<T> &obj2, uint8_t m2){
  using V = upper_type_t<T>; // wider integer for intermediates

  //Normal vector
  Pnt<T> n = obj2.pos - obj1.pos;

  //Tangent ortogonal vector
  Pnt<T> t = n.ortogonal();

  //Project speeds to normal and ortogonal vectors
  Pnt<V> u1(obj1.vel * n, obj1.vel * t);
  Pnt<V> u2(obj2.vel * n, obj2.vel * t);

  //Bounce speeds in normal direction
  bounce1d<V>(u1.x, m1, u2.x, m2);
 
  // len * len
  V len2 = n * n;

  //Update velocities
  obj1.vel.x = (T)divRound<V>(u1.x * n.x + u1.y * t.x, len2);
  obj1.vel.y = (T)divRound<V>(u1.x * n.y + u1.y * t.y, len2);

  obj2.vel.x = (T)divRound<V>(u2.x * n.x + u2.y * t.x, len2);
  obj2.vel.y = (T)divRound<V>(u2.x * n.y + u2.y * t.y, len2);
}


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

  inline Pnt<T> normal() const __attribute__((always_inline)){
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