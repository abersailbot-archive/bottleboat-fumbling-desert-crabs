#include <Wire.h>
#include <Servo.h> 
#define ADDRESS 0x21
#define SERVO_MID 95
#define WANTED 0

Servo rudder_servo;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rudder_servo.attach(2);
}

void loop() {
  double x = getCompassHeading();
  Serial.println(x);
  double diff = angleDifference(x, WANTED);
  double servoAng = setServo(diff);
  setRudder(servoAng);
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

void setRudder(double rudderPos){
  int servoAngle = 0;
  if (rudderPos>0){
    servoAngle = rudderPos+SERVO_MID;
  } else if (rudderPos<0){
      servoAngle=SERVO_MID-rudderPos;
  }

  rudder_servo.write(servoAngle);
}

double setServo(double angleDifference){
  static double pGain = 0.2;
  static double iGain = 0.2;
  static double integral;
  if (angleDifference < 5){
    integral += angleDifference;
  } else {
    integral = 0;
  }
  double p = angleDifference * pGain;
  double i = integral * iGain;
  double angToSet = p + i;
  return angToSet;
}

