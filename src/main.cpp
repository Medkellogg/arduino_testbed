#include <Arduino.h>
#include "Ticker.h"
#include <Bounce2.h>

//------------Setup sensor debounce from Bounce2 library-----
#define mainSensInpin 11
#define mainSensOutpin 12
#define revSensInpin 10 
#define revSensOutpin 9
#define LED_PIN 13  //debug

// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce(); Bounce debouncer4 = Bounce();

//---Sensor variables
byte mainSensTotal = 0, mainSens_Report = 0; 
byte mainPassByState = false, mainPassByTotal = 0;
byte mainInValue = 1, mainIn_LastValue = 1; 
byte mainOutValue = 1, mainOut_LastValue = 1;

byte sensBusy = 0;

//DEBUG SECTION
const int mainPassByOff = 7;  // green wire main
int mainPassByToZero = 1;
//const int revPassByOff = 8;  // wht wire rev
//int revPassByToZero = 1;
//----END DEBUG---------------

//---Function Declarations---------------
int readMainSens();



void setup() {

  // Setup the button (using external pull-up) :
  pinMode(mainSensInpin, INPUT); pinMode(mainSensOutpin, INPUT);
  pinMode(revSensInpin, INPUT);  pinMode(revSensOutpin, INPUT);

  // After setting up the button, setup the Bounce instances :
  debouncer1.attach(mainSensInpin); debouncer2.attach(mainSensOutpin);
  debouncer3.attach(revSensInpin);  debouncer4.attach(revSensOutpin);
  debouncer1.interval(5); debouncer2.interval(5); // interval in ms
  debouncer3.interval(5); debouncer4.interval(5); 

//DEBUG Section - these are manual switches until functions are ready
  pinMode(mainPassByOff, INPUT_PULLUP);
  //pinMode(revPassByOff, INPUT_PULLUP);
  //----END DEBUG---------------

  Serial.begin(115200);
  //Setup the LED :
  pinMode(LED_PIN,OUTPUT);

}

void loop() {
  
  
  //int revSenInValue = debouncer3.read();  
  //int revSenOutValue = debouncer4.read();

sensBusy = readMainSens();

mainPassByToZero = digitalRead(mainPassByOff);

    if(mainPassByToZero == 0){
       mainPassByState = 0;
      }
      
      //Serial.print("mainOutValue: ");
      //Serial.println(mainOutValue);
      Serial.print("sensBusy: ");
      Serial.println(sensBusy);
      //Serial.print("mainOutLastValue: ");
      //Serial.println(mainOut_LastValue);
      Serial.print("mainSens_Report: ");
      Serial.println(mainSens_Report);
      Serial.print("mainSensTotal: ");
      Serial.println(mainSensTotal);
      Serial.println("-----end of SENSORS--------");
      Serial.print("mainPassByTotal: ");
      Serial.println(mainPassByTotal);
      Serial.print("mainPassByState: ");
      Serial.println(mainPassByState);
      Serial.println("-----end of the line--------");
      delay(150);
      
 
}// end loop

int readMainSens() {
  
  debouncer1.update();
  int mainInValue = debouncer1.read();
    
  if(mainInValue != mainIn_LastValue)     
    {
      if(mainInValue == 0) bitSet(mainSens_Report, 0);
      else bitClear(mainSens_Report, 0); 

      mainIn_LastValue = mainInValue;

      if (mainSens_Report > 0) {
        mainSensTotal = mainSensTotal + mainSens_Report;
        mainPassByTotal = mainSensTotal;
      }
      else mainSensTotal = 0;   
    }
    // end readMainSenIn()

    //void readMainSenOut() {
  
  debouncer2.update();
  int mainOutValue = debouncer2.read();
    
  if(mainOutValue != mainOut_LastValue)     
    {
      if(mainOutValue == 0) bitSet(mainSens_Report, 1);
      else bitClear(mainSens_Report, 1); 

      mainOut_LastValue = mainOutValue;

      if (mainSens_Report > 0) {
        mainSensTotal = mainSensTotal + mainSens_Report;
        mainPassByTotal = mainSensTotal;
      }
      else mainSensTotal = 0; 
    }
    if(mainSensTotal == 0 && mainPassByTotal == 6) {
       mainPassByState = true;
       mainPassByTotal = 0;
      }
    else if(mainSensTotal == 0) mainPassByTotal = 0;
      
  
  return mainSensTotal;
}  // end readMainSen--
  
  
  
  
  