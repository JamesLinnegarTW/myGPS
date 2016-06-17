#include <math.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"
SoftwareSerial mySerial(11, 10);
Adafruit_GPS gps(&mySerial);
uint32_t timer = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gps.begin(9600);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
}

String formatNumber( float number ) {
  String input = String(number);
  int i = input.lastIndexOf(".");
  String output = input.substring(0, i); 

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

double convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}


void loop() {

  if(gps.newNMEAreceived()){
    if(!gps.parse(gps.lastNMEA()))
      return;  
  }
  if(millis() - timer > 2000) {
    timer = millis();
    if(gps.fix) {
        Serial.println(getGridRef(convertDegMinToDecDeg(gps.latitude),
                       convertDegMinToDecDeg(gps.longitude)));
    } else {
      Serial.println("Search..");
    }
  }
//  //TA 39684 10775
//   float gps_latitude = 53.574977;
//   float gps_longitude = 0.10814667;
// 
//     //   SK 03108 93611
//    gps_latitude = 53.439263;
//    gps_longitude = -1.9546840;
//    
//    //NY325 066
//    gps_latitude = 54.450746;
//    gps_longitude = -3.041759;
    
//  Serial.println(getGridRef(gps_latitude, gps_longitude));  
//  delay(15000);
}
