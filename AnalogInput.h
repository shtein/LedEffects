#ifndef __ANALOGINPUT_H
#define __ANALOGINPUT_H

#include "IRremote.h"

////////////////////////////
// Simple potentiometer
// Define pin and scale in constructor
#define POT_MIN 0
#define POT_MAX 1023


class Potentiometer{
  public:
    Potentiometer(int pin, float min, float max);
    ~Potentiometer();

    void read();
    float value() const;

  protected:
    int   _pin;  
    float _min;
    float _max;
    int   _value;
};


///////////////////////
// Simple push button
// Define pin
 
class PushButton{
  public:
    PushButton(int pin);
    ~PushButton();

    void read();
    bool pushed() const;

  protected:
    int  _pin;
    int  _value;
    int  _valueIdle;
    bool _pushed;
    
};


///////////////////////////////////////
// Simple IR Receiver for Keyes remote

//Keys
#define RKEY_REPEAT 0xFFFFFFFF
#define RKEY_OK     0xFF02FD
#define RKEY_LEFT   0xFF22DD
#define RKEY_RIGHT  0xFFC23D
#define RKEY_UP     0xFF629D
#define RKEY_DOWN   0xFFA857
#define RKEY_0      0xFF4AB5
#define RKEY_1      0xFF6897
#define RKEY_2      0xFF9867
#define RKEY_3      0xFFB04F
#define RKEY_4      0xFF30CF
#define RKEY_5      0xFF18E7
#define RKEY_6      0xFF7A85
#define RKEY_7      0xFF10EF
#define RKEY_8      0xFF38C7
#define RKEY_9      0xFF5AA5
#define RKEY_STAR   0xFF42BD
#define RKEY_HASH   0xFF52AD

class IRRemoteRecv {
  public:
    IRRemoteRecv(int pin);
    ~IRRemoteRecv();

    void read();
    int pushed(unsigned long key) const;

  protected:
    void translate();

  protected:
    IRrecv         _recv;          
    decode_results _results;
    unsigned long  _millis;
    unsigned long  _value;
    int            _repeat;
    bool           _pushed;
};

#endif //__ANALOGINPUT_H
