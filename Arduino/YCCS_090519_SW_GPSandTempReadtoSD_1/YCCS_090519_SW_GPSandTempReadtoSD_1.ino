/*
 * CAUTION: MAY RETURN NULL VALUES FOR CERTAIN PIECES OF DATA * 
  
  GPSandTemp_ReadtoSD

  Reads a continuous stream of temperature and gps data in .csv format from a DS18B20 and NEO-6M to an SD card.
  Data only stops reocrded when SD card is pulled form the SD card slot.

  Note: the F() Macro reduces program size more info here: https://www.baldengineer.com/arduino-f-macro.html 
  
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
  *** Additional pullup/down resistors are needed
    GPS Connected as follows:
  ** VCC - 5V
  ** TX - pin 5
  ** RX - pin 3
  ** GND - GND
  *** Additional pullup/down resistors are needed
*/


//SD card libraries
#include <SPI.h>
#include <SD.h>
//Temperature sensor Libraries
#include <OneWire.h>
#include <DallasTemperature.h>
//GPS libraries
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//Set up a file instance
File dataFile;


// Temp Sensor Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass the oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

// initialize variables
float Temperature;

//Set up a GPS instance
TinyGPSPlus gps;
//Set pins 5 rx and 3 tx for the serial communication with the GPS
SoftwareSerial mySerial(5, 3);

void setup() {
  // Open serial communications for debugging
  Serial.begin(9600);

  //Begin software serial port for the GPS
  mySerial.begin(9600);

  // Start the Temp Sensor libraries
  sensors.begin();

  //initializing the SD card
  Serial.print(F("initializing SD card..."));
  if (!SD.begin(4))
  {
    Serial.println(F("initialization failed."));
    while (1);
  }
  Serial.println(F("initialization done."));

  //Removing previous allData Files
  Serial.println(F("Removing allData.csv..."));
  SD.remove("allData.csv");

  //Opening new allData file
  dataFile = SD.open("allData.csv", FILE_WRITE);

  //Creating a string for the column names
  String titleString = "";
  titleString += F("Latitude(deg)");
  titleString += F(",");
  titleString += F("Longitude(deg)");
  titleString += F(",");
  titleString += F("Altitude(')");
  titleString += F(",");
  titleString += F("Temperature(C)");
  titleString += F(",");
  titleString += F("Date(M/D/Y)");
  titleString += F(",");
  titleString += F("Time(H:M:S)");

  //if the data file is available write a title and the column names to it
  if (dataFile)
  {
    Serial.println(F("Writing to allData.csv..."));
    dataFile.println(F("GPS and Temperature Data:"));
    dataFile.println(titleString);
    dataFile.close();
  }
  else
  {
    Serial.println(F("error opening allData.csv"));
  }

}

void loop() {
  //Check for GPS data and encode it, everytime a new sentence is encoded correctly write the data to the SD card
  while (mySerial.available() > 0)
  {
    if (gps.encode(mySerial.read()))
    {
      //Retrieve temperature information and put it into the TEmperature variable
      sensors.requestTemperatures();
      Temperature = sensors.getTempCByIndex(0);

      //create a string for the data and add each piece of incoming data to it, creates one row of data. Commas seperate the columns
      String dataString = "";
      dataString += String( gps.location.lat(), 6);
      dataString += F(",");
      dataString += String( gps.location.lng(), 6);
      dataString += F(",");
      dataString += String( gps.altitude.feet(), 6);
      dataString += F(",");
      dataString += String(Temperature);
      dataString += F(",");
      //add date information in M/D/Y format
      dataString += String(gps.date.month());
      dataString += F("/");
      dataString += String(gps.date.day());
      dataString += F("/");
      dataString += String(gps.date.year());
      dataString += F(",");
      //add time information in H:M:S format
      dataString += String(gps.time.hour());
      dataString += F(":");
      dataString += String(gps.time.minute());
      dataString += F(":");
      dataString += String(gps.time.second());

      //Open the data file
      dataFile = SD.open("allData.csv", FILE_WRITE);

      //if the data file is available write the datastring to it
      if (dataFile)
      {
        dataFile.println(dataString);
        dataFile.close();
        Serial.println(dataString);
      }
      else
      {
        Serial.println(F("error opening allData.csv"));
      }
    }
  }

}
