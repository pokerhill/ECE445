#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>

//flex sensor pins Input
int PIN_FLEX_1_IN = A0;
int PIN_FLEX_2_IN = A1;
int PIN_FLEX_3_IN = A2;
int PIN_FLEX_4_IN = A3;
int PIN_FLEX_5_IN = A4;

//Hall PINS Input
int PIN_HALL_1_IN =16 ;
int PIN_HALL_2_IN =15 ;
int PIN_HALL_3_IN =7 ;
int PIN_HALL_4_IN =11 ;

void pinsetup(){
  pinMode(PIN_HALL_1_IN, INPUT);
  pinMode(PIN_HALL_2_IN, INPUT);
  pinMode(PIN_HALL_3_IN, INPUT);
  pinMode(PIN_HALL_4_IN, INPUT);
  pinMode(PIN_FLEX_1_IN, INPUT);
  pinMode(PIN_FLEX_2_IN, INPUT);
  pinMode(PIN_FLEX_3_IN, INPUT);
  pinMode(PIN_FLEX_4_IN, INPUT);
  pinMode(PIN_FLEX_5_IN, INPUT);
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial.println("setting up pins:");
pinsetup();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Value of first button: ");
  Serial.println(digitalRead(PIN_HALL_1_IN));
  Serial.print("Value of second button: ");
  Serial.println(digitalRead(PIN_HALL_2_IN));
  Serial.print("Value of third button: ");
  Serial.println(digitalRead(PIN_HALL_3_IN));
  Serial.print("Value of forth button: ");
  Serial.println(digitalRead(PIN_HALL_4_IN));
  Serial.println("________________________ ");
  Serial.print("Value of first flex sensor: ");
  Serial.println(analogRead(PIN_FLEX_1_IN));
  Serial.print("Value of second flex sensor: ");
  Serial.println(analogRead(PIN_FLEX_2_IN));
  Serial.print("Value of third flex sensor: ");
  Serial.println(analogRead(PIN_FLEX_3_IN));
  Serial.print("Value of forth flex sensor: ");
  Serial.println(analogRead(PIN_FLEX_4_IN));
  Serial.print("Value of fifth flex sensor: ");
  Serial.println(analogRead(PIN_FLEX_5_IN));
  Serial.println("________________________ ");
  delay(5000);

}
