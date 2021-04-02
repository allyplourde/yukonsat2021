// Basic demo for IMU readings


#include "imu.h"

IMU imu;
#define CLK 13
#define MISO 12
#define MOSI 11
#define IMU_CS 9


/* Specify sensor parameters (sample rate is twice the bandwidth)
 * choices are:
      AFS_2G, AFS_4G, AFS_8G, AFS_16G  
      GFS_15_125DPS, GFS_31_25DPS, GFS_62_5DPS, GFS_125DPS, GFS_250DPS, GFS_500DPS, GFS_1000DPS, GFS_2000DPS 
      AODR_1_5625Hz, AODR_3_125Hz, AODR_6_25Hz, AODR_12_5Hz, AODR_25Hz, AODR_50Hz, AODR_100Hz, AODR_200Hz, AODR_500Hz, AODR_1000Hz, AODR_2000Hz, AODR_4000Hz, AODR_8000Hz
      GODR_12_5Hz, GODR_25Hz, GODR_50Hz, GODR_100Hz, GODR_200Hz, GODR_500Hz, GODR_1000Hz, GODR_2000Hz, GODR_4000Hz, GODR_8000Hz
*/ 
uint8_t Ascale = AFS_2G, Gscale = GFS_250DPS, AODR = AODR_1000Hz, GODR = GODR_1000Hz;

float aRes, gRes;               // scale resolutions per LSB for the accel and gyro sensor2
float accelBias[3] = {0., 0., 0.}, gyroBias[3] = {0., 0., 0.}; // offset biases for the accel and gyro

void setup(void) {
  Serial.begin(115200);

   // get sensor resolutions, only need to do this once
   aRes = imu.getAres(Ascale);
   gRes = imu.getGres(Gscale);

  Serial.println("IMU test!");
  delay(100);
  if (! imu.init(IMU_CS)) {  // hardware SPI mode
  //if (! magnetometer.begin_SPI(magnetometer_CS, magnetometer_CLK, magnetometer_MISO, magnetometer_MOSI)) { // soft SPI
    Serial.println("Failed to find IMU chip");
    while (1) { delay(10); }
  }

  imu.reset(Ascale, Gscale, AODR, GODR);
  
  Serial.println("IMU Found!");
  Serial.println("IMU Status:");
  imu.status();

   imu.offsetBias(accelBias, gyroBias);
   Serial.println("accel biases (mg)"); Serial.println(1000.0f * accelBias[0]); Serial.println(1000.0f * accelBias[1]); Serial.println(1000.0f * accelBias[2]);
   Serial.println("gyro biases (dps)"); Serial.println(gyroBias[0]); Serial.println(gyroBias[1]); Serial.println(gyroBias[2]);
   delay(1000); 

}

void loop() {
  imu.readData();      // get X Y and Z data at once
  
  // Then print out the raw data
  Serial.print("AX:  "); Serial.print(imu.ax*aRes - accelBias[0]); 
  Serial.print("  \tAY:  "); Serial.print(imu.ay*aRes - accelBias[1]); 
  Serial.print("  \tAZ:  "); Serial.println(imu.az*aRes - accelBias[2]); 
  Serial.print("GX:  "); Serial.print(imu.gx*gRes - gyroBias[0]); 
  Serial.print("  \tGY:  "); Serial.print(imu.gy*gRes - gyroBias[1]); 
  Serial.print("  \tGZ:  "); Serial.println(imu.gz*gRes - gyroBias[2]); 
  
  /* Or....get a new sensor event, normalized to uTesla */
  //sensors_event_t event; 
  //magnetometer.getEvent(&event);
  /* Display the results (magnetic field is measured in uTesla) */
  
  //Serial.print("\tX: "); Serial.print(event.magnetic.x);
  //Serial.print(" \tY: "); Serial.print(event.magnetic.y); 
  //Serial.print(" \tZ: "); Serial.print(event.magnetic.z); 
  //Serial.println(" uTesla ");

  delay(5000); 
}
