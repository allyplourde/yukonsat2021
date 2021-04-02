#include "storage.h"

#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

File file;


Storage::Storage(byte cs_pin) {
  this->cs_pin = cs_pin;
}
void Storage::init() {
  Serial.print("\nInitializing SD card...");
  //SD.begin(cs_pin);
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!SD.begin(cs_pin)) {
    Serial.println("SD Card initialization failed.");
  } else {
    Serial.println("SD Card initialization was successful!");
  }

}


byte Storage::storeDataString(String data, String file_name) {
  byte ACK;
  file = SD.open(file_name, FILE_WRITE);
  // if the file opened okay, write to it:
  if (file) {
    file.println(data);
    // close the file:
    file.close();
    ACK = 1;
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(file_name);
    ACK = 0;
  }
  return ACK;
}

void Storage::uploadImage(){
  // functions to read and store image
  //TODO: determine image size and read in appropriate number of bytes
  //TODO: break into chunks
  File in = SD.open("profile.bmp", FILE_WRITE);
  long byteCount = 0;
  
  while(Serial.available() == 0){
    Serial.println("waiting....");
  }
        
  long lastData = millis();
    
  while( lastData + 1000 > millis()){
    if(Serial.available()) {
      lastData = millis();
      byte byt = Serial.read();
      if(byt!= -1) {
        in.write(byt);  
          byteCount++;  
      }
    }
  }
      
  in.flush();
  Serial.println();
  Serial.print(in.size());
  Serial.println("f");
  Serial.print(byteCount);
  Serial.println("i");
     
  in.close();
  delay(500);
  
}
