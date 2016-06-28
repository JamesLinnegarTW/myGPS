#include <math.h>
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"
SoftwareSerial mySerial(11, 10);
Adafruit_GPS gps(&mySerial);
uint32_t timer = millis();
uint32_t last_render = millis();
uint32_t display_time = 5000;
uint32_t flash_time = 5000;
uint32_t button_time = 10000;

boolean render = false;
Adafruit_AlphaNum4 alpha4_a = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha4_b = Adafruit_AlphaNum4();
int button_pin = 9;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button_pin, INPUT);
  alpha4_a.begin(0x70);  // pass in the address
  alpha4_b.begin(0x71);  // pass in the address
  //alpha4_a.setBrightness(7);
  //alpha4_b.setBrightness(7);
  gps.begin(9600);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  renderString("OH HELLO");
  delay(2000);
  clearDisplay();
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


void renderString(String toDisplay){
  for(int i=0; i<4; i++){
    alpha4_a.writeDigitAscii(i, toDisplay.charAt(i));
    alpha4_b.writeDigitAscii(i, toDisplay.charAt(i+4));
  }
  
  alpha4_a.writeDisplay();
  alpha4_b.writeDisplay();
 Serial.println(toDisplay); 
}

void clearDisplay(){
  alpha4_a.clear();
  alpha4_b.clear();
  alpha4_a.writeDisplay();
  alpha4_b.writeDisplay();
}

void loop() {
  char c = gps.read();
  if(gps.newNMEAreceived()){
    if(!gps.parse(gps.lastNMEA()))
      return;  
  }
  
  if(digitalRead(button_pin)==HIGH){
    last_render = millis()+ 20000;
    display_time = button_time;
  }
  
  if(millis() - last_render > 10000 && render == false) {
    timer = millis();
    render = true;
  }
  
  if(render){
    if(gps.fix) {
        renderString(getGridRef(gps.latitudeDegrees,
                       gps.longitudeDegrees));
    } else {
      renderString("LOCATING");
    }
    if(millis() - timer > display_time) {
      last_render = millis();
      timer = millis();
      clearDisplay();
      if(gps.fix){
        display_time = flash_time;
      } else {
        display_time = 1000;
      }
      render = false;
    }
  }

}
