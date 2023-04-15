#include <Servo.h>

Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;
Servo myServo5;

void setup() {
  Serial.begin(9600);
  myServo1.attach(D2); // attach servo to pin 9
  myServo2.attach(D3);
  myServo3.attach(D4);
  myServo4.attach(D5);
  myServo5.attach(D6);
  // Serial.println("Beginning");
  Servo servoArray[] = {myServo1, myServo2, myServo3, myServo4, myServo5};
  int angle = 90; // desired angle
  int count = 0;
  for (int i = 0; i<5; i++){
  // Serial.print("This is servo: ");
  // Serial.println(i);
  // Serial.print("this is counter: ");
  // Serial.println(count);
  // count++;
  // int pulseWidth = map(angle, 0, 180, 500, 2500); // map angle to pulse width
  servoArray[i].write(angle);
  delay(4000); // wait for servo to reach angle
  }
}

void loop() {
  delay(100000);
}
