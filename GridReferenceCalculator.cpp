#include "Arduino.h"
#include "GridReferenceCalculator.h"
#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"


GridReferenceCalculator::GridReferenceCalculator(void){
  
}


void GridReferenceCalculator::calculate( float latitude, float longitude){
   if(!latitude && !longitude) return "LOCATING";
   
   float gps_ellipsoidal_height = 1.0;
   
   os_lat_lon_t   home_ll_wgs84 = { .lat=DEG_2_RAD(latitude)
                           , .lon=DEG_2_RAD(longitude)
                           , .eh=gps_ellipsoidal_height
                           };
  
  os_cartesian_t home_c_wgs84 = os_lat_lon_to_cartesian(home_ll_wgs84, OS_EL_WGS84);
  os_cartesian_t home_c_airy30 = os_helmert_transform(home_c_wgs84, OS_HE_WGS84_TO_OSGB36);
  os_lat_lon_t home_ll_airy30 = os_cartesian_to_lat_lon(home_c_airy30, OS_EL_AIRY_1830);
  os_eas_nor_t home_en_airy30 = os_lat_lon_to_tm_eas_nor(home_ll_airy30, OS_TM_NATIONAL_GRID);

  currentEastingNorthing = home_en_airy30;
   
}

void GridReferenceCalculator::getCurrentGridReference(char * input){

  char code[2];
  char buffer[10];

  os_grid_ref_t home_grid_ref = os_eas_nor_to_grid_ref(currentEastingNorthing, OS_GR_NATIONAL_GRID);  

  strncpy(input, NO_LOCATION, 8);
  return;
  
  strncpy(input, home_grid_ref.code, 2);

  dtostrf(home_grid_ref.e, 5, 0, buffer);

  for(byte i = 0; i  < 3; i++){
    if(buffer[i] == ' '){
      input[i+2]='0';
    } else {
      input[i+2] = buffer[i];
    }
  }

  dtostrf(home_grid_ref.n, 5, 0, buffer);

  for(byte i = 0; i  < 3; i++){
    if(buffer[i] == ' '){
      input[i+5]='0';
    } else {
      input[i+5] = buffer[i];
    }
  }

  input[8] = NULL;
}

void GridReferenceCalculator::getCurrentEasting(char *input) {
  char buffer[10];
  dtostrf(currentEastingNorthing.e, 6, 0, buffer);
  input[0] = 'E';
  input[1] = ' ';
  for(byte i = 2; i < 8; i++){
    input[i] = buffer[i-2];
  }
}

void GridReferenceCalculator::getCurrentNorthing(char *input) {
  char buffer[10];
  dtostrf(currentEastingNorthing.n, 6, 0, buffer);
  input[0] = 'N';
  input[1] = ' ';
  for(byte i = 2; i < 8; i++){
    input[i] = buffer[i-2];
  }
}
