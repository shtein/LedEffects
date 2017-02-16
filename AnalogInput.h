#ifndef __ANALOGINPUT_H
#define __ANALOGINPUT_H

////////////////////////////
// Simple potentiometer

#define POT_MIN 0
#define POT_MAX 1023

class Potentiometer{
  public:
    Potentiometer(int pin, float min, float max);
    ~Potentiometer();

    float value() const;

  protected:
    int   _pin;  
    float _min;
    float _max;
};

#endif //__ANALOGINPUT_H
