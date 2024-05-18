// I2C master
// Alla pressione di un pulsante legge un valore da un potenziometro lo trasforma in volt e
// manda il valore int e il valore float a uno slave I2C
// Based on work of Nick Gammon, May 2012. Thanks!
// Roberto De Colle 18.5.2024

#include <Wire.h>
#include "I2C_Anything.h"

#define buttonPin 3
#define potPin A0
#define LEDPin 13

const byte SLAVE_ADDRESS = 42;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LEDPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Inizio trasmissione: premi il pulsante per mandare i dati");
  Wire.begin();
  blink();
}
void loop() {
  int val = leggi_dato();
  float vout = 5. / 1023 * val;
  //Serial.print("val = ");
  //Serial.println(val);
  //Serial.print("vout = ");
  //Serial.println(vout);
  if (!digitalRead(buttonPin)) {
    //blink();
    Wire.beginTransmission(SLAVE_ADDRESS);
    I2C_writeAnything(val);
    I2C_writeAnything(vout);
    Wire.endTransmission();
    delay(200);
  }
}

void blink() {
  for (int i = 1; i < 5; i++) {
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);
  }
}

//legge 10 dati da analogRead e restituisce la media
int leggi_dato() {
  int dato = 0;
  for (int k = 1; k <= 10; k++) {
    dato += analogRead(potPin);
  }
  return (dato/10);
}