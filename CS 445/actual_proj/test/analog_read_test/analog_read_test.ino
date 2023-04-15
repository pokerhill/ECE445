#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>
#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h> // for Serial
#endif
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(2);
  Serial.println(sensorValue);
  delay(10);

}
