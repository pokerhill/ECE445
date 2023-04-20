#include <ArduinoBLE.h>

// UUID of the UART service and characteristic
const char* deviceServiceUuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";//6e400001-b5a3-f393-e0a9-e50e24dcca9e
const char* CHARACTERISTIC_UUID = "6E400006-B5A3-F393-E0A9-E50E24DCCA9E";

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }
  BLE.setLocalName("Nano 33 BLE (Central)"); 
  BLE.advertise();
  // BLE.scanForName("Bluefruit Feather52");
}
// void loop() {
//   int count = BLE.scan();
//   Serial.print("Found ");
//   Serial.print(count);
//   Serial.println(" devices.");

//   for (int i = 0; i < count; i++) {
//     BLEDevice peripheral = BLE.available();
//     if (peripheral) {
//       Serial.print("UUID: ");
//       Serial.println(peripheral.advertisedServiceUuid());
//       Serial.print("Address: ");
//       Serial.println(peripheral.address());
//       Serial.print("device name: ");
//       Serial.print(peripheral.deviceName());
//       Serial.println("-----------------------");
//     }
//   }

//   delay(5000);
// }

// void loop() { 
//   Serial.println("looking for periperal");
//   BLEDevice peripheral;
//   do{
//     BLE.scanForAddress("e5:74:ef:18:90:3b");
//     peripheral = BLE.available();
//   }while(!peripheral);
//   if (peripheral) {
//     Serial.print("Connecting to ");
//     Serial.println(peripheral.address());
//     Serial.print("name");
//     Serial.println(peripheral.deviceName());
//     Serial.println(peripheral.advertisedServiceUuid());
//     if(peripheral.discoverAttributes()){
//       Serial.println("attributes are discoverable");
//     }
//     else{
//       Serial.println("no attribute ");
//     }
//     BLE.stopScan();
  
//     if (peripheral.connect()) {
//       Serial.println("Connected to peripheral");
//       BLEService uartService;
//        uartService = peripheral.service("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
//       Serial.println("Trying to connect to service");
//       if(!uartService){
//         Serial.println("service dne");
//       }
//       else{
//         Serial.println("Serive EXISTS YAY");
//       }
//       BLECharacteristic txChar = uartService.characteristic(CHARACTERISTIC_UUID);
//       // if(!txChar){
//       //   Serial.println("Perephirla device doesn't have readable characterisitc");
//       //   peripheral.disconnect();
//       //   return;
//       // }
//       if(txChar.canRead()){
//         Serial.println("CAN be read to");
//       }
//       else{
//         Serial.println("CAN NOT be read to");
//       }
//       while (peripheral.connected()) {
//         // Serial.println("inside while");
//         if (txChar.written()){ 
//           String message = (const char*) txChar.value();
//           Serial.print("Received: ");
//           Serial.println(message);
//         }
//       }
//       Serial.println("Disconnected from peripheral");
//       peripheral.disconnect();
//     } else {
//       Serial.println("Failed to connect to peripheral");
//     }
//   }
// }
void loop(){
  connectToPeripheral();
}
void connectToPeripheral(){
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }
  if(peripheral.hasService(deviceServiceUuid)){
    Serial.println("service exists");
  }
  else{
    Serial.println("service doesn't exists");
  }
  BLEService service = peripheral.service(0);
  if (service){
    Serial.println("service working");
  }
  else{
    Serial.println("Service not working");
  }
  BLECharacteristic txChar = service.characteristic(CHARACTERISTIC_UUID);
  if(txChar.subscribe()){
     Serial.println("Subscribed to characteristic.");
  }
  else{
Serial.println("Not Subscribed to characteristic.");
  }

  if (txChar){
    Serial.println("exsists");
  }
  else{
    Serial.println("txChar doesn't exist");
  }
  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }
}
