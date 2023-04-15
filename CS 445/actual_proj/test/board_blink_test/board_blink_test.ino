// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
// #include <bluefruit.h>

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   pinMode(16, INPUT);

// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   Serial.print("hello");
//   Serial.print(digitalRead(16));
//   delay(1000);
// }
#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}