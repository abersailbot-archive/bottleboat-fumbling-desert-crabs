#include <Wire.h>
#define ADDRESS 0x21


void setup() {
  Serial.begin(9600);
  Wire.begin();
  
}

void loop() {
  int x = getCompassHeading();
  Serial.println(x);
}

double getCompassHeading(){
  int x;
  double heading;
  
  Wire.beginTransmission(ADDRESS);
  Wire.write('A');
  Wire.endTransmission();

  Wire.requestFrom(ADDRESS, 2);
  
  while(Wire.available() < 2);
  x = Wire.read()<<8; //X msb
  x |= Wire.read(); //X lsb

  heading = x/10;
  return heading;
}

double angleDifference(double a1, double a2){
  double angle;
  angle = 180 - abs(abs(a1 - a2) - 180);
  return angle;
}

