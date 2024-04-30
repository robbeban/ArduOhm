/*Misura una resistenza Rx collegata come partitore ad una resistenza R1.
La Vin = +5V da Arduino, le 2 R sono in serie, Rx va a GND. In mezzo alle R c'è il collegamento
all' ingresso analogico.
Attenzione: per valori bassi (<20 ohm) o molto alti (> 100K) la misura non è precisa,
manca il cambio scala dell'ohmmetro.
Roberto De Colle 19.4.24 Ver. 0.9 */

#include <LiquidCrystal.h>
//LiquidCrystal(rs, enable, d4, d5, d6, d7);
//LiquidCrystal lcd(12, 11, 2, 3, 4, 5);  //valori "normali"
LiquidCrystal lcd(12, 11, 7, 8, 9, 10);  //collegamenti per lo SHIELD LCD1

int val;  //valore letto 0..1023
//float R1 = 1000.0;              //R1 del partitore
float Rx, volt;                   //Rx da misurare, volt calcolati sulla Rx
int pin[5] = { 10, 9, 8, 7, 6 };  //pin dove collegare le R1 da 1M, 100K, 10K, 1K, 100
int scala = 0;
float R1[5] = { 1000000.0, 100000.0, 10000.0, 1000.0, 100.0 };  //valori di R1

void setup() {
  lcd.begin(16, 2);  //inizializza display a 16 car. e 2 righe
  lcd.clear();       //pulisce lo schermo
  lcd.print("OHMMETRO ARDUINO");
  delay(3000);
  Serial.begin(9600);
}

void loop() {
  staccatutto();                    //stacca tutti i pin
  pinMode(pin[scala], OUTPUT);     //attivo il pin giusto
  digitalWrite(pin[scala], HIGH);  //metto alto il pin della R1
  delay(100);
  val = analogRead(A0);
  if (val > 500) {                //se il val letto è buono...
    calcola(R1[scala]);
    stampa();
    debug();
  } else {                        //cambio scala
    if (scala < 4)
      scala++;
    else
      scala = 0;
  }

  delay(500);
}

void calcola(float R) {
  volt = 5.0 / 1023 * val;       //calcola i volt su Rx
  Rx = volt * R / (5.0 - volt);  //calcola Rx
}

void stampa() {
  lcd.clear();
  lcd.print("Volt= ");
  lcd.print(volt);
  lcd.print(" V");
  lcd.setCursor(0, 1);         //passiamo alla seconda riga
  if (volt >= 5)               //se la lettura è troppo grande
    lcd.print("Inserire Rx");  //la Rx non è inserita
  else {
    lcd.print("Rx = ");
    if (Rx >= 10)          //se la Rx è grande
      lcd.print(int(Rx));  //stampa valore senza virgola
    else
      lcd.print(Rx);    //se la Rx è piccola
    lcd.print(" ohm");  //stampa valore con virgola
  }
}

void staccatutto() {  //mette tutti i pin R1 INPUT
  for (int i = 0; i <= 4; i++) {
    pinMode(pin[i], INPUT);
  }
}

void debug(){
  Serial.print("scala = ");
  Serial.println(scala);
  Serial.print("R1 = ");
  Serial.println(R1[scala]);
  Serial.print("val = ");
  Serial.println(val);
  Serial.print("Rx = ");
  Serial.println(Rx);
}