#include "Display.h"
#include "LEDBackpack.h"

Display::Display(void) {
  AlphaNum4 a1_4 = AlphaNum4();
  AlphaNum4 a5_8 = AlphaNum4();
}

void Display::init(){
  a1_4.begin(0x70);
  a5_8.begin(0x71);
  
  a1_4.setBrightness(15);
  a5_8.setBrightness(15);  
}

void Display::renderString(String to_display) {
  Serial.println(to_display);
  
  a1_4.clear();
  a5_8.clear();
  
  for(byte i=0; i<4; i++){
    a1_4.writeDigitAscii(i, to_display.charAt(i));
    a5_8.writeDigitAscii(i, to_display.charAt(i+4));
  }
  
  a1_4.writeDisplay();
  a5_8.writeDisplay(); 

}

void  Display::clear(){
  a1_4.clear();
  a5_8.clear();
  a1_4.writeDisplay();
  a5_8.writeDisplay();
  Serial.println("--------");
}

