#include "Button.h"
#include "Arduino.h"

Button::Button(void){
  
}

void Button::init(int setup_pin){
  pin = setup_pin; 
  debounce_delay = 50;     
  pinMode(setup_pin, INPUT);
  button_press_length = 1000;
}

void Button::tick(){
  int reading = digitalRead(pin);
  
  if (reading != last_button_state) {
    last_debounce_time = millis();
    button_press_time = millis();
  }
  
  if ((millis() - last_debounce_time) > debounce_delay) { //check for a debounce
    is_pressed = reading; // high or low
    
    if(reading==HIGH){
       if((millis() - button_press_time) > button_press_length){
          is_held = 1;
          button_press_time = millis();
        } 
     } else {
       is_held = 0;
     }
  }
  last_button_state = reading;
}
