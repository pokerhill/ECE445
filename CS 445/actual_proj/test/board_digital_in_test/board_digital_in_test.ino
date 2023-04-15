#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>
#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

void setup() {
  Serial.begin(115200);
  pinMode(LED_CONN, OUTPUT);
  pinMode(11, INPUT);
  Serial.println("setting up");
}

void loop() {
  if(digitalRead(11) == HIGH){
    digitalWrite(LED_CONN, LOW);
    Serial.println("pressed");
  }
  else {
    digitalWrite(LED_CONN, HIGH);
    Serial.println("not pressed");
  }
  delay(1000);

}