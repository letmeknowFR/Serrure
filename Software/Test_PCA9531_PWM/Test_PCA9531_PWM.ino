// Test PCA9531
// Servo en LED0 et capteur en LED1

#include<Wire.h>

#define Addr 0x60 // Adresse I2C du chip

void setup() {
  
  Serial.begin(9600);

  Wire.begin();

  Wire.beginTransmission(Addr);
  Wire.write(0x01);
  Wire.write(0x01); // Fréquence de PWM à 50Hz pour le servo
  Wire.endTransmission();

  Wire.beginTransmission(Addr);
  Wire.write(0x05);
  Wire.write(0x02); // LED0 sur PWM0 et LED1 à LED3 en input
  Wire.endTransmission();

  Wire.beginTransmission(Addr);
  Wire.write(0x06);
  Wire.write(0x00); // LED4 à LED7 en input
  Wire.endTransmission();
  
  delay(300);
}

void loop()
{
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(Addr, 1);    // request 6 bytes from slave device
  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read();    // receive a byte as character
    Serial.println(c, HEX);  // print the character
  }

  Wire.beginTransmission(Addr);
  Wire.write(0x02);
  Wire.write(0x01); // Rapport cyclique de 0
  Wire.endTransmission();
  
  delay(500);

  Wire.beginTransmission(Addr);
  Wire.write(0x02);
  Wire.write(0xFF); // Rapport cyclique de 0
  Wire.endTransmission();
  
  //delay(3000);

}

