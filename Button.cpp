#include "Button.h"
#include "Arduino.h"

Button::Button(void){
  
}

void Button::init(byte setup_pin){
  pin = setup_pin; 
  debounce_delay = 50;     
  pinMode(setup_pin, INPUT);
}

void Button::tick(){
  int reading = digitalRead(pin);
  
  if (reading != last_button_state) {
    last_debounce_time = millis();
  }
  
  if ((millis() - last_debounce_time) > debounce_delay) { //check for a debounce
    is_pressed = reading; // high or low
  }
  last_button_state = reading;
}
