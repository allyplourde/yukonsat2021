//Find the libraries at this link
//https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806

//libraries for reading in from onewire (the middle sensor pin) and a library specific for the temp sensor
#include <OneWire.h>
#include <DallasTemperature.h>

//defines the sensor input into pin 2 on the arduino
#define ONE_WIRE_BUS 2    

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//sets digital pins 5 and 6 as pins for the leds
const int LEDPin1 = 5;
const int LEDPin2 = 6;

//sets Temperature as a (float) variable
float Temperature;   

void setup() {
  //begins serial monitor
  Serial.begin(9600);   
  
  //defines led pins and sets them to off
  pinMode(LEDPin1, OUTPUT);    
  pinMode(LEDPin2, OUTPUT);
  digitalWrite(LEDPin1, LOW);
  digitalWrite(LEDPin2, LOW);

  //begins the onewire and dallas libraries
  sensors.begin();    
}

void loop() {
  // Send the command to get temperature readings to temp sensor
  sensors.requestTemperatures();

  //Print out temp to serial monitor
  Serial.print("Temperature is: "); 
  Serial.print(sensors.getTempCByIndex(0));   

  //Read temp value in and assign it to Temperature 
  Temperature = sensors.getTempCByIndex(0);

  //if the temp is below 22 degrees the leds alternate slowly
  if(Temperature < 22){   
    digitalWrite(LEDPin1, HIGH);
    digitalWrite(LEDPin2, LOW);
    delay(750);
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, HIGH);
  }
  
  // if the temp is above 22 degrees the leds alternate quickly
  if(Temperature > 22){                     
    digitalWrite(LEDPin1, HIGH);
    digitalWrite(LEDPin2, LOW);
    delay(250);
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, HIGH);
  }  
    
}
