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
  _pushed     = 0;
  _value      = digitalRead(_pin) == HIGH ? 1 : 0;
  _valueIdle  = _value;
}

PushButton::~PushButton(){
}

void PushButton::read(){
  if( clicked() ){
    _pushed = 0;
  }
  
  _value = digitalRead(_pin) == HIGH ? 1 : 0;

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





