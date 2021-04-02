#include "sensors.h"
#include "camera.h"
#include "storage.h"
#include "screen.h"

#include <SPI.h>
#include <SD.h>

// ArduinoUno Digital Pin Set Up:
// The SCK, MISO, and MOSI, pins of the components must
// be connected to the following digital pins; multiple
// components can be connected to these pins in series.
// SCK to pin 13
// MISO to pin 12
// MOSI to pin 11
// The chip select (CS) pins for the individual
// components must be connected to the digial
// pins indicated below.
#define SD_CARD_PIN 10  
#define IMU_PIN 9
#define MAG_PIN 8
//#define ADC_PIN 3
#define CAMERA_PIN 7
#define SCREEN_PIN 5


// Component class definition
Camera camera(CAMERA_PIN, SD_CARD_PIN);
//Sensors sensors(ADC_PIN);
Storage storage(SD_CARD_PIN);
Screen screen(SCREEN_PIN, SD_CARD_PIN);

// Set the components you would like to use to 'true'
// The program will skip over any code related to 
// a component not in use. This should allow you to 
// test and modify the program with only a subset of 
// the avaialble devices.
bool test_camera = false;  // ArduCam
bool test_sensors = false; // ADC converter 
bool test_sd_card = true; // SD Card reader
bool test_screen = true;  // OLED display

//Other Variables
char inChar;
String image_bytes = "";
int LED = 2;
byte ACK;

void setup() {
    // This is where each component is initialized
    // Components will only be initialized if its
    // designated global variable has been set to true.

    pinMode(LED, OUTPUT);
    
    Serial.begin(9600);
    
    Serial.println("Welcome to YukonSat Payload!");
    pinMode(10, OUTPUT); // change this to 53 if you are using arduino mega
    digitalWrite(10, HIGH); // Add this line

    // initialize ADC converter 
    if (test_sensors) {
//      sensors.init();
    };

    // initialize SD Card reader
    if (test_sd_card) {
      storage.init();
    }

    // initialize camera
    if (test_camera) {
      camera.init(); 
    }

    // initialize screen
    if (test_screen) {
      screen.init();
    }
    Serial.println("Setup Complete");
}//end Setup

void loop() {  
  handleSerial(); // handle serial commands
}

void handleSerial() {
 // check for incoming characters
  if (Serial.available() > 0) { 
    Serial.print("New Input Recieved: ");
    char incomingCharacter = Serial.read(); // store incoming command
    Serial.println(incomingCharacter);
    // the switch block executes the function related to the 
    // incoming command entered on the serial monitor
    switch (incomingCharacter) {   
            
      case 'a': //OBC sends image to payload
        Serial.println("Case a: loading image to sd card....");
        digitalWrite(LED, HIGH);
        storage.uploadImage();
        digitalWrite(LED, LOW);
        break;    
         
      case 'b':
        Serial.println("Case b");
        break;
      
      case 'c':// Capture an image
        Serial.println("Case c");
        digitalWrite(LED, HIGH);
        screen.loadImage(); // loads image from SD card onto display, if no images are in the queue the default yukonsat logo should be displayed
        //camera.snapshot();  // takes a snapshot and stores on sd card
        delay(2000);
        //storage.updateImageQueue();  // removes last display image from memory and queus up next image to be displayed
        //camera.off();
        screen.off();
        digitalWrite(LED, LOW);
        break;
        
      case 'd':
        Serial.println("Case d");
        break;
        
      case 'e':
        Serial.println("Case e");
        break;
        
      // Transfer image from payload to OBC
      case 'f':
        // functions to access images/ convert to desired format and send
        Serial.println("Case f");
        break;
        
      case 'g':
        Serial.println("Case g");
        break;
        
      case 'h':
        Serial.println("Case h");
        break;
        
      // send OBC IMU data
      case 'i':
        Serial.println("Case i");
        // functions to open IMU data (stored as txt file on sd card)
        //file = storage.getIMU();
        //dataToSend = convertTxtToBytes(file);
        break;
        
      // send OBC analog sensor readings
      case 'j':
        Serial.println("Case j");
        // functions to open analog data (stored as txt file on sd card)
        //file = storage.getAnalog();
        //dataToSend = convertTxtToBytes(file);
        break;
      case 't':
        Serial.println("Case t");
        break;

      //testing function, LED wired to digital pin 2
      case 'x':
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        break;
      
    }//end switch block
    
  } else {
    // Payload in default mode. Duty Cycles for data
    // collection will go here. 
    Serial.println("Data Collection Mode");
    delay(2000);
    //functions to collect/process/ and store data
  }
}//end handleSerial
