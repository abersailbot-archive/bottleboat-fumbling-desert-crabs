#include <Wire.h>
#include <Servo.h> 
#define ADDRESS 0x21
#define SERVO_MID 95
#define WANTED 0
#define MOTOR 5

Servo rudder_servo;
int motor_speed = 130;

void setup() {
  Serial.begin(9600);
  delay(3000);
  Wire.begin();
  rudder_servo.attach(2);
}

void loop() {
  double x = getCompassHeading();
  Serial.println(x);
  double diff = angleDifference(x, WANTED);
  double servoAng = setServo(diff);
  setRudder(servoAng);
  setMotor(motor_speed);
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

double angleDifference(double current, double desired){
  double abs_diff, abs_deduction, diff, angle;
  abs_diff = 180 - abs(abs(current - desired) - 180);
  abs_deduction = abs(current - desired);
  diff = current - desired;
  if (abs_diff == 0) {
    angle = 0;
  } else if (abs_deduction < 180) {
    if (diff < 0) {
      angle = abs_diff;
    } else {
      angle = abs_diff * -1;
    }
  } else if (abs_deduction > 180) {
    if (diff < 0) {
      angle = abs_diff * -1;
    } else {
      angle = abs_diff;
    }
  }

  return angle;
}

void setRudder(double rudderPos){
  int servoAngle = 0;
  if (rudderPos>0){
    servoAngle = rudderPos+SERVO_MID;
  } else if (rudderPos<0){
      servoAngle=SERVO_MID+rudderPos;
  }

  rudder_servo.write(servoAngle);
}

double setServo(double angleDifference){
  static double pGain = 0.2;
  static double iGain = 0.2;
  static double integral;
  if (angleDifference < 5 && angleDifference > -5){
    integral += angleDifference;
  } else {
    integral = 0;
  }
  double p = angleDifference * pGain;
  double i = integral * iGain;
  double angToSet = p + i;
  return angToSet;
}

void setMotor(int pwm){
  analogWrite(MOTOR, pwm);
}
