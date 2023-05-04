#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Adafruit_TinyUSB.h> 
#include <cstring>
#define NUM_HALLS 4
#define NUM_FLEX_SENS 5
#define TIME_TILL_RECORD 3000
#define DB_POWER 4 // values possible form high power to low: 4, 0, -4, -8, -12, -16, -20, -40 was set to 4 before
#define FILENAME "/adafruit.txt"
const unsigned long interval = 3000;

using namespace Adafruit_LittleFS_Namespace;
bool write_flag = false;


// BLE Service
BLEDfu  bledfu;
BLEDis  bledis;
BLEUart bleuart;

//Pointerfinger hall: 17, FLEX: A1 CHECKED
//Middle finger hall: 16, Flex: A2  CHECKED
//Ring finger hall: 15, Flex: A3 CHECKED
//Pinky finger: hall:14 , Flex: A7
//Thumb : flex: A0
//flex sensor pins Input
int PIN_FLEX_1_IN = A0;//thumb
int PIN_FLEX_2_IN = A1;//index
int PIN_FLEX_3_IN = A2;//middle
int PIN_FLEX_4_IN = A3;//ring
int PIN_FLEX_5_IN = A7;//pinky

//Hall PINS Input
int PIN_HALL_1_IN = 16;//17;
int PIN_HALL_2_IN = 15;//16;
int PIN_HALL_3_IN = 7;//15;
int PIN_HALL_4_IN = 11;//14;
File file(InternalFS);

//LED PINS
int RED_LED =A5;
int YELLOW_LED =A4;
int GREEN_LED = 27;

int hall_array[NUM_HALLS] = { PIN_HALL_1_IN, PIN_HALL_2_IN, PIN_HALL_3_IN, PIN_HALL_4_IN };

unsigned long start_time_hall[NUM_HALLS] = { 0, 0, 0, 0 };

// unsigned long curr_time_hall[NUM_HALLS] = {0,0,0,0};
int num_button_press[NUM_HALLS] = { 0, 0, 0, 0 };

int prev_button_state[NUM_HALLS] = { 0, 0, 0, 0 };

int flag_hall[NUM_HALLS] = { 0, 0, 0, 0 };

int flex_sensor_array[NUM_FLEX_SENS] = { PIN_FLEX_1_IN, PIN_FLEX_2_IN, PIN_FLEX_3_IN, PIN_FLEX_4_IN, PIN_FLEX_5_IN };

int flex_sensor_values[NUM_HALLS-1][NUM_FLEX_SENS] = {{ 0, 0, 0, 0, 0 },{ 0, 0, 0, 0, 0 },{ 0, 0, 0, 0, 0 }};

// BLECharacteristic myCharacteristic;

void pinsetup() {
  for (int i = 0; i < NUM_HALLS; i++) {
    pinMode(hall_array[i], INPUT);
  }

  for (int i = 0; i < 5; i++) {
    pinMode(flex_sensor_array[i], INPUT);
  }
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(27,OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(27, LOW);
}

void set_flags() {
  for (int i = 0; i < NUM_HALLS; i++) {
    if (digitalRead(hall_array[i]) == LOW) {
      prev_button_state[i] = HIGH;

      if (start_time_hall[i] == 0) {
        start_time_hall[i] = millis();
      }
    }
  }

  for (int i = 0; i < NUM_HALLS; i++) {
    if (digitalRead(hall_array[i]) == HIGH && prev_button_state[i] == HIGH) {
      prev_button_state[i] = LOW;
      num_button_press[i]++;
    }
  }

  for (int i = 0; i < NUM_HALLS; i++) {
    if (millis() - start_time_hall[i] >= interval && start_time_hall[i] != 0) {
      //Serial.print("5 seconds have ellapsed and the value of the count for ");
      //Serial.print(i);
      //Serial.print(" is:");
      //Serial.println(num_button_press[i]);
      if(num_button_press[1] <=2){
      flag_hall[i] = num_button_press[i];
      }
      num_button_press[i] = 0;
      start_time_hall[i] = 0;
      //Serial.println("___________________________");
    }
  }
}
void clear_flags() {
  for (int i = 0; i < NUM_HALLS; i++) {
    flag_hall[i] = 0;
  }
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}


void BluetoothSetup(){
 // while ( !//Serial ) delay(10);   // for nrf52840 with native usb
  // Config Neopixels

  // Init Bluefruit
  Bluefruit.begin();
  Bluefruit.setTxPower(DB_POWER);    // Check bluefruit.h for supported values maybe set to loewr vlaue later
  //Serial.print("in Bluetooth Setup before connectcallback");
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
  //Serial.print("in connect_callback");
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));
  //Serial.println(central_name);


}
void write_fs(){
  String s = "";
  for (int i = 0; i < NUM_HALLS-1; i++){
    for (int j =0; j < NUM_FLEX_SENS; j++){
      s +=String(flex_sensor_values[i][j])+",";
    }
  }
   if( file.open(FILENAME, FILE_O_WRITE) )
    {
      file.write(s.c_str(),strlen(s.c_str()));
      file.close();
    }
    else
  {
    //Serial.println("Failed!");
  }

}

void clear_fs(){
  file.open(FILENAME, FILE_O_WRITE);
    if(!file.truncate(0)){
      //Serial.println("ERRORS");
    }
    file.close();

}

void read_fs(){

  file.open(FILENAME, FILE_O_READ);

  // file existed
  if ( file )
  {
    //Serial.println(FILENAME " file exists");
    uint32_t readlen;
    char buffer[128] = { 0 };
    readlen = file.read(buffer, sizeof(buffer));
    buffer[readlen] = 0;
    const char deilimeter[] = ",";//arduino stupid
    int i = 0;
    int j = 0;
    char* token = strtok(buffer, deilimeter);
    while(token != NULL && (i < (NUM_HALLS-2) * (NUM_FLEX_SENS-1))){
      if (j >= NUM_FLEX_SENS){
        j=0;
        i++;
      }
      flex_sensor_values[i][j] = atoi(token);
      //Serial.print(flex_sensor_values[i][j]);
      //Serial.print(",");
      token = strtok(NULL,deilimeter);
      j++;
    }
    // //Serial.println("Below is what is in the array after reaidng it in");
    // for (int k =0; k < (NUM_HALLS-1); k++){
    //   for (int l = 0; l < NUM_FLEX_SENS; l++){
    //     //Serial.print(" ");
    //     //Serial.print(flex_sensor_values[k][l]);
    //     //Serial.print(",");
    //   }
    // }
    file.close();
    // delay(100000000);
  }
  else{
    //Serial.print("Open " FILENAME " file to write ... ");

    if( file.open(FILENAME, FILE_O_WRITE) )
    {
      const char* contents = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,";
      //Serial.println("OK");
      file.write(contents, strlen(contents));
      file.close();
    }
    // else
    // {
    //   //Serial.println("Failed!");
    // }
  }

    //Serial.println("Done");
  }

void fs(){
 // while ( !Serial ) delay(10);   // for nrf52840 with native usb

  //Serial.println("Internal Read Write File Example");
  //Serial.println();

  // Wait for user input to run. Otherwise the code will 
  // always run immediately after flash and create the FILENAME in advance
  // //Serial.print("Enter to any keys to continue:");
  //Serial.println();
  //Serial.println();

  // Initialize Internal File System
  InternalFS.begin();
  //Serial.println("Opening File");
  read_fs();
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BluetoothSetup();
  pinsetup();
  fs();
  //Serial.print("Begin");
  
  //remmeber to add grabbing data from flash here
  //  out()
  //  pinsetup();
}

void loop() {

  set_flags();
  switch (flag_hall[0]) {
    case 1:
    {
      //Serial.println("Go to Mirroring Mode");
      
      clear_flags();
      unsigned long curr_time = millis();
      while((flag_hall[0]==0 || flag_hall[0] == 1) && flag_hall[1] == 0 && flag_hall[2] == 0 && flag_hall[3]==0){
        digitalWrite(RED_LED, HIGH);    
        set_flags();
        String str;
        for (int j = 0; j < NUM_FLEX_SENS; j++) {
          //Serial.print("the value of flex sensor ");
          //Serial.print(j);
          //Serial.print(" is ");
          uint32_t flex_sens = analogRead(flex_sensor_array[j]);
          //Serial.println(flex_sens);
          str += String(flex_sens)+",";
        }
        // str+="\n";
        Serial.print("before advertise: ");
        Serial.println(millis()-curr_time);
        bleuart.write(str.c_str(), str.length());
        Serial.print("after advertised: ");
        Serial.println(millis()-curr_time);
        unsigned long prev = millis();
        while(millis()-prev<30){ // transmitting mirroring mode rate
          set_flags();
        }
        set_flags();
       curr_time = millis(); 
      }
      break;
    }
    case 2:
      //Serial.println("Turn off");
      goto end_loop;
      //  default:
      //  // do nothing
  }
  for (int i = 1; i < NUM_HALLS; i++) {
    //  //Serial.print("The hall value: ");
    //  //Serial.println(i);
    switch (flag_hall[i]) {
      case 1:{
        digitalWrite(YELLOW_LED, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED, LOW);
        delay(500);
        digitalWrite(YELLOW_LED, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED, LOW);
        delay(500);
        digitalWrite(YELLOW_LED, HIGH);
        delay(500);
        // bleuart.write("these are presets");
        //Serial.println("Send preset to Arduino ");
        String str_2;
        for (int j = 0; j < NUM_FLEX_SENS; j++) {
          //Serial.print("the value of flex sensor ");
          //Serial.print(j);
          //Serial.print(" is ");
          //Serial.println(flex_sensor_values[i-1][j]);
          str_2 += String(flex_sensor_values[i-1][j])+",";
        }
        // str_2 +="/n";
        bleuart.write(str_2.c_str(), str_2.length());
        // delay(3000);
        goto end_loop;
      }
      case 2:{
        digitalWrite(YELLOW_LED, HIGH);
        delay(TIME_TILL_RECORD);
        write_flag = true; 
        // bleuart.write("these are the values");
        String str_3;
        for (int j = 0; j < NUM_FLEX_SENS; j++) {
          flex_sensor_values[i-1][j] = analogRead(flex_sensor_array[j]);
          //Serial.print("the value of flex sensor ");
          //Serial.print(j);
          //Serial.print(" is ");
          //Serial.println(flex_sensor_values[i-1][j]);
          str_3 += String(flex_sensor_values[i-1][j])+",";
        }
        // str_3+="/n";
          bleuart.write(str_3.c_str(), str_3.length());
        goto end_loop;
      }
      default:
        continue;
    }
  }
  end_loop:
  if(write_flag == true){
  clear_fs();
  write_fs();
  write_flag = false; 
  }
  clear_flags();
}
