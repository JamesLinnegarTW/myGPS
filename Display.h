#ifndef DISPLAY_H
#define DISPLAY_H

#include "LEDBackpack.h"

class Display {
 public:
  Display(void);
  void init();
  void renderString(String to_display);
  void clear();
 protected:
  AlphaNum4 alpha4_a;
  AlphaNum4 alpha4_b;
};

#endif
