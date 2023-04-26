#include <ArduinoBLE.h>
#include <Servo.h>
#define BLUE_LED D10
 BLECharacteristic tx;
 BLEDevice peripheral;
 BLEService service;
 #define NUM_FLEX_SENSORS 5
int flex_sensor_values[NUM_FLEX_SENSORS] = {0,0,0,0,0};
Servo thumb;
Servo index_finger;
Servo middle;
Servo ring;
Servo pinky;
Servo servoArray[] = { thumb, index_finger, middle, ring, pinky };
void hand_setup(){
  thumb.attach(D6);  // attach servo to pin 9
  index_finger.attach(D5);
  middle.attach(D4);
  ring.attach(D3);
  pinky.attach(D2);
  //Serial.println("Beginning");
}
void connect_ble(){
  do {
   do {
    int count = BLE.scanForUuid("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    //Serial.print("Found ");
    //Serial.print(count);
    //Serial.println(" peripherals with UUID.");
    peripheral = BLE.available();
    //Serial.println("looking for peripheral to connect");
  }while(!peripheral);
  if (peripheral) {
    //Serial.print("Connecting to ");
    //Serial.println(peripheral.address());
    //Serial.print("Local name: ");
    //Serial.println(peripheral.localName());
    //Serial.print("advertised service: ");
    //Serial.println(peripheral.advertisedServiceUuid());
    BLE.stopScan();
    boolean connect = peripheral.connect();
    do{
      //Serial.print("Try connect ");
      connect = peripheral.connect();
    }while(!connect);
    //Serial.println("im out les go");
    if(peripheral.discoverService("6e400001-b5a3-f393-e0a9-e50e24dcca9e")){ // 6e400001-b5a3-f393-e0a9-e50e24dcca9e
    service = peripheral.service("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    }
    else{
      //Serial.println("service DOESN'T WORK");
    }
    tx = service.characteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");//6E400003-B5A3-F393-E0A9-E50E24DCCA9E
    // while(!tx){
    //   //Serial.println("trying to connect ot characteristic");
    //    tx = service.characteristic("6E400006-B5A3-F393-E0A9-E50E24DCCA9E");
    // }
    //Serial.print("this is subscribable: ");
    //Serial.println(tx.canSubscribe());
    if(tx){
      //Serial.print("tx exists");
    }
    else{
      //Serial.print("tx DOESN'T EXISITS");
    }
    if(!tx.subscribe()){
      peripheral.disconnect();
    }
  }
  }while(!tx.subscribe());

}
void setup() {
  //Serial.begin(9600);
  // while (!//Serial);
      if (!BLE.begin()) {
    //Serial.println("Failed to initialize BLE.");
    while (1);
  }
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);
  connect_ble();
  digitalWrite(BLUE_LED, HIGH);
  hand_setup();
}

void loop() {

  if(tx.canRead()){
    //Serial.print("readable");
  }
  else{
    //Serial.print("not readable");
  }
  if(tx.canWrite()){
    //Serial.print("writable");

  }
  else{
    //Serial.print("not writable");
  }
  while(peripheral.connected()){
    if(tx.valueUpdated()){
      //Serial.print("inside if while");
          char val[20]={0} ;
          tx.readValue(val,20);
          //Serial.print("Received: ");
          // //Serial.println(String(val));
          const char deilimeter[] = ",";//arduino stupid
          int i = 0;
          char* token = strtok(val, deilimeter);
          while(token != NULL && (i < NUM_FLEX_SENSORS)){
            flex_sensor_values[i] = atoi(token);
            //Serial.print(flex_sensor_values[i]);
            //Serial.println(",");
            token = strtok(NULL,deilimeter);
            i++;
          }
          //Serial.println("these are the servo values");
          servoArray[0].write(constrain(map(flex_sensor_values[0],310, 540, 15,145),15,145));
          servoArray[1].write(constrain(map(flex_sensor_values[1],260, 570, 0,145),0,145));
          servoArray[2].write(constrain(map(flex_sensor_values[2],360, 635, 15,150),15,150));
          servoArray[3].write(constrain(map(flex_sensor_values[3],340, 618, 8,140),8,140));
          servoArray[4].write(constrain(map(flex_sensor_values[4],356, 595, 26,140),26,140));
          //thumb min: 15 middle: 40 max:145
          //index min:0 middle: 25 max 145
          // middle min:15  middle: 25  max:150
          // ring min:5 middle: 25 max: 135
          //pink min: 50 middle: 45 max:140 
       }
    }
  if(!peripheral.connected()){
    //Serial.println("disconnected");
  }


}
