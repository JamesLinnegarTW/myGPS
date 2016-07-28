#include "Arduino.h"

class Button {
 public:
  Button(int setup_pin);
  void tick();
  boolean is_pressed;
  boolean is_held;
 protected:
   int pin;
   long last_debounce_time;  // the last time the output pin was toggled
   long debounce_delay;    // the debounce time; increase if the output flickers
   long button_press_time;
   
   int button_state;
   int last_button_state;
   int button_press_length;
   
};

