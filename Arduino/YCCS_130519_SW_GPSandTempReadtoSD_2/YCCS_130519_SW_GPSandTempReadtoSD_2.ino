/*
  GPSandTemp_ReadtoSD

  Reads a continuous stream of temperature and gps data in .csv format from a DS18B20 and adafruit ultimate gps breakou to an SD card (saves file as a .txt)
  Data only stops recording when a button is pressed.

  ----- MAKE SURE THE NEOGPS LIBRARY IS CONFIGURED -------
  See libraries folder in yukonsat repo for configured files
  More info on configuration can be found here: https://github.com/SlashDevin/NeoGPS

  Note: the F() Macro reduces program size more info here: https://www.baldengineer.com/arduino-f-macro.html

  The circuit:
      SD card attached to SPI bus as follows:
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 4
  *** Pin 10 will also be occupied if using arduino ethernet shield
    Temp sensor connected as follows:
  ** VCC - 5V
  ** DQ (Data) - pin 2
  ** GND - GND
  *** Additional pullup/down resistors are needed
    GPS Connected as follows:
  ** VCC - 5V
  ** TX - pin 0 (rx)
  ** RX - pin 1 (tx)
  ** GND - GND
  *** Additional pullup/down resistors are needed
*/

//SD card using an arduino ethernet shield
#include <SdFat.h>
#include <SPI.h>
SdFat SD; // Creates an SD instance
File  gpsDataFile; // Data object you will write your sensor data to
const char *gpsFilename = "GPSData.txt"; // Sets File name
const int8_t DISABLE_CHIP_SELECT = 10; // Disables pin 10 (ethernet chip select) on arduino ethernet shield
int chipSelect = 4; // SD chip select pin set to pin 4 (standard with arduino ethernet shield)
#include <NeoHWSerial.h>
#define gpsPort NeoSerial // GPS communication coming through the (Neo)Hardware Serial ports

//NeoGPS
#include <NMEAGPS.h>
NMEAGPS GPS; // set up an NMEAGPS instance
gps_fix fix; // set up a fix object

//Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3 // Temp Sensor Data wire is plugged into pin 3 on the Arduino
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature

const int buttonPin = 2; // define button input pin

void setup()
{
  gpsPort.begin( 9600 ); // begin the gpsport (NeoSerial)

  GPS.send_P( &gpsPort, F("PGCMD,33,0") );  //Turn off antenna update nuisance data
  GPS.send_P( &gpsPort, F("PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0") ); //Request RMC and GGA Sentences only
  GPS.send_P( &gpsPort, F("PMTK220,1000") ); //Set update rate to 1 hz

  if (!SD.begin(chipSelect)) { // Begin SD card connection if not present send error message to serial monitor
    NeoSerial.println( F("No SD card/reader found!") );
    for (;;); // hang here!
  }

  if (SD.exists(gpsFilename)) { //Delete old data files to start fresh
    SD.remove(gpsFilename);
  }

  gpsDataFile = SD.open(gpsFilename, FILE_WRITE); // Opens file on Sd card

  gpsDataFile.println( F("Lat(deg),Lng(deg),Alt(m),Temp(C),Date(M/D/Y),Time(H:M:S)") ); // Print header (column names) to file

  pinMode(buttonPin, INPUT); //buttonPin will receive

  sensors.begin(); // Begin Temp sensors
}


void loop()
{
  // Read characters and parse into a fix structure
  if (GPS.available( gpsPort )) {

    fix = GPS.read(); // Get the new fix data (GPS quiet time just started)

    sensors.requestTemperatures(); // Request temperatures

    // Only save data if we have a fix
    if (fix.valid.status && (fix.status >= gps_fix::STATUS_STD)) {

      //Print data to file in rows in accordance to the above column names in setup()
      gpsDataFile.print( fix.latitude(), 5 );
      gpsDataFile.print( F(",") );
      gpsDataFile.print( fix.longitude(), 5);
      gpsDataFile.print( F(",") );
      gpsDataFile.print( fix.altitude() );
      gpsDataFile.print( F(",") );
      gpsDataFile.print( sensors.getTempCByIndex(0) );
      gpsDataFile.print( F(",") );
      gpsDataFile.print( fix.dateTime.month );
      gpsDataFile.print( F("/"));
      gpsDataFile.print( fix.dateTime.date );
      gpsDataFile.print( F("/"));
      gpsDataFile.print( fix.dateTime.year );
      gpsDataFile.print( F(",") );
      gpsDataFile.print( fix.dateTime.hours );
      gpsDataFile.print( F(":") );
      gpsDataFile.print( fix.dateTime.minutes );
      gpsDataFile.print( F(":") );
      gpsDataFile.println( fix.dateTime.seconds );
      gpsDataFile.flush();

      NeoSerial.println("recording...");
    }
  }

  // if the button is pressed close the file
  if (digitalRead(buttonPin) == HIGH) {
    gpsDataFile.close();
    NeoSerial.println("file closed");
    while (true) { // forever
      ;
    }
  }

} // loop
