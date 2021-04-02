/*
  TempDataSDRead_FixedTime

  Reads a continuous stream of temperature data in .csv format from a DS18B20 to an SD card.
  Data only stops reocrded when SD card is pulled form the SD card slot.

  The circuit:
    SD card attached to SPI bus as follows:
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 4
    Temp sensor connected as follows:
  ** VCC - 5V
  ** DQ (Data) - pin 2
  ** GND - GND
*/

// SD card libraries
#include <SPI.h>
#include <SD.h>

// Temperature Sensor libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Set up a file instance
File dataFile;

// Temp Sensor Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

// initialize variables
float Temperature;
int i;


void setup() {
  // Open serial communications
  Serial.begin(9600);

  // Start the Temp Sensor libraries
  sensors.begin();


  //initializing the SD card
  Serial.print("initializing SD card...");
  Serial.println("card initialized.");
  if (!SD.begin(4))
  {
    Serial.println("initialization failed.");
    while (1);
  }
  Serial.println("initialization done.");

  //Removing previous TempData Files
  Serial.println("Removing TempData.csv...");
  SD.remove("TempData.csv");

  //Opening new TempData file
  dataFile = SD.open("TempData.csv", FILE_WRITE);

  // If the file is available write a title at the top of the file
  if (dataFile)
  {
    Serial.println("Writing to TempData.csv...");
    dataFile.println("Temperature Data:");
    dataFile.close();
  }
  else
  {
    Serial.println("error opening TempData.csv");
  }


}

void loop() {
  // Retrieve temperature information and put it into a string
  sensors.requestTemperatures();
  Temperature = sensors.getTempCByIndex(0);
  String dataString = "";
  dataString = String(Temperature);

  // open the file
  dataFile = SD.open("TempData.csv", FILE_WRITE);

  // if the file is available, write the Temperature data (datastring) to it
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else
  {
    Serial.println("error opening TempData.csv");
  }

}
