#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Adafruit_TinyUSB.h> 
#include <cstring>
#define NUM_HALLS 4
#define NUM_FLEX_SENS 5
#define TIME_TILL_RECORD 5000
#define DB_POWER -12 // values possible form high power to low: 4, 0, -4, -8, -12, -16, -20, -40 was set to 4 before
#define FILENAME "/adafruit.txt"
#define PIZZO_SPEAKER 13

using namespace Adafruit_LittleFS_Namespace;
bool write_flag = false;

const unsigned long interval = 5000;
// int prev_button_state = LOW;

//buzzer is pin 13, MOSI

//Pointerfinger hall: 17, FLEX: A1 CHECKED
//Middle finger hall: 16, Flex: A2  CHECKED
//Ring finger hall: 15, Flex: A3 CHECKED
//Pinky finger: hall:14 , Flex: A7
//Thumb : flex: A0
// BLE Service
BLEDfu  bledfu;
BLEDis  bledis;
BLEUart bleuart;

//flex sensor pins Input
int PIN_FLEX = A7;//thumb
int PIN_FLEX_2 = A0;


//Hall PINS Input
int PIN_HALL_IN = 14;



// BLECharacteristic myCharacteristic;
void pinsetup() {
    pinMode(PIN_FLEX, INPUT);
    pinMode(PIN_HALL_IN, INPUT);
    pinMode(PIN_FLEX_2, INPUT);
  // pinMode(RED_LED, OUTPUT);
  // pinMode(YELLOW_LED, OUTPUT);
  // pinMode(GREEN_LED, OUTPUT);
  // digitalWrite(RED_LED, LOW);
  // digitalWrite(YELLOW_LED, LOW);
  // digitalWrite(GREEN_LED, LOW);
}



void BluetoothSetup(){
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  // Config Neopixels

  // Init Bluefruit
  Bluefruit.begin();
  Bluefruit.setTxPower(DB_POWER);    // Check bluefruit.h for supported values maybe set to loewr vlaue later
  Serial.print("in Bluetooth Setup before connectcallback");
  Bluefruit.Periph.setConnectCallback(connect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();  

  // Configure and start BLE UART service
  
  bleuart.begin();

  // Set up and start advertising
  startAdv();
}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  
  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);


  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  Serial.print("in connect_callback");
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));
  Serial.println(central_name);


}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BluetoothSetup();
  pinsetup();
  Serial.print("Begin");
}

void loop() {
  String data_send = String(analogRead(PIN_FLEX)) + ", " + String(digitalRead(PIN_HALL_IN))+", " + String(analogRead(PIN_FLEX_2)) + "\n";
  bleuart.write(data_send.c_str(), data_send.length());
  
}
