#include "Display.h"
#include "LEDBackpack.h"

Display::Display(void) {
  AlphaNum4 alpha4_a = AlphaNum4();
  AlphaNum4 alpha4_b = AlphaNum4();
}

void Display::init(){
  alpha4_a.begin(0x70);
  alpha4_b.begin(0x71);
}

void Display::renderString(String to_display) {
  for(int len = to_display.length(); len < 8; len++){
    to_display = to_display + " ";
  }
 
  for(int i=0; i<4; i++){
    alpha4_a.writeDigitAscii(i, to_display.charAt(i));
    alpha4_b.writeDigitAscii(i, to_display.charAt(i+4));
  }
  
  alpha4_a.writeDisplay();
  alpha4_b.writeDisplay(); 

}

void  Display::clear(){
  alpha4_a.clear();
  alpha4_b.clear();
  alpha4_a.writeDisplay();
  alpha4_b.writeDisplay();
}
