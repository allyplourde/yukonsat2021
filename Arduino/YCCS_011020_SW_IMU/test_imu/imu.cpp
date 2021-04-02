/*!
 * @file     IMU.cpp
 *
 * This is a library is adapted from the Adafruit magnetometer breakout board
 * ----> https://www.adafruit.com/product/4479
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @author   Allison Plourde
 *
 */

#include "Arduino.h"
#include "imu.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new IMU class
*/
/**************************************************************************/
IMU::IMU() {}


/*!
 *    @brief  Sets up the hardware and initializes hardware SPI
 *    @param  cs_pin The arduino pin # connected to chip select
 *    @param  theSPI The SPI object to be used for SPI connections.
 *    @return True if initialization was successful, otherwise false.
 */

boolean IMU::init(uint8_t cs_pin, SPIClass *theSPI) {

  if (!spi_dev) {
    spi_dev = new Adafruit_SPIDevice(cs_pin,
                                     1000000,               // frequency
                                     SPI_BITORDER_MSBFIRST, // bit order
                                     SPI_MODE0,             // data mode
                                     theSPI);
  }
  if (!spi_dev->begin()) {
    return false;
  }
  return _init();
}

/*!
 *    @brief  Common initialization code for I2C & SPI
 *    @return True if initialization was successful, otherwise false.
 */
bool IMU::_init(void) {
  // Check connection
  Adafruit_BusIO_Register chip_id =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_WHO_AM_I, 1);

  // make sure we're talking to the right chip
  if (chip_id.read() != 0x42) {
    // No IMU detected ... return false
    return false;
  }

  return true;
}

/**************************************************************************/
/*!

@brief  Performs a software reset
*/
/**************************************************************************/
void IMU::reset(uint8_t Ascale, uint8_t Gscale, uint8_t AODR, uint8_t GODR) {
  Adafruit_BusIO_Register CTRL_REG1 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_DEVICE_CONFIG, 1);
  Adafruit_BusIO_RegisterBits resetbits1 =
      Adafruit_BusIO_RegisterBits(&CTRL_REG1, 1, 0);
  resetbits1.write(0x01);
  delay(10);

  // enable gyro and accel in low noise mode
  Adafruit_BusIO_Register temp =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_PWR_MGMT0, 1 );
  Serial.println("ICM42605_PWR_MGMT0");
  Serial.println(temp.read());

  Adafruit_BusIO_RegisterBits resetbits2 =
      Adafruit_BusIO_RegisterBits(&temp,4,0);
  resetbits2.write(0x0F);
  delay(10);

  Serial.println("ICM42605_PWR_MGMT0");
  Serial.println(temp.read());

  Adafruit_BusIO_Register temp2 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_GYRO_CONFIG0, 1);

  Serial.println("ICM42605_GYRO_CONFIG0");
  Serial.println(temp2.read());
  
  Adafruit_BusIO_RegisterBits resetbits3 =
      Adafruit_BusIO_RegisterBits(&temp2,4,0);
  resetbits3.write(0x06);
  delay(10);

  Adafruit_BusIO_Register temp3 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_REG_BANK_SEL, 1);

  Serial.println("ICM42605_REG_BANK_SEL");
  Serial.println(temp3.read());
  
  Adafruit_BusIO_RegisterBits resetbits4 =
      Adafruit_BusIO_RegisterBits(&temp3,3,0);
  resetbits4.write(0x01);
  delay(10);

  /*
  Adafruit_BusIO_Register temp4 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_APEX_CONFIG5, 1);

  Serial.println("ICM42605_APEX_CONFIG5");
  Serial.println(temp4.read());
  
  Adafruit_BusIO_RegisterBits resetbits5 =
      Adafruit_BusIO_RegisterBits(&temp4,3,0);
  resetbits5.write(0x07);
  delay(10);

  Serial.println("ICM42605_APEX_CONFIG5");
  Serial.println(temp4.read());

  */
  
  Adafruit_BusIO_Register SENS_CFG =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_SENSOR_CONFIG0, 1);
  Serial.println("ICM42605_SENSOR_CONFIG0");
  Serial.println(SENS_CFG.read());
  Adafruit_BusIO_RegisterBits resetbits_SENS_CFG =
      Adafruit_BusIO_RegisterBits(&SENS_CFG,1,0);
  resetbits_SENS_CFG.write(0x00);
  delay(10);
  Serial.println("ICM42605_SENSOR_CONFIG0");
  Serial.println(SENS_CFG.read());

 Adafruit_BusIO_Register temp5 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_REG_BANK_SEL, 1);
  
  Adafruit_BusIO_RegisterBits resetbits6 =
      Adafruit_BusIO_RegisterBits(&temp5,3,0);
  resetbits6.write(0x00);
  delay(10);

  Serial.println("ICM42605_REG_BANK_SEL");
  Serial.println(temp5.read());

  



    // Select Bank 4
   //temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_REG_BANK_SEL);
  //_i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_REG_BANK_SEL, temp | 0x04 ); // select Bank 4
  
  
  /*
   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_ACCEL_CONFIG0);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_ACCEL_CONFIG0, temp | AODR | Ascale << 5); // set accel full scale and data rate

   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_GYRO_CONFIG1);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_GYRO_CONFIG1, temp | 0xD0); // set temperature sensor low pass filter to 5Hz, use first order gyro filter

   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_INT_CONFIG);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_INT_CONFIG, temp | 0x18 | 0x03 ); // set both interrupts active high, push-pull, pulsed

   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_INT_CONFIG1);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_INT_CONFIG1, temp & ~(0x10) ); // set bit 4 to zero for proper function of INT1 and INT2
 
   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_INT_SOURCE0);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_INT_SOURCE0, temp | 0x08 ); // route data ready interrupt to INT1
 
   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_INT_SOURCE3);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_INT_SOURCE3, temp | 0x01 ); // route AGC interrupt interrupt to INT2



   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_APEX_CONFIG5);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_APEX_CONFIG5, temp & ~(0x07) ); // select unitary mounting matrix

   temp = _i2c_bus->readByte(ICM42605_ADDRESS, ICM42605_REG_BANK_SEL);
  _i2c_bus->writeByte(ICM42605_ADDRESS, ICM42605_REG_BANK_SEL, temp & ~(0x07) ); // select Bank 0
  */
  
}

void IMU::status()
{
  // reset device
  Adafruit_BusIO_Register temp =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              ICM42605_INT_STATUS, 1);
  Serial.println(temp.read());
}


float IMU::getAres(uint8_t Ascale) {
  switch (Ascale)
  {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
    case AFS_2G:
      _aRes = 2.0f / 32768.0f;
      return _aRes;
      break;
    case AFS_4G:
      _aRes = 4.0f / 32768.0f;
      return _aRes;
      break;
    case AFS_8G:
      _aRes = 8.0f / 32768.0f;
      return _aRes;
      break;
    case AFS_16G:
      _aRes = 16.0f / 32768.0f;
      return _aRes;
      break;
  }
}

float IMU::getGres(uint8_t Gscale) {
  switch (Gscale)
  {
    case GFS_15_125DPS:
      _gRes = 15.125f / 32768.0f;
      return _gRes;
      break;
    case GFS_31_25DPS:
      _gRes = 31.25f / 32768.0f;
      return _gRes;
      break;
    case GFS_62_5DPS:
      _gRes = 62.5f / 32768.0f;
      return _gRes;
      break;
    case GFS_125DPS:
      _gRes = 125.0f / 32768.0f;
      return _gRes;
      break;
    case GFS_250DPS:
      _gRes = 250.0f / 32768.0f;
      return _gRes;
      break;
    case GFS_500DPS:
      _gRes = 500.0f / 32768.0f;
      return _gRes;
      break;
    case GFS_1000DPS:
      _gRes = 1000.0f / 32768.0f;
      return _gRes;
      break;
    case GFS_2000DPS:
      _gRes = 2000.0f / 32768.0f;
      return _gRes;
      break;
  }
}


void IMU::offsetBias(float * dest1, float * dest2)
{
  int16_t temp[7] = {0, 0, 0, 0, 0, 0, 0};
  int32_t sum[7] = {0, 0, 0, 0, 0, 0, 0};

  Serial.println("Calculate accel and gyro offset biases: keep sensor flat and motionless!");
  delay(4000);

  for (int ii = 0; ii < 128; ii++)
  {
    readData();
    sum[1] += ax;
    sum[2] += ay;
    sum[3] += az;
    sum[4] += gx;
    sum[5] += gy;
    sum[6] += gz;
    delay(50);
  }

  dest1[0] = sum[1] * _aRes / 128.0f;
  dest1[1] = sum[2] * _aRes / 128.0f;
  dest1[2] = sum[3] * _aRes / 128.0f;
  dest2[0] = sum[4] * _gRes / 128.0f;
  dest2[1] = sum[5] * _gRes / 128.0f;
  dest2[2] = sum[6] * _gRes / 128.0f;

  if (dest1[0] > 0.8f)  {
    dest1[0] -= 1.0f; // Remove gravity from the x-axis accelerometer bias calculation
  }
  if (dest1[0] < -0.8f) {
    dest1[0] += 1.0f; // Remove gravity from the x-axis accelerometer bias calculation
  }
  if (dest1[1] > 0.8f)  {
    dest1[1] -= 1.0f; // Remove gravity from the y-axis accelerometer bias calculation
  }
  if (dest1[1] < -0.8f) {
    dest1[1] += 1.0f; // Remove gravity from the y-axis accelerometer bias calculation
  }
  if (dest1[2] > 0.8f)  {
    dest1[2] -= 1.0f; // Remove gravity from the z-axis accelerometer bias calculation
  }
  if (dest1[2] < -0.8f) {
    dest1[2] += 1.0f; // Remove gravity from the z-axis accelerometer bias calculation
  }

}

/**************************************************************************/
/*!
  @brief  Read the XYZ data from the IMU and store in the internal
  x, y and z (and x_g, y_g, z_g) member variables.
*/
/**************************************************************************/

void IMU::readData() {

  //uint8_t buffer[14];  // x/y/z accel register data stored here

  Adafruit_BusIO_Register XYZDataReg = Adafruit_BusIO_Register(
      NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC, ICM42605_GYRO_DATA_X0, 1);
  //XYZDataReg.read(buffer, 14);
  //Serial.println(XYZDataReg.read());
 
  //t0 = ((int16_t)buffer[0] << 8) | buffer[1] ;  // Turn the MSB and LSB into a signed 16-bit value
  //ax = ((int16_t)buffer[2] << 8) | buffer[3] ;
  //ay = ((int16_t)buffer[4] << 8) | buffer[5] ;
  //gz = ((int16_t)buffer[6] << 8) | buffer[7] ;
  //gx = ((int16_t)buffer[8] << 8) | buffer[9] ;
  //gy = ((int16_t)buffer[10] << 8) | buffer[11] ;
  //gz = ((int16_t)buffer[12] << 8) | buffer[13] ;

  ax = 0;
  ay = 0;
  az = 0;
  gx = XYZDataReg.read();
  gy = 0;
  gz = 0;

  /*
  magnetometer_range_t range = getRange();
  float scale = 1; // LSB per gauss
  if (range == magnetometer_RANGE_16_GAUSS)
    scale = 1711;
  if (range == magnetometer_RANGE_12_GAUSS)
    scale = 2281;
  if (range == magnetometer_RANGE_8_GAUSS)
    scale = 3421;
  if (range == magnetometer_RANGE_4_GAUSS)
    scale = 6842;

  x_gauss = (float)x / scale;
  y_gauss = (float)y / scale;
  z_gauss = (float)z / scale;
  */
}





 
