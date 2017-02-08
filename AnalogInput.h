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
    int _pin;  
    float _min;
    float _max;
};

inline Potentiometer::Potentiometer(int pin, float min, float max){
  _pin = pin;
  _min = min;
  _max = max;
}

inline Potentiometer::~Potentiometer(){
}

inline float Potentiometer::value() const{
  int val = analogRead(_pin);

  return _min + (val - POT_MIN) * (_max - _min) / (POT_MAX - POT_MIN);
}
