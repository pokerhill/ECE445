#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Adafruit_TinyUSB.h> // for Serial

using namespace Adafruit_LittleFS_Namespace;

#define FILENAME    "/adafruit.txt"
#define CONTENTS    "Adafruit Little File System test file contents"

File file(InternalFS);

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Internal Read Write File Example");
  Serial.println();

  // Wait for user input to run. Otherwise the code will 
  // always run immediately after flash and create the FILENAME in advance
  // Serial.print("Enter to any keys to continue:");
  Serial.println();
  Serial.println();

  // Initialize Internal File System
  InternalFS.begin();
  Serial.println("Opening File");
  file.open(FILENAME, FILE_O_READ);

  // file existed
  if ( file )
  {
    Serial.println(FILENAME " file exists");
    
    uint32_t readlen;
    char buffer[128] = { 0 };
    readlen = file.read(buffer, sizeof(buffer));
    buffer[readlen] = 0;
    Serial.println(buffer);
    file.close();
    Serial.println("Writing new values");
    file.open(FILENAME, FILE_O_WRITE);
    if(file.truncate(0)){
      Serial.println("should make file to length 0");
      Serial.print("this is the size: ");
      Serial.println(file.size());
    }
    else{
      Serial.println("ERRORS");
    }
    file.close();
    file.open(FILENAME, FILE_O_WRITE);
    const char * values = "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,";
    file.write(values,strlen(values));
    Serial.print("error above if not printed if printed below");
    file.close();
    file.open(FILENAME, FILE_O_READ);
    char buffer2[128] = { 0 };
    readlen = file.read(buffer2, sizeof(buffer2));
    buffer2[readlen] = 0;
    Serial.println("data after writing");
    Serial.println(buffer2);
    file.close();
  }else
  {
    Serial.print("Open " FILENAME " file to write ... ");

    if( file.open(FILENAME, FILE_O_WRITE) )
    {
      Serial.println("OK");
      file.write(CONTENTS, strlen(CONTENTS));
      file.close();
    }else
    {
      Serial.println("Failed!");
    }
  }

  Serial.println("Done");
}

// the loop function runs over and over again forever
void loop() 
{
}