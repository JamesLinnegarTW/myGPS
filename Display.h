
#include "LEDBackpack.h"

class Display {
 public:
  Display(void);
  void init();
  void renderString(String toDisplay);
  void clear();
 protected:
  AlphaNum4 alpha4_a;
  AlphaNum4 alpha4_b;
};

