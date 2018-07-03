/* Servomoteur piloté par un Arduino et actionné par un fin de course; le tout par liaison I2C.
 * 
 *  *** Cablage du PCA9531 ***
 *   A0 -> GND
 *   A1 -> GND
 *   A2 -> GND
 *   LED0 -> Servomoteur
 *   LED1 -> Fin de course
 *   LED2 -> GND
 *   LED3 -> GND
 *   LED4 -> GND
 *   LED5 -> GND
 *   LED6 -> GND
 *   LED7 -> GND
 */
#include<Wire.h> // Bibliothèque I2C

#define Addr 0x60 // Adresse I2C du PCA9531

void setup() 
{
  /*** Vitesse du terminal série ***/
  Serial.begin(9600);
  
  /*** Initialisation communication avec Arduino "maître" ***/
  Wire.begin();

  Wire.beginTransmission(Addr);
  Wire.write(0x01);             // Registre du prédiviseur de fréquence 0
  Wire.write(0x01);             // Fréquence de PWM à 50Hz pour le servo
  Wire.endTransmission();

  Wire.beginTransmission(Addr);
  Wire.write(0x05);             // Registre du selecteur LED0 à LED3
  Wire.write(0x02);             // LED0 sur PWM0 et LED1 à LED3 en entrée
  Wire.endTransmission();

  Wire.beginTransmission(Addr);
  Wire.write(0x06);             // Registre du selecteur LED4 à LED7
  Wire.write(0x00);             // LED4 à LED7 en entrée
  Wire.endTransmission();
  
  delay(300);
}

void loop()
{
  Wire.beginTransmission(Addr);
  Wire.write(0x00);             // Selection du registre d'entrée pour la lecture de données
  Wire.endTransmission();
  Wire.requestFrom(Addr, 1);    // Demande de données à l'esclave (PCA9531) sur 1 bit

  while(Wire.available())       // L'esclave peut envoyer moins que la demande
  { 
 
    char c = Wire.read();       // Recevoir un octet comme caractère
    Serial.println(c, HEX);     // Afficher le caractère
    delay(50);

     if (c ==0)                 // Si le fin de course est actionné
    {
      Wire.beginTransmission(Addr);
      Wire.write(0x02);             // LED0 sur PWM0 et LED1 à LED3 en entrée 
      Wire.write(0xE7);             // Rotation de 50° du servomoteur pour la position fermée
      Wire.endTransmission();
      
      delay(3000);
  
      Wire.beginTransmission(Addr);
      Wire.write(0x02);             // LED0 sur PWM0 et LED1 à LED3 en entrée
      Wire.write(0x8F);             // Rotation de 130° du servomoteur pour la position ouverte
      Wire.endTransmission();
      
      delay(1000);
    }
    else                           // Si le fin de course n'est pas actionné
    {
      Wire.beginTransmission(Addr);
      Wire.write(0x02);             // LED0 sur PWM0 et LED1 à LED3 en entrée
      Wire.write(0x8F);             // Rotation de 130° du servomoteur pour la position ouverte
      Wire.endTransmission();
    }
  }
}

