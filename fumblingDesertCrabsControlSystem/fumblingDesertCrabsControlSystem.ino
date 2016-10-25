#include <Wire.h>
#define ADDRESS 0x1E


void setup() {
  Serial.begin(9600);
  Wire.begin();
  
}

void loop() {
  int x = getCompassHeading();
  Serial.print(x);
}

int getCompassHeading(){
  byte x, y, z;
  double heading;
  
  Wire.beginTransmission(ADDRESS);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(ADDRESS, 6);
  
  while(Wire.available() < 6);
  x = Wire.read()<<8; //X msb
  x |= Wire.read(); //X lsb
  z = Wire.read()<<8; //Z msb
  z |= Wire.read(); //Z lsb
  y = Wire.read()<<8; //Y msb
  y |= Wire.read(); //Y lsb

  heading = atan2(x, y);
  return heading*57.29582;
}
