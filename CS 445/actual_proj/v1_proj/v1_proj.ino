#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>


// //Hall PINS Input
// int PIN_HALL_1_IN = ;
// int PIN_HALL_2_IN = ;
// int PIN_HALL_3_IN = ;
// int PIN_HALL_4_IN = ;
unsigned long startTime;
int count = 0;
const unsigned long interval = 5000; 
int prev_button_state = LOW; 



// //flex sensor pins Input
// int PIN_FLEX_1_IN = ;
// int PIN_FLEX_2_IN = ;
// int PIN_FLEX_3_IN = ;
// int PIN_FLEX_4_IN = ;
// int PIN_FLEX_5_IN = ;


// void pinsetup(){
//   pinMode(PIN_HALL_1_IN, INPUT)
//   pinMode(PIN_HALL_2_IN, INPUT)
//   pinMode(PIN_HALL_3_IN, INPUT)
//   pinMode(PIN_HALL_4_IN, INPUT)
//   pinMode(PIN_FLEX_1, INPUT)
//   pinMode(PIN_FLEX_2, INPUT)
//   pinMode(PIN_FLEX_3, INPUT)
//   pinMode(PIN_FLEX_4, INPUT)
//   pinMode(PIN_FLEX_5, INPUT)
// }

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
    pinMode(11, INPUT);
    Serial.print("Begin");
  //  out()
  //  pinsetup();
   
}

void loop() {
  Serial.print("this is the vlue of Start Time ");
  Serial.println(startTime);
  unsigned long currentMills = millis();
  if (digitalRead(11) == HIGH){
    prev_button_state = HIGH;
    if (startTime == 0) { // if counting hasn't started yet
      startTime = millis(); // set the start time
    }
    
  } 
   Serial.println(prev_button_state);
  if (digitalRead(11) == LOW && prev_button_state == HIGH )
  {
    prev_button_state = LOW;
    Serial.println("Counting");
    count++; // increment the count
  }
  //  if (digitalRead(PIN_HALL_2_IN) == HIGH){
  //   if (startTime == 0) { // if counting hasn't started yet
  //     startTime = millis(); // set the start time
  //   }
  //   count++; // increment the count
  // } else {
  //   startTime = 0; // reset the start time
  // }

  //  if (digitalRead(PIN_HALL_3_IN) == HIGH){
  //   if (startTime == 0) { // if counting hasn't started yet
  //     startTime = millis(); // set the start time
  //   }
  //   count++; // increment the count
  // } else {
  //   startTime = 0; // reset the start time
  // }

  //  if (digitalRead(PIN_HALL_4_IN) == HIGH){
  //   if (startTime == 0) { // if counting hasn't started yet
  //     startTime = millis(); // set the start time
  //   }
  //   count++; // increment the count
  // } else {
  //   startTime = 0; // reset the start time
  // }
   if (millis() - startTime >= interval && startTime != 0) { // if the counting interval has elapsed
    Serial.print("5 seconds has ellapsed and the vlaue of count is ");
    Serial.println(count); // print the count change this to what it should be 
    count = 0; // reset the count
    startTime = 0; // reset the start time
    delay(5000);
  }

  // put your main code here, to run repeatedly:
  // digital_hall_1 = digitalRead(PIN_HALL_1_IN)
  // digital_hall_2 = digitalRead(PIN_HALL_2_IN)
  // digital_hall_3 = digitalRead(PIN_HALL_3_IN)
  // digital_hall_4 = digitalRead(PIN_HALL_4_IN)  
  // if(PIN_HALL_1_IN == HIGH){

  }

