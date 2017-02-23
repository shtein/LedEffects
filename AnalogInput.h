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

class IRRemoteRecv {
  public:
    IRRemoteRecv(int pin);
    ~IRRemoteRecv();

    void read();
    bool pushed(unsigned long key) const;

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
