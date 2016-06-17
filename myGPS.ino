#include <math.h>
#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

String formatNumber( float number ) {
  String input = String(number);
  int blah = input.lastIndexOf(".");
  String output = input.substring(0, blah); 

  for(int len = output.length(); len < 5; len++){
    output = "0" + output;
  } 
  return output.substring(0,3);
}

String formatGridRef( os_grid_ref grid_ref){
  String output = grid_ref.code;
  output += formatNumber(grid_ref.e);
  output += formatNumber(grid_ref.n);
  return output;
}

String getGridRef( float latitude, float longitude){

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
  
  return formatGridRef(home_grid_ref);
}

void loop() {
  //TA 39684 10775
   float gps_latitude = 53.574977;
   float gps_longitude = 0.10814667;
 
     //   SK 03108 93611
    gps_latitude = 53.439263;
    gps_longitude = -1.9546840;
    
    //NY325 066
    //gps_latitude = 54.450746;
    //gps_longitude = -3.041759;
    
  Serial.print(getGridRef(gps_latitude, gps_longitude));  
  Serial.println("");
  delay(15000);
}
