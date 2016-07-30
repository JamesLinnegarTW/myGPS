#include "Arduino.h"

class Button {
 public:
  Button(void);
  void init(int setup_pin);
  void tick();
  boolean is_pressed;
  
 protected:
   int pin;
   long last_debounce_time;  // the last time the output pin was toggled
   long debounce_delay;    // the debounce time; increase if the output flickers
   int last_button_state;
   
};

