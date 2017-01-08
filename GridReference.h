#ifndef GRID_REFERENCE_H
#define GRID_REFERENCE_H

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"
#include "Arduino.h"

class GridReference {
 public:
  GridReference(void);
  String calculate(float latitude, float longitude);

 private:
   String formatNumber(float number , byte chars);
   String format( os_grid_ref grid_ref );
};
#endif
