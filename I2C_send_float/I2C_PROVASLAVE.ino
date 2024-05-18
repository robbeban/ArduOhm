// I2C slave
// riceve un valore int e un valore float da un master I2C
// e li mostra sul serial monitor. All'arrivo di dati accende un led per 500 ms
// slave address = 42
// Based on work of Nick Gammon, May 2012. Thanks!
// Roberto De Colle 18.5.2024

#include <Wire.h>
#include "I2C_Anything.h"

const byte MY_ADDRESS = 42;
#define LEDPin 13

void setup() {
  pinMode(LEDPin, OUTPUT);
  Wire.begin(MY_ADDRESS);
  Serial.begin(115200);
  Wire.onReceive(receiveEvent);
  digitalWrite(LEDPin, HIGH);
  delay(500);
  digitalWrite(LEDPin, LOW);
  delay(500);
}

volatile boolean haveData = false;
volatile int val;
volatile float vout;

void loop() {
  if (haveData) {
    Serial.print("Ricevuto val = ");
    Serial.println(val);
    Serial.print("Ricevuto vout = ");
    Serial.println(vout);
    haveData = false;
    digitalWrite(LEDPin, HIGH);
    delay(500);
  }
  else{
    digitalWrite(LEDPin, LOW);
    //Serial.println("niente dati");
  }
}

// called by interrupt service routine when incoming data arrives
void receiveEvent(int howMany) {
  if (howMany >= (sizeof val) + (sizeof vout)) {
    I2C_readAnything(val);
    I2C_readAnything(vout);
    haveData = true;
  } 
} 