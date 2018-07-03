#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define buttonRAZ 6 // Bouton Remise à zero
#define buttonHold 7 // Bouton Hold
#define fc 8 // Fin de course
#define servopin 9 // Pin du servo
#define addresseHold 0 // EEprom pour le hold
#define addresseLive 1 // EEprom pour le live
#define angle_initial 130 //angle initial
#define angle_final 40 //angle final

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo myservo;  // création de l'objet myservo 

boolean StateHold = 0; // Etat bouton Hold
boolean StateRAZ = 0; // Etat bouton RAZ
boolean StateFc = 0; // Etat fin de course
long unsigned int valeurHold = 0; // Compteur Hold
long unsigned int valeurLive = 0; // Compteur Live

void setup() {
  // Initialisation LCD I2C
  lcd.clear();
  lcd.init();
  lcd.backlight();
  // Initialisation des entrées / sorties
  myservo.attach(servopin);  // attache le servo au pin spécifié sur l'objet myservo
  pinMode(fc, INPUT_PULLUP);
  pinMode(buttonHold, INPUT_PULLUP);
  pinMode(buttonRAZ, INPUT_PULLUP);
  // Chargement des paramètres
  valeurHold = EEPROM.read(addresseHold); // lit une valeur en mémoire Eeprom à l'adresse indiquée
  valeurLive = EEPROM.read(addresseLive); // lit une valeur en mémoire Eeprom à l'adresse indiquée
}

void loop() {
  // Lecture des états
  StateFc = digitalRead(fc);
  StateHold = digitalRead(buttonHold);
  StateRAZ = digitalRead(buttonRAZ);

  // Interprétation
  if (StateFc == LOW)  {  // Fait bouger le bras de 0° à 180°
    myservo.write(angle_final); // Position verrouillée
    delay(1000);
    myservo.write(angle_initial); // Position libre
    delay(1000);
    valeurLive++; // incrémentation du compteur
    EEPROM.write(addresseLive, valeurLive); // écrit une valeur en mémoire Eeprom à l'adresse indiquée
    if(valeurLive%100 == 0) {
      EEPROM.write(addresseLive, valeurLive); // écrit une valeur en mémoire Eeprom à l'adresse indiquée
    }
  } else {
    myservo.write(angle_initial); // Position libre
  }
  
  if (StateHold == LOW) { // Bouton Hold
    valeurHold = valeurLive;
    EEPROM.write(addresseHold, valeurHold); // écrit une valeur en mémoire Eeprom à l'adresse indiquée
    EEPROM.write(addresseLive, valeurLive); // écrit une valeur en mémoire Eeprom à l'adresse indiquée
  }

  if (StateRAZ == LOW) { // button raz
    lcd.clear();
    valeurLive = 0;
    valeurHold = 0;
    EEPROM.write(addresseLive, valeurLive); // écrit une valeur en mémoire Eeprom à l'adresse indiquée
    EEPROM.write(addresseHold, valeurHold); // écrit une valeur en mémoire Eeprom à l'adresse indiquée                
  }

  // Actualisation de l'affichage
  lcd.setCursor(0,0);
  lcd.print("Cycle :");
  lcd.setCursor(8,0);
  lcd.print(valeurLive);
  lcd.setCursor(4,1);
  lcd.print("H : ");
  lcd.setCursor(8,1);
  lcd.print(valeurHold);

}
