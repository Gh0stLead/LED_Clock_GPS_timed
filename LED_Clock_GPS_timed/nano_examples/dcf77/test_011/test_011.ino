/*
 * DCFPulse
 * Ralf Bohnen, 2013
 * This example code is in the public domain.    
*/
 
#define BLINKPIN 13
#define DCF77PIN 2
 
int SignalHIGHStart = 0;
int SignalHIGHEnde = 0;
int SignalHIGHZeit = 0;
int SignalLOWStart = 0;
int SignalLOWEnde = 0;
int SignalLOWZeit = 0;
 
bool Signal = false;
bool neueMinute = false;
int bitnr = -1;
 
void setup() {
  Serial.begin(9600);
  pinMode(DCF77PIN, INPUT);
  pinMode(BLINKPIN, OUTPUT);
  Serial.println("Warte auf Start einer neuen Minute...");
}
 
void loop() {
 
  int pinValue = digitalRead(DCF77PIN); //Wert am PIN einlesen
 
  if (pinValue == HIGH && Signal == false) { //PIN ist HIGH, vorher war kein HIGH
    Signal = true; 
    SignalHIGHStart = millis(); 
    SignalLOWEnde = SignalHIGHStart;  
    SignalLOWZeit = SignalLOWEnde - SignalLOWStart; 
 
    //DEBUG Ausgabe nach Serial
    if (neueMinute) {PrintBeschreibung(bitnr);
      Serial.print("BitNr.:"); Serial.print (bitnr); 
      Serial.print (" Wert :"); Serial.print (werteBitAus(SignalHIGHZeit)); 
      Serial.println (" ");
    }
  } //ENDE (pinValue == HIGH && Signal == false)
 
  if (pinValue == LOW && Signal == true) { //PIN ist LOW vorher war HIGH
    Signal = false; 
    SignalHIGHEnde = millis();  
    SignalLOWStart = SignalHIGHEnde; 
    SignalHIGHZeit = SignalHIGHEnde - SignalHIGHStart; 
 
    feststellenObNeueMinute(SignalLOWZeit);
  } //ENDE (pinValue == LOW && Signal == true)
 
} //END LOOP
 
//Gebe den Wert zurück den das Bit aufgrund der HIGH Zeit-Länge haben muss 
int werteBitAus (int SignalHIGHZeit) {
  //mit den Zeiten ein wenig größzügig sein, die schwanken um den Optimalwert
   if (SignalHIGHZeit >= 90 && SignalHIGHZeit <= 110) {return 0;} 
   if (SignalHIGHZeit >= 190 && SignalHIGHZeit <= 225) {return 1;} // 190 210
}
 
//Wenn LOW Zeit größer 1750ms dann neue Minute BitNummer auf 0 setzen, 
//ansonsten BitNummer hochzählen
void feststellenObNeueMinute (int SignalLOWZeit) {
  //auch hier ein wenig Luft lassen bei der Zeit.
  if (SignalLOWZeit >= 1750) {bitnr = 0; neueMinute = true;} else {bitnr++; }
}
 
void PrintBeschreibung(int BitNummer) {
  switch (BitNummer) {
    case  0: Serial.println("\n### S T A R T  N E U E  M I N U T E"); break;
    case  1: Serial.println("\n### CODIERTE WETTERDATEN"); break;
    case 15: Serial.println("\n### RUFBIT RESERVEANTENNE"); break;
    case 16: Serial.println("\n### ANKUENDIGUNG UMSTELLUNG MEZ/MESZ"); break;
    case 17: Serial.println("\n### 0 = MEZ | 1 = MESZ"); break;
    case 18: Serial.println("\n### 0 = MESZ | 1 = MEZ"); break;
    case 19: Serial.println("\n### 1 = SCHALTSEKUNDE AM ENDE DER STUNDE"); break;
    case 20: Serial.println("\n### BEGIN ZEITINFORMATION (IMMER 1)"); break;
    case 21: Serial.println("\n### BEGIN MINUTEN"); break;
    case 28: Serial.println("\n### PARITAET MINUTE"); break;
    case 29: Serial.println("\n### BEGIN STUNDE");break;
    case 35: Serial.println("\n### PARITAET STUNDE"); break;
    case 36: Serial.println("\n### BEGIN KALENDERTAG"); break;
    case 42: Serial.println("\n### BEGIN WOCHENTAG"); break;
    case 45: Serial.println("\n### BEGIN MONATSNUMMER"); break;
    case 50: Serial.println("\n### BEGIN JAHR"); break;
    case 58: Serial.println("\n### PARITAET DATUM"); break;
  }
}
