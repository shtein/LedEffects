///////////////////
// Basic effect

class Effect{
  public:
    Effect();
    virtual ~Effect();

    //Two functions to proceed: init and proceed
    void init(CRGB *leds, int numLeds);
    void loop(int speedDelay);
    

  protected:
    virtual void reset() = 0;
    virtual void proceed(int speedDelay) = 0;
  
    //Utility functions
    void showStrip();
    void setPixel(int led, byte red, byte green, byte blue);
    void setPixel(int led, const CRGB &color);
    void setAll(byte red, byte green, byte blue);
    void setAll(const CRGB &color);
    CRGB selectRandomColor() const;

    //Proceeding related
    void nextProceedIn(int delta);
    bool timeToProceed() const;
    
 protected:
  //Data
  int   _numLeds;
  CRGB *_leds;

  //Next check point
  unsigned long _millis;
};

inline Effect::Effect(){
  _numLeds = 0;
  _leds    = NULL;
  _millis   = 0;
}

inline Effect::~Effect(){
}

inline void Effect::init(CRGB *leds, int numLeds){
  _leds    = leds;
  _numLeds = numLeds;

  setAll(0, 0, 0);
  nextProceedIn(0);

  reset();
}


inline void Effect::loop(int speedDelay){
  //Check timer
  if(!timeToProceed())
    return;

  //Serial.print("proceed\n");
  
  //Run effect step
  proceed(speedDelay);
  showStrip();

  //if next time to proceed is not set then set it by default
  if(timeToProceed())
    nextProceedIn(speedDelay);
}

inline void Effect::nextProceedIn(int delta){
  _millis = millis() + delta;
}

inline bool Effect::timeToProceed() const{ 
  return _millis <= millis() ? true: false;
}
    

inline void Effect::showStrip() {
   FastLED.show();
}

inline void Effect::setPixel(int led, byte red, byte green, byte blue) {
   _leds[led].r = red;
   _leds[led].g = green;
   _leds[led].b = blue;
}

inline void Effect::setAll( byte red, byte green, byte blue) {
  for(int i = 0; i < _numLeds; i++ ) {
    setPixel(i, red, green, blue); 
  }
  
  showStrip();
}

inline void Effect::setPixel(int led, const CRGB &color) {
   _leds[led] = color;
}

inline void Effect::setAll(const CRGB &color) {
  for(int i = 0; i < _numLeds; i++ ) {
    setPixel(i, color); 
  }
}


inline CRGB Effect::selectRandomColor() const {
  CRGB color(0xFF00FF);
  color.setHue(random(256));
  
 
  return color;
}

