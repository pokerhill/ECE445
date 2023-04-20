#include <ArduinoBLE.h>
 BLECharacteristic tx;
 BLEDevice peripheral;
 BLEService service;
void connect_ble(){
   do {
    int count = BLE.scanForUuid("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    Serial.print("Found ");
    Serial.print(count);
    Serial.println(" peripherals with UUID.");
    peripheral = BLE.available();
    Serial.println("looking for peripheral to connect");
  }while(!peripheral);
  if (peripheral) {
    Serial.print("Connecting to ");
    Serial.println(peripheral.address());
    Serial.print("Local name: ");
    Serial.println(peripheral.localName());
    Serial.print("advertised service: ");
    Serial.println(peripheral.advertisedServiceUuid());
    BLE.stopScan();
    boolean connect = peripheral.connect();
    do{
      Serial.print("Try connect ");
      connect = peripheral.connect();
    }while(!connect);
    Serial.println("im out les go");
    if(peripheral.discoverService("6e400001-b5a3-f393-e0a9-e50e24dcca9e")){ // 6e400001-b5a3-f393-e0a9-e50e24dcca9e
    service = peripheral.service("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    }
    else{
      Serial.println("service DOESN'T WORK");
    }
    tx = service.characteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");//6E400003-B5A3-F393-E0A9-E50E24DCCA9E
    if(!tx)
    // while(!tx){
    //   Serial.println("trying to connect ot characteristic");
    //    tx = service.characteristic("6E400006-B5A3-F393-E0A9-E50E24DCCA9E");
    // }
    Serial.print("this is subscribable: ");
    Serial.println(tx.canSubscribe());
    if(tx){
      Serial.print("tx exists");
    }
    else{
      Serial.print("tx DOESN'T EXISITS");
    }
    while(!tx.subscribe()){
      Serial.println("trying to subscribe");
    }
  } 
}
void setup() {
  Serial.begin(9600);
  while (!Serial);
      if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE.");
    while (1);
  }
  connect_ble();
  
 

}

void loop() {
  if(tx.canRead()){
    Serial.print("readable");
  }
  else{
    Serial.print("not readable");
  }
  if(tx.canWrite()){
    Serial.print("writable");

  }
  else{
    Serial.print("not writable");
  }
  while(peripheral.connected()){
    if(tx.valueUpdated()){
      Serial.print("inside if while");
          char val[20]={0} ;
          tx.readValue(val,20);
          Serial.print("Received: ");
          Serial.println(String(val));
          // for (int i =0; i < 20; i++){
          //   Serial.print(value[i]);
          //   Serial.print(", ");
          // }
    // }
  }
  }
  if(!peripheral.connected()){
    Serial.println("disconnected");
  }


  // if (Serial.available()) {
  //   char incoming = Serial.read();
  //   Serial.println(incoming);
  // }

  // if (BLE.available()) {
  //   char incoming = BLE.read();
  //   Serial.println(incoming);
  // }
}
