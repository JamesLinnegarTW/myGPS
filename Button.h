#include "Arduino.h"

class Button {
 public:
  Button(void);
  void init(byte setup_pin);
  void tick();
  boolean is_pressed;
  
 protected:
   int pin;
   long last_debounce_time;  // the last time the output pin was toggled
   long debounce_delay;    // the debounce time; increase if the output flickers
   byte last_button_state;
   
};

