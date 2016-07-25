#include <Wire.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "Adafruit_LEDBackpack.h"

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"

SoftwareSerial mySerial(11, 10);
Adafruit_GPS gps(&mySerial);

long timer = millis();
long last_render = millis();
long display_time = 15000;
long flash_time = 5000;
long button_time = 20000;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
long buttonPressTime = 0;


int buttonState;
int lastButtonState = LOW;
int buttonPressLength = 500;
int button_pin = 9;
int toDisplay = 0;

boolean render = false;

Adafruit_AlphaNum4 alpha4_a = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha4_b = Adafruit_AlphaNum4();

void setup() {
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT);
  alpha4_a.begin(0x70);
  alpha4_b.begin(0x71);
  
  gps.begin(9600);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  renderString("OH HELLO");
  delay(2000);
  clearDisplay();
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

void renderString(String toDisplay){
   
  for(int len = toDisplay.length(); len < 8; len++){
    toDisplay = toDisplay + " ";
  }
 
  for(int i=0; i<4; i++){
    alpha4_a.writeDigitAscii(i, toDisplay.charAt(i));
    alpha4_b.writeDigitAscii(i, toDisplay.charAt(i+4));
  }
  
  alpha4_a.writeDisplay();
  alpha4_b.writeDisplay(); 
}

void clearDisplay(){
  alpha4_a.clear();
  alpha4_b.clear();
  alpha4_a.writeDisplay();
  alpha4_b.writeDisplay();
}

void renderLocation(){
  if(gps.fix) {
        renderString(getGridRef(gps.latitudeDegrees,
                       gps.longitudeDegrees));
    } else {
      renderString("LOCATING");
    }
}

void renderAlt(){
  renderString("ALT " + String((int) gps.altitude));
}


void loop() {
  gps.read();
  int reading = digitalRead(button_pin);
  
   if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    buttonPressTime = millis();
  }
  
  if(gps.newNMEAreceived()){
    gps.parse(gps.lastNMEA());    
  }
    
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if(reading==HIGH){
        if(render){
          render = false;
          timer = millis() + 60000;
           
           Serial.println("Turn off?");
        } else {
          last_render = millis()+ 20000;
          display_time = button_time;
        }
      } 
    } else {
      if(reading==HIGH){
         if((millis() - buttonPressTime) > 1000){
            if(render == true){
              toDisplay++;
              
              if(toDisplay > 1){
                toDisplay = 0;
              }
            }
            last_render = millis() + 20000;
            buttonPressTime = millis();
          } 
      }
    }
  }
  
  if(millis() - last_render > 10000 && render == false) {
    timer = millis();
    render = true;
  }
  
  if(render){
    switch (toDisplay) {
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
  lastButtonState = reading;

}
