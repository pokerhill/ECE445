#include <Servo.h>
int i = 3;
Servo thumb;
Servo index_finger;
Servo middle;
Servo ring;
Servo pinky;
Servo servoArray[] = { thumb, index_finger, middle, ring, pinky }; //90 is 0 position
//thumb min: 15 middle: 40 max:145
//index min:0 middle: 25 max 145
// middle min:0  middle: 25  max:150
// ring min:8  middle: ? max: 140
//pink min: 26   middle: 45 max:140 


void setup() {
  Serial.begin(9600);
  thumb.attach(D6);  // attach servo to pin 9
  index_finger.attach(D5);
  middle.attach(D4);
  ring.attach(D3);
  pinky.attach(D2);
  Serial.println("Beginning");
}


void loop() {
  if (Serial.available() > 0) {
    int angle = Serial.parseInt();  // desired angle
    if (angle > 270 || angle < 0) {
      Serial.println("Try again");
      return;
    } else {
      Serial.print("value of angle: ");
      Serial.println(angle);
    }
    servoArray[i].write(angle);
  }
}
