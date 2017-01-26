#ifndef GRID_REFERENCE_CALCULATOR_H
#define GRID_REFERENCE_CALCULATOR_H

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"
#include "Arduino.h"

class GridReferenceCalculator {
 public:
  GridReferenceCalculator(void);
  void calculate(float latitude, float longitude);
  void getCurrentGridReference(char * input);
  void getCurrentEasting(char * input);
  void getCurrentNorthing(char * input);

 private:
   os_eas_nor_t currentEastingNorthing;
};
#endif
