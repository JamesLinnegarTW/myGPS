#include "Button.h"
#include "Arduino.h"

Button::Button(void){}

void Button::init(byte setup_pin, unsigned int _hold_delay){
  pin = setup_pin; 
  debounce_delay = 50;     
  hold_delay = _hold_delay;

  is_pressed = false;
  is_held = false;
  
  pinMode(setup_pin, INPUT);

}

void Button::tick(){
  int reading = digitalRead(pin);
  is_held = false;
  if (reading != last_button_state) {
    last_debounce_time = millis();
  }
  
  if ((millis() - last_debounce_time) > debounce_delay) { //check for a debounce
    is_pressed = reading; // high or low
    if(is_pressed && last_pressed_time == NULL) {
      last_pressed_time = millis();
    } else if(!is_pressed) {
      last_pressed_time = NULL;
      is_held = false;
    }

  }

  if(is_pressed && ((millis() - last_pressed_time) > hold_delay)) {
      last_pressed_time = millis();
      is_held = true;
  }

  last_button_state = reading;
}

boolean Button::isPressed(){
  return is_pressed;
}

boolean Button::isHeld() {
  return is_held;
}

