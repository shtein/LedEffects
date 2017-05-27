#include "precomp.h"
#include "AnalogInput.h"

///////////////////
//AnalogInput

AnalogInput::AnalogInput(){
}

AnalogInput::~AnalogInput(){
}

///////////////////
// Potentiometer

Potentiometer::Potentiometer(uint8_t pin){
  _pin   = pin;
  _value = POT_MIN;
 
  read();
}

Potentiometer::~Potentiometer(){
}

void Potentiometer::read(){ 
  _value = analogRead(_pin);
}

int Potentiometer::value() const{
  return _value;
}


//////////////////
// PushButton
PushButton::PushButton(uint8_t pin){
   _pin        = pin;
   pinMode(_pin, INPUT);
   digitalWrite(_pin, HIGH);
  _pushed     = 0;
  _value      = digitalRead(_pin);
  _valueIdle  = _value;
}

PushButton::~PushButton(){
}

void PushButton::read(){
  if( clicked() ){
    _pushed = 0;
  }
  
  _value = digitalRead(_pin);

  if (_value != _valueIdle){
    _pushed = 1;
  }
}


bool PushButton::clicked() const{
  return _value == _valueIdle && _pushed == 1;
}

//////////////////////////
// IRRemoteRecv
#define REMOTE_TIMEOUT      10
#define REMOTE_REPEAT_LIMIT 127

IRRemoteRecv::IRRemoteRecv(uint8_t pin): _recv(pin){
  _recv.enableIRIn(); 
  _millis = 0;
  _value  = 0;
  _repeat = 0;
  _pushed = false;
}

IRRemoteRecv::~IRRemoteRecv(){
}



void IRRemoteRecv::read(){
  
   //Reset if necessary
   if(pushed(_value)){
    _pushed = false;
   }


  //Check if we are waiting for receiver or for resuming receiver
  if (_millis == 0) { 


    //Stupid workaround for ws2811, ws2812 and ws2812b where internal clock conflicts with interrups required for IR receiver
    //See https://github.com/FastLED/FastLED/issues/198
    while (!_recv.isIdle());
 
    //Now go and get IR signals
    decode_results results;
    if (_recv.decode(&results)) {
    
      // Remember time when reset IR
      _millis = millis() + REMOTE_TIMEOUT;   

      //Remember value
      _pushed = true;
      
      if(results.value == RKEY_REPEAT && _repeat < REMOTE_REPEAT_LIMIT){
        _repeat ++;
      }
      else{
        _value  = results.value;
        _repeat = 1;
      }

      Serial.print("Remote button ");
      Serial.print(results.value, HEX);
      Serial.print("\n");
    }
    
  }
  else {
    if (_millis <= millis()){
      // Receiving  next value        
      _recv.resume(); 
      // Reset timer
      _millis = 0;      
    }
    
  }

  
}

int IRRemoteRecv::pushed(unsigned long key) const{
  if(_pushed == true && _value == key){
    return _repeat;
  }

  return 0;
}

//////////////////////////////////////
// RotaryEncodder
RotaryEncoder::RotaryEncoder(uint8_t pinData, uint8_t pinClock){
  _pinData      = pinData;
  _pinClock     = pinClock;
  _value        = 0;
  _valClock    = HIGH;
  
  pinMode (_pinData, INPUT);
  pinMode (_pinClock, INPUT); 
}


RotaryEncoder::~RotaryEncoder(){
}

// _value can be one of the following: 0 (1 up), 1 (no change), 2 (1 down)

void RotaryEncoder::read(){    
  
 //See where the clock is
 int valClock = digitalRead(_pinClock);

 //Read pin only when clock is changing from LOW to HIGH
 if((_valClock == LOW) && (valClock == HIGH)) {
      //Now we can read data
     _value = digitalRead(_pinData) == LOW ? 2 : 0 ;     
  } 
  else {
    _value = 1;
  }

 //Remember clock
 _valClock = valClock;
}

int RotaryEncoder::value() const{  
  return (int)1 - (int)_value;
}




