#include <Arduino.h>

#include "Ticker.h"

void printMessage();
void printCounter();
void printCountdown();
void blink();
void printCountUS();
void mekMessage();

bool ledState;
int counterUS;

Ticker timer1(printMessage, 3000, 5);
Ticker timer6(mekMessage,1500, 9);
//Ticker timer2(printCounter, 1000, MILLIS);
//Ticker timer3(printCountdown, 1000, 5);
//Ticker timer4(blink, 200);
//Ticker timer5(printCountUS, 100, 0, MICROS_MICROS);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(2000);
  timer1.start();
  timer6.start();
  //timer2.start();
  //timer3.start();
  //timer4.start();
  //timer5.start();
  Serial.println("------------Start-----------------");
  }

void loop() {
  timer1.update();
  timer6.update();
  //timer2.update();
  //timer3.update();
  //timer4.update();
  //timer5.update();
  //if (timer4.counter() == 20) timer4.interval(200);
  //if (timer4.counter() == 80) timer4.interval(1000);
  }



/*void printCounter() {
  Serial.print("Counter ");
  Serial.println(timer2.counter());
  }

void printCountdown() {
  Serial.print("Countdowm ");
  Serial.println(5 - timer3.counter());
  }
  */
void printMessage() {
  Serial.println("Hello!");
  }

void mekMessage() {
  Serial.println("Ha! I'm twice as fast as you!");
}
/*
void blink() {
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
  }

void printCountUS() {
  counterUS++;  
  if (counterUS == 10000) {
    Serial.println("10000 * 100us");
    counterUS = 0;
    }  
  } */