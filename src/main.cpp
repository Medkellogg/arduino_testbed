#include <Arduino.h>
#include "Ticker.h"
#include <Bounce2.h>

//------------Setup sensor debounce from Bounce2 library-----
#define mainSensInpin 11
#define mainSensOutpin 12
#define revSensInpin 10 
#define revSensOutpin 9

#define INBOUND 1    
#define OUTBOUND 2
#define CLEAR 0

#define LED_PIN 13  //debug

// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce(); Bounce debouncer4 = Bounce();

//---Sensor variables
byte mainSensTotal = 0, mainSens_Report = 0; 
byte mainPassByState = false, mainPassByTotal = 0;
byte mainInValue = 1, mainIn_LastValue = 1; 
byte mainOutValue = 1, mainOut_LastValue = 1;
byte main_LastDirection = 0;
byte mainLineDirection = 0;
byte getMainDirection = 0;
byte mainDirection = 0;
bool mainOutbound = 0;
bool mainInbound = 0;

byte revSensTotal = 0, revSens_Report = 0; 
byte revPassByState = false, revPassByTotal = 0;
byte revInValue = 1, revIn_LastValue = 1; 
byte revOutValue = 1, revOut_LastValue = 1;
byte revLoopDirection = 0;
byte rev_LastDirection = 0;
byte getRevLoopDirection = 0;
byte revDirection = 0;
bool revOutbound = 0;
bool revInbound = 0;

bool entry_ExitBusy = false;

//DEBUG SECTION
const int mainPassByOff = 7;  // green wire main
int mainPassByToZero = 1;
const int revPassByOff = 8;  // wht wire rev
int revPassByToZero = 1;
//----END DEBUG---------------

//---Function Declarations---------------
int readMainSens();
int readRevSens();
byte rptMainDirection();
byte rptRevDirection();
bool readAllSens();
//adding this now for the benefit of Linda my love



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
  pinMode(revPassByOff, INPUT_PULLUP);
  //pinMode(revPassByOff, INPUT_PULLUP);
  //----END DEBUG---------------

  Serial.begin(115200);
  //Setup the LED :
  pinMode(LED_PIN,OUTPUT);

}

void loop() {
  
  entry_ExitBusy = readAllSens();
  getMainDirection = rptMainDirection();
  getRevLoopDirection = rptRevDirection();
  
mainPassByToZero = digitalRead(mainPassByOff);  //debug

    if(mainPassByToZero == 0){
       mainPassByState = 0;
      }

revPassByToZero = digitalRead(revPassByOff);  //debug

    if(revPassByToZero == 0){
       revPassByState = 0;
      }
       //debug

      Serial.print("mainOutValue: ");
      Serial.print(mainOutValue);
      Serial.print("        revOutValue: ");
      Serial.println(revOutValue);
      Serial.print("mainSens_Report: ");
      Serial.print(mainSens_Report);
      Serial.print("     revSens_Report: ");
      Serial.println(revSens_Report);
      Serial.print("mainSensTotal: ");
      Serial.print(mainSensTotal);
      Serial.print("       revSensTotal: ");
      Serial.println(revSensTotal);
      Serial.print("mainPassByTotal: ");
      Serial.print(mainPassByTotal);
      Serial.print("     revPassByTotal: ");
      Serial.println(revPassByTotal);
      Serial.print("mainPassByState: ");
      Serial.print(mainPassByState);
      Serial.print("     revPassByState: ");
      Serial.println(revPassByState);
      Serial.print("entryExitBusy: ");
      Serial.println(entry_ExitBusy);
      Serial.print("mainLineDirection: ");
      Serial.print(getMainDirection);
      Serial.print("   revLoopDirection: ");
      Serial.println(getRevLoopDirection);
      Serial.print("main_LastDirection: ");
      Serial.print(main_LastDirection);
      Serial.print("   rev_lastDirection: ");
      Serial.println(rev_LastDirection);
      Serial.println();
      Serial.println("=======Report Starts Here!=======");
      delay(307); 

 
}// end void loop

  /*---------------Updating Sensor Functions----------------------
  All in this section update and track sensor information: Busy,
  Direction, PassBy.  Only the mainOut sensor is documented.  The
  remaining three work identically.
  ------------------------------end of note---------------------*/

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
    
  
  debouncer2.update();  //read Out sensor
  int mainOutValue = debouncer2.read();

      //---update history register:*Sens_Report    
  if(mainOutValue != mainOut_LastValue)   
    {
      if(mainOutValue == 0) bitSet(mainSens_Report, 1);
      else bitClear(mainSens_Report, 1); 

      mainOut_LastValue = mainOutValue;

      //---add running total to "*"SensTotal to track PassBy status
      if (mainSens_Report > 0) {
        mainSensTotal = mainSensTotal + mainSens_Report;
        mainPassByTotal = mainSensTotal;
      }
      else mainSensTotal = 0; 
    }
      /*---PassByTotal of "6" means train has cleared the sensor success-
           fully.  Only done once at the end of each "read"*"Sens()".---*/

    if(mainSensTotal == 0 && mainPassByTotal == 6) {
       mainPassByState = true;
       mainPassByTotal = 0;
      }
    else if(mainSensTotal == 0) mainPassByTotal = 0;
      
  
  return mainSensTotal;
}  // end readMainSen--

int readRevSens() {  
  debouncer3.update();
  int revInValue = debouncer3.read();
    
  if(revInValue != revIn_LastValue)     
    {
      if(revInValue == 0) bitSet(revSens_Report, 0);
      else bitClear(revSens_Report, 0); 

      revIn_LastValue = revInValue;

      if (revSens_Report > 0) {
        revSensTotal = revSensTotal + revSens_Report;
        revPassByTotal = revSensTotal;
      }
      else revSensTotal = 0;   
    }
      
  debouncer4.update();
  int revOutValue = debouncer4.read();
    
  if(revOutValue != revOut_LastValue)     
    {
      if(revOutValue == 0) bitSet(revSens_Report, 1);
      else bitClear(revSens_Report, 1); 

      revOut_LastValue = revOutValue;

      if (revSens_Report > 0) {
        revSensTotal = revSensTotal + revSens_Report;
        revPassByTotal = revSensTotal;
      }
      else revSensTotal = 0; 
    }
    if(revSensTotal == 0 && revPassByTotal == 6) {
       revPassByState = true;
       revPassByTotal = 0;
      }
    else if(revSensTotal == 0) revPassByTotal = 0;
  return revSensTotal;
}  // end readrevSen--

  
bool readAllSens() {
  bool sensBusy;
  int sensMainBusy = 0;
  int sensRevBusy = 0;

  sensMainBusy = readMainSens();
  sensRevBusy = readRevSens();

  if(sensMainBusy > 0 || sensRevBusy >0){
    sensBusy = true;
  }
  else sensBusy = false;
  return sensBusy;
}


//--------------------------------------------------

byte rptMainDirection() 
  {      
    if((mainSensTotal == 2) && (mainSens_Report = 2)) {
      mainOutbound = 1;
      mainInbound = 0;
      }
    else if((mainSensTotal == 1) && (mainSens_Report = 1)) {
      mainOutbound = 0;
      mainInbound = 1;
      }
    if((mainDirection == 0) && (mainSensTotal == 0)) {
      mainOutbound = 0;
      mainInbound = 0;
    } 
    
    if(mainOutbound == 1){
      mainLineDirection = 2;
      main_LastDirection = 2;
    }
    if(mainInbound == 1){
      mainLineDirection = 1;
      main_LastDirection = 1;
    }
    if((mainOutbound == 0) && (mainInbound == 0)){
     mainLineDirection = 0;
    }

    return mainLineDirection;
  }  //End function

/*-----------------FUNCTION rptRevDirection()--------------------------
    This function is the same as above, but reports on the Reverse Loop.
 --------------------------------------------------------------------*/

  byte rptRevDirection() 
  {
    if((revSensTotal == 2) && (revSens_Report = 2)) {
      revOutbound = 1;
      revInbound = 0;
      }
    else if((revSensTotal == 1) && (revSens_Report = 1)) {
      revOutbound = 0;
      revInbound = 1;
      }
    if((revDirection == 0) && (revSensTotal == 0)) {
      revOutbound = 0;
      revInbound = 0;
      }
    
    if(revOutbound == 1){
      revLoopDirection = 2;
      rev_LastDirection = 2;
    }
    if(revInbound == 1){
      revLoopDirection = 1;
      rev_LastDirection = 1;
    }
    if((revOutbound == 0) && (revInbound == 0)){
     revLoopDirection = 0;
    }

    return revLoopDirection;
  }  //End function   

  
  