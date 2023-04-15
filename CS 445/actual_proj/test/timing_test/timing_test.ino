#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>
#define NUM_HALLS 4
#define NUM_FLEX_SENS 5
#define TIME_TILL_RECORD 5000
#define DB_POWER -16 // values possible form high power to low: 4, 0, -4, -8, -12, -16, -20, -40

// //Hall PINS Input
// int PIN_HALL_1_IN = ;
// int PIN_HALL_2_IN = ;
// int PIN_HALL_3_IN = ;
// int PIN_HALL_4_IN = ;
// unsigned long startTime;
// int count = 0;
const unsigned long interval = 5000;
// int prev_button_state = LOW;


//flex sensor pins Input
int PIN_FLEX_1_IN = A0;
int PIN_FLEX_2_IN = A1;
int PIN_FLEX_3_IN = A2;
int PIN_FLEX_4_IN = A3;
int PIN_FLEX_5_IN = A4;

//Hall PINS Input
int PIN_HALL_1_IN = 16;
int PIN_HALL_2_IN = 15;
int PIN_HALL_3_IN = 7;
int PIN_HALL_4_IN = 11;

int hall_array[NUM_HALLS] = { PIN_HALL_1_IN, PIN_HALL_2_IN, PIN_HALL_3_IN, PIN_HALL_4_IN };

unsigned long start_time_hall[NUM_HALLS] = { 0, 0, 0, 0 };

// unsigned long curr_time_hall[NUM_HALLS] = {0,0,0,0};
int num_button_press[NUM_HALLS] = { 0, 0, 0, 0 };

int prev_button_state[NUM_HALLS] = { 0, 0, 0, 0 };

int flag_hall[NUM_HALLS] = { 0, 0, 0, 0 };

int flex_sensor_array[NUM_FLEX_SENS] = { PIN_FLEX_1_IN, PIN_FLEX_2_IN, PIN_FLEX_3_IN, PIN_FLEX_4_IN, PIN_FLEX_5_IN };

int flex_sensor_values[NUM_FLEX_SENS] = { 0, 0, 0, 0, 0 };


void pinsetup() {
  for (int i = 0; i < NUM_HALLS; i++) {
    pinMode(hall_array[i], INPUT);
  }

  for (int i = 0; i < 5; i++) {
    pinMode(flex_sensor_array[i], INPUT);
  }
}

void set_flags() {
  for (int i = 0; i < NUM_HALLS; i++) {
    if (digitalRead(hall_array[i]) == HIGH) {
      prev_button_state[i] = HIGH;

      if (start_time_hall[i] == 0) {
        start_time_hall[i] = millis();
      }
    }
  }

  for (int i = 0; i < NUM_HALLS; i++) {
    if (digitalRead(hall_array[i]) == LOW && prev_button_state[i] == HIGH) {
      prev_button_state[i] = LOW;
      num_button_press[i]++;
    }
  }

  for (int i = 0; i < NUM_HALLS; i++) {
    if (millis() - start_time_hall[i] >= interval && start_time_hall[i] != 0) {
      Serial.print("5 seconds have ellapsed and the value of the count for ");
      Serial.print(i);
      Serial.print(" is:");
      Serial.println(num_button_press[i]);
      flag_hall[i] = num_button_press[i];
      num_button_press[i] = 0;
      start_time_hall[i] = 0;
      Serial.println("___________________________");
    }
  }
}
void clear_flags() {
  for (int i = 0; i < NUM_HALLS; i++) {
    flag_hall[i] = 0;
  }
}

void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);
  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));
  // Serial.print("Connected to ");
  // Serial.println(central_name);

  // Serial.println("Please select the 'Neopixels' tab, click 'Connect' and have fun");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinsetup();
  Serial.print("Begin");
  BLEService myService = BLEService(0x1234);
  Bluefruit.begin();
  Bluefruit.setTxPower(DB_POWER);    // Check bluefruit.h for supported values
  Bluefruit.Periph.setConnectCallback(connect_callback);
  bledfu.begin();
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();  
  // Configure and start BLE UART service
  bleuart.begin();
  // Set up and start advertising
  startAdv();
  //remmeber to add grabbing data from flash here
  //  out()
  //  pinsetup();
}



void loop() {

  set_flags();
  switch (flag_hall[0]) {
    case 1:
      Serial.println("Go to Mirroring Mode");
      goto end_loop;
    case 2:
      Serial.println("Turn off");
      goto end_loop;
      //  default:
      //  // do nothing
  }
  for (int i = 1; i < NUM_HALLS; i++) {
    //  Serial.print("The hall value: ");
    //  Serial.println(i);
    switch (flag_hall[i]) {
      case 1:
        Serial.println("Send preset to Arduino ");
        goto end_loop;
      //add code to do that here
      case 2:
        delay(TIME_TILL_RECORD);
        for (int j = 0; j < NUM_FLEX_SENS; j++) {
          flex_sensor_values[j] = analogRead(flex_sensor_array[j]);
          Serial.print("the value of flex sensor ");
          Serial.print(j);
          Serial.print(" is ");
          Serial.println(flex_sensor_values[j]);
        }
        goto end_loop;
      default:
        continue;
    }
  }
end_loop:
  clear_flags();
}