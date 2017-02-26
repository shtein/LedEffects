#include "precomp.h"
#include "AnalogInput.h"

///////////////////
// Potentiometer

Potentiometer::Potentiometer(int pin, float min, float max){
  _pin = pin;
  _min = min;
  _max = max;
  read();
}

Potentiometer::~Potentiometer(){
}

void Potentiometer::read(){
  _value = analogRead(_pin);
}

float Potentiometer::value() const{

  return _min + (_value - POT_MIN) * (_max - _min) / (POT_MAX - POT_MIN);
}


//////////////////
// PushButton
PushButton::PushButton(int pin){
   pinMode(_pin, INPUT);
  _pin        = pin;
  _pushed     = false;
  _value      = digitalRead(_pin);
  _valueIdle  = _value;
}

PushButton::~PushButton(){
}

void PushButton::read(){
  if( pushed() ){
    _pushed = false;
  }
  
  _value = digitalRead(_pin);

  if (_value != _valueIdle){
    _pushed = true;
  }
}


bool PushButton::pushed() const{
  return _value == _valueIdle && _pushed == true;
}

//////////////////////////
// IRRemoteRecv
#define REMOTE_TIMEOUT 50

IRRemoteRecv::IRRemoteRecv(int pin): _recv(pin){
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
    if (_recv.decode(&_results)) {
      // Remember time when reset IR
      _millis = millis() + REMOTE_TIMEOUT;   

      //Remember value
      _pushed = true;
      
      if(_results.value == RKEY_REPEAT){
        _repeat ++;
      }
      else{
        _value  = _results.value;
        _repeat = 1;
      }

      //Debug output
      translate(); 
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

void IRRemoteRecv::translate() {

  switch(_results.value) {
    case 0xFF629D: Serial.println(" FORWARD"); break;
    case 0xFF22DD: Serial.println(" LEFT");    break;
    case 0xFF02FD: Serial.println(" -OK-");    break;
    case 0xFFC23D: Serial.println(" RIGHT");   break;
    case 0xFFA857: Serial.println(" REVERSE"); break;
    case 0xFF6897: Serial.println(" 1");    break;
    case 0xFF9867: Serial.println(" 2");    break;
    case 0xFFB04F: Serial.println(" 3");    break;
    case 0xFF30CF: Serial.println(" 4");    break;
    case 0xFF18E7: Serial.println(" 5");    break;
    case 0xFF7A85: Serial.println(" 6");    break;
    case 0xFF10EF: Serial.println(" 7");    break;
    case 0xFF38C7: Serial.println(" 8");    break;
    case 0xFF5AA5: Serial.println(" 9");    break;
    case 0xFF42BD: Serial.println(" *");    break;
    case 0xFF4AB5: Serial.println(" 0");    break;
    case 0xFF52AD: Serial.println(" #");    break;
    case 0xFFFFFFFF: Serial.println(" REPEAT");break;  
  
    default: 
      Serial.println(" other button   "); 
      Serial.println(_results.value, HEX); 
    break;

  }


}


