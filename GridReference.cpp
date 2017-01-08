#include "Arduino.h"
#include "GridReference.h"
#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"


GridReference::GridReference(void){
  
}


String GridReference::calculate( float latitude, float longitude){

   float gps_ellipsoidal_height = 1.0;
   
   os_lat_lon_t   home_ll_wgs84 = { .lat=DEG_2_RAD(latitude)
                           , .lon=DEG_2_RAD(longitude)
                           , .eh=gps_ellipsoidal_height
                           };
  
  os_cartesian_t home_c_wgs84 = os_lat_lon_to_cartesian(home_ll_wgs84, OS_EL_WGS84);
  os_cartesian_t home_c_airy30 = os_helmert_transform(home_c_wgs84, OS_HE_WGS84_TO_OSGB36);
  os_lat_lon_t home_ll_airy30 = os_cartesian_to_lat_lon(home_c_airy30, OS_EL_AIRY_1830);
  os_eas_nor_t home_en_airy30 = os_lat_lon_to_tm_eas_nor(home_ll_airy30, OS_TM_NATIONAL_GRID);
  os_grid_ref_t home_grid_ref = os_eas_nor_to_grid_ref(home_en_airy30, OS_GR_NATIONAL_GRID);
  
  return format(home_grid_ref);
}

String  GridReference::formatNumber( float number , byte chars) {
  String input = String(number);

  String output = input.substring(0, input.lastIndexOf(".")); 

  for(byte len = output.length(); len < chars; len++){
    output = "0" + output;
  } 
  return output.substring(0,3);
}

String  GridReference::format( os_grid_ref grid_ref){
  String output = grid_ref.code;
  output += formatNumber(grid_ref.e, 5);
  output += formatNumber(grid_ref.n, 5);
  return output;
}
