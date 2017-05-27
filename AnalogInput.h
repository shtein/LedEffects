#ifndef __ANALOGINPUT_H
#define __ANALOGINPUT_H

#include "IRremote.h"

////////////////////////////
// Base AnalogInput
class AnalogInput {
  public:
    AnalogInput();
    ~AnalogInput();

    virtual void read() = 0;
};

////////////////////////////
// Simple potentiometer
// Define pin
#define POT_MIN             0
#define POT_MAX             1023
#define POT_NOISE_THRESHOLD 4


class Potentiometer: public AnalogInput {
  public:
    Potentiometer(uint8_t pin);
    ~Potentiometer();

    void read();
    int value() const;

  protected:
    uint16_t  _pin:5;  
    uint16_t  _value:11;
};


///////////////////////
// Simple push button
// Define pin
 
class PushButton: public AnalogInput{
  public:
    PushButton(uint8_t pin);
    ~PushButton();

    void read();
    bool clicked() const;

  protected:
    uint8_t         _pin:5;
    uint8_t        _value:1;
    uint8_t        _valueIdle:1;
    uint8_t        _pushed:1;    
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

class IRRemoteRecv: public AnalogInput {
  public:
    IRRemoteRecv(uint8_t pin);
    ~IRRemoteRecv();

    void read();
    int pushed(unsigned long key) const;

  protected:
    IRrecv         _recv;          
    unsigned long  _millis;
    unsigned long  _value;
    uint8_t        _repeat:7;
    uint8_t        _pushed:1;
};


////////////////////////////
//RotaryEncoder

class RotaryEncoder: public AnalogInput {
  public:
    RotaryEncoder(uint8_t pinData, uint8_t pinClock);
    ~RotaryEncoder();

    void read();
    int value() const;

  protected:
    uint8_t  _pinData:5;  
    uint8_t  _pinClock:5;  
    uint8_t  _value:5;
    uint8_t  _valClock:1;
};



#endif //__ANALOGINPUT_H
