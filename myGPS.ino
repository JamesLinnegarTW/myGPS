/**
 * Wiring Pins
 * 
 * Display
 *  SCL 3
 *  SDA 2
 *   
 * GPS Software serial
 *  9, 8 TX, RX
 *  
 */
 
#include <Wire.h>
#include <SoftwareSerial.h>

#include "GPS.h"
#include "Display.h"
#include "Button.h"
#include "GridReferenceCalculator.h"

const byte  DISPLAY_BUTTON_PIN = 6;
const long  DISPLAY_TIME = 10000;
const int   REFRESH_INTERVAL = 2000;
const long  AUTO_RENDER_INTERVAL = 20000;
const int   DISPLAY_BUTTON_HOLD_TIME = 1000;
const unsigned int CALCULATE_INTERVAL = 5000;

SoftwareSerial gpsSerial(9, 8);
GPS   gps(&gpsSerial);

unsigned long timeAtLastRender;
unsigned long timeAtLastRefresh;
unsigned long timeAtLastCalculation;

byte toDisplay = 0;
boolean render = false;

Display screen = Display();
Button displayButton = Button();

GridReferenceCalculator calculator = GridReferenceCalculator();

void setup() {
  screen.init();
  displayButton.init(DISPLAY_BUTTON_PIN, DISPLAY_BUTTON_HOLD_TIME);
  
  gps.begin(9600);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

  screen.renderCharArray("  MYGPS ");
  timeAtLastRender = millis();
  delay(2000);
  screen.clear();
}

void loop() {
  unsigned long now = millis();
  
  displayButton.sample();
  
  if(displayButton.isPressed()){
    render = true;
    timeAtLastRender = now;
  }
  
  if(displayButton.isHeld()) { 
    toDisplay++;
    if(toDisplay > 7){
      toDisplay = 0;
    } 
  }
  
  if(gps.fix && (now  - timeAtLastCalculation > CALCULATE_INTERVAL)){
    calculator.calculate(gps.latitudeDegrees, gps.longitudeDegrees);
    timeAtLastCalculation = now;
  }
  
  if(now - timeAtLastRender > DISPLAY_TIME) { // reset the screen and turn it off
    render = false;
  }


  if((render == false) && (now - timeAtLastRender) > AUTO_RENDER_INTERVAL){
    render = true;
    timeAtLastRender = now;
  }
    
  if((now - timeAtLastRefresh) > REFRESH_INTERVAL || displayButton.isPressed()){
   
    if(render){ // only render the screen every n milliseconds

      switch (toDisplay) {
        case 0:
          renderLocation();
          break;
        case 1:
          renderAlt();
          break;
        case 2:
          renderSpeed();
          break;
        case 3:
          renderSatellites();
          break;
        case 4:
          renderAngle();
          break;
        case 5:
          renderTime();
          break;
        case 6:
          renderEasting();
          break;
        case 7:
          renderNorthing();
          break;
        case 8:
          renderDate();
          break;
        default:
           renderLocation();
      } 

    } else {
      screen.clear();
    }
   
    timeAtLastRefresh = now;   
   
  }
  
  char c = gps.read();
  
  if(gps.newNMEAreceived()){
    gps.parse(gps.lastNMEA()); 
  }
}

void renderLocation(){
  char toDisplay[9] = "LOCATING";
  if(gps.fix){
    calculator.getCurrentGridReference(toDisplay);
  }
  screen.renderCharArray(toDisplay);
}

void renderAlt(){
  char toDisplay[9];
  sprintf(toDisplay, "ALT %4d", (int) gps.altitude);
  screen.renderCharArray(toDisplay);
}

void renderSatellites(){
  char toDisplay[9];
  sprintf(toDisplay, "SAT %4d", (int) gps.satellites);
  screen.renderCharArray(toDisplay);
}

void renderSpeed(){
  char toDisplay[9];

  sprintf(toDisplay, "SPD %4d", (int) gps.speed);
  screen.renderCharArray(toDisplay);
}

void renderAngle(){
  char toDisplay[9];
  sprintf(toDisplay, "AGL %4d", (int) gps.angle);
  screen.renderCharArray(toDisplay);
}

void renderTime(){
  char toDisplay[9];
  sprintf(toDisplay, "TME %02d%02d", (int) gps.hour, (int) gps.minute);
  screen.renderCharArray(toDisplay);
}

void renderDate(){
  char toDisplay;
  sprintf(toDisplay, "%02d/%02d/%02d", (int) gps.day, (int) gps.month, (int) gps.year);
  screen.renderCharArray(toDisplay);
}
void renderEasting(){
  char toDisplay[9];
  calculator.getCurrentEasting(toDisplay);
  screen.renderCharArray(toDisplay);
}

void renderNorthing() {
  char toDisplay[9];
  calculator.getCurrentNorthing(toDisplay);
  screen.renderCharArray(toDisplay);
}


