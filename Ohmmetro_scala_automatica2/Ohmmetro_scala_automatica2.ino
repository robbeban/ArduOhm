//Roberto De Colle
//ver. 0.2
//22.4.2024

int val;                                                     //valore letto
float Rx, tensione;                                          //Rx resistenza da misurare, tensione calcolata
float R1 = { 100.0, 1000.0, 10000.0, 100000.0, 1000000.0 };  //valori di R1
int pin = { 1, 2, 3, 4, 5 };                                 //pin dove sono collegati le R1
bool misura_ok = FALSE;

void setup() {
  Serial.begin(9600);
  //imposta LCD
  for (int i; i < 5; i++) {  //stacca tutte le R1
    pinMode(pin[i], INPUT);
  }
  scala = 0;
}

void loop() {
  if (!misura_ok) {
    for (int k; k < 5; k++)  //stacca tutte le R1
      pinMode(pin[k], INPUT);

    pinMode(pin[scala], OUTPUT);  //attiva la R1 della scala attuale
    if (scala == 0)
      digitalWrite(pin[scala], LOW);  //attiva il PNP
    else
      digitalWrite(pin[scala], HIGH);  //manda 5V sulla R1

    val = analogRead(A0);
    if (val <= 683) {                //se il valore letto è buono..
      Rx = Calcola(val, R1[scala]);  //calcola il valore di Rx con la R1 della scala corretta
      Stampa();
      misura_ok = TRUE;
    } else {
      scala++;
    }
  }
}

float Calcola(int v, float R1) {
  tensione = 5 * val / 1023;
  return tensione * R1 / (5 - R1);
}

void Stampa() {
  lcd.clear();
  lcd.print("tensione= ");
  Serial.print("tensione= ");
  lcd.print(tensione);
  Serial.print(tensione);
  lcd.print(" V");
  Serial.println(" V");
  lcd.setCursor(1, 0);
  if (tensione < 5.0) {  //se la tensione non è 5 V
    lcd.print("Rx= ");
    Serial.print("Rx= ");
    if (Rx < 10.) {
      lcd.print(Rx);  //per resistenze piccole (<10) stampa il valore con la virgola
      Serial.print(Rx);
    } else {
      lcd.print(int(Rx));  //per resistenze grandi (>10) stampa il valore senza virgola
    }
    lcd.print(" Ohm");
    Serial.println(" Ohm");
  } else {  //se invece la tensione è al massimo..
    lcd.println("Inserire Resistenza");
    Serial.println("Inserire Resistenza");
  }
}
