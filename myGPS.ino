#include <Wire.h>
#include <SoftwareSerial.h>
#include "Display.h"
#include "Button.h"
#include "GPS.h"
#include "LEDBackpack.h"

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"

SoftwareSerial mySerial(11, 10);
GPS gps(&mySerial);

long timer = millis();
long display_time = 15000;

int button_pin = 9;
int to_display = 0;

boolean render = false;
Display screen = Display();
Button display_button = Button();


void setup() {
  screen.init();
  display_button.init(button_pin);
  gps.begin(9600);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
 
  screen.renderString("Oh Hello");
  delay(2000);
  screen.clear();
}

String formatNumber( float number , int chars) {
  String input = String(number);

  String output = input.substring(0, input.lastIndexOf(".")); 

  for(int len = output.length(); len < chars; len++){
    output = "0" + output;
  } 
  return output.substring(0,3);
}

String formatGridRef( os_grid_ref grid_ref){
  String output = grid_ref.code;
  output += formatNumber(grid_ref.e, 5);
  output += formatNumber(grid_ref.n, 5);
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

void renderLocation(){
  if(gps.fix) {
        screen.renderString(getGridRef(gps.latitudeDegrees,
                       gps.longitudeDegrees));
    } else {
      screen.renderString("LOCATING");
    }
}

void renderAlt(){
  screen.renderString("ALT " + String((int) gps.altitude));
}


void loop() {
  gps.read();
  
  if(gps.newNMEAreceived()){
    gps.parse(gps.lastNMEA());    
  }
  display_button.tick();

  if(display_button.is_pressed){
    timer = millis(); // length of time to display
    render = true;
  }
  
  if(display_button.is_held) {
    timer = millis();
    render = true;
    to_display++;
    
    if(to_display > 1){
      to_display = 0;
    }
  }
  

  if(render){
    switch (to_display) {
      case 0:
        renderLocation();
        break;
      case 1:
        renderAlt();
        break;
      default:
         renderLocation();
    }
    
    if(millis() - timer > display_time) {
      timer = millis();
      screen.clear();
      render = false;
    }
  }


}
