// Basic demo for magnetometer readings from Adafruit magnetometer


#include "magnetometer.h"

Magnetometer magnetometer;
#define CLK 13
#define MISO 12
#define MOSI 11
#define magnetometer_CS 8

void setup(void) {
  Serial.begin(115200);

  Serial.println("Adafruit magnetometer test!");
  
  if (! magnetometer.init(magnetometer_CS)) {  // hardware SPI mode
  //if (! magnetometer.begin_SPI(magnetometer_CS, magnetometer_CLK, magnetometer_MISO, magnetometer_MOSI)) { // soft SPI
    Serial.println("Failed to find magnetometer chip");
    while (1) { delay(10); }
  }
  
  Serial.println("magnetometer Found!");
/*
  magnetometer.setPerformanceMode(magnetometer_MEDIUMMODE);
  Serial.print("Performance mode set to: ");
  switch (magnetometer.getPerformanceMode()) {
    case magnetometer_LOWPOWERMODE: Serial.println("Low"); break;
    case magnetometer_MEDIUMMODE: Serial.println("Medium"); break;
    case magnetometer_HIGHMODE: Serial.println("High"); break;
    case magnetometer_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  magnetometer.setOperationMode(magnetometer_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (magnetometer.getOperationMode()) {
    case magnetometer_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case magnetometer_SINGLEMODE: Serial.println("Single mode"); break;
    case magnetometer_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  magnetometer.setDataRate(magnetometer_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (magnetometer.getDataRate()) {
    case magnetometer_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case magnetometer_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case magnetometer_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case magnetometer_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case magnetometer_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case magnetometer_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case magnetometer_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case magnetometer_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case magnetometer_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case magnetometer_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case magnetometer_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case magnetometer_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }
  
  magnetometer.setRange(magnetometer_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (magnetometer.getRange()) {
    case magnetometer_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case magnetometer_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case magnetometer_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case magnetometer_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  magnetometer.setIntThreshold(500);
  magnetometer.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!
 */
}

void loop() {
  magnetometer.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("\nX:  "); Serial.print(magnetometer.x); 
  Serial.print("  \tY:  "); Serial.print(magnetometer.y); 
  Serial.print("  \tZ:  "); Serial.println(magnetometer.z); 
  
  /* Or....get a new sensor event, normalized to uTesla */
  //sensors_event_t event; 
  //magnetometer.getEvent(&event);
  /* Display the results (magnetic field is measured in uTesla) */
  
  //Serial.print("\tX: "); Serial.print(event.magnetic.x);
  //Serial.print(" \tY: "); Serial.print(event.magnetic.y); 
  //Serial.print(" \tZ: "); Serial.print(event.magnetic.z); 
  //Serial.println(" uTesla ");

  delay(10000); 
  Serial.println();
}
