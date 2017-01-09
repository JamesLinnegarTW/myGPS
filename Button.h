#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class Button {
 public:
  Button(void);
  void init(byte setup_pin, unsigned int _hold_delay);
  void tick();
  boolean isPressed();
  boolean isHeld();

 private:
   int pin;

   boolean is_pressed;
   boolean is_held;
   
   byte debounce_delay;    // the debounce time; increase if the output flickers
   unsigned int hold_delay;
   byte last_button_state;
   
   unsigned long last_debounce_time;  // the last time the output pin was toggled
   unsigned long last_pressed_time;
   
};
#endif
