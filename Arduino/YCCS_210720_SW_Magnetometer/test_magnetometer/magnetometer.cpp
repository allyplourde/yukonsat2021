/*!
 * @file     Magnetometer.cpp
 *
 * @mainpage Adafruit magnetometer Breakout
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit magnetometer magnetometer breakout board
 * ----> https://www.adafruit.com/product/4479
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @author   Limor Fried (Adafruit Industries)
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "Arduino.h"
#include "magnetometer.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new magnetometer class
*/
/**************************************************************************/
Magnetometer::Magnetometer() {}


/*!
 *    @brief  Sets up the hardware and initializes hardware SPI
 *    @param  cs_pin The arduino pin # connected to chip select
 *    @param  theSPI The SPI object to be used for SPI connections.
 *    @return True if initialization was successful, otherwise false.
 */
boolean Magnetometer::init(uint8_t cs_pin, SPIClass *theSPI) {

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
bool Magnetometer::_init(void) {
  // Check connection
  Adafruit_BusIO_Register chip_id =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_WHO_AM_I, 1);

  // make sure we're talking to the right chip
  if (chip_id.read() != 0x3D) {
    // No magnetometer detected ... return false
    return false;
  }

  reset();

  // set high quality performance mode
  setPerformanceMode(magnetometer_ULTRAHIGHMODE);

  // 155Hz default rate
  setDataRate(magnetometer_DATARATE_155_HZ);

  // lowest range
  setRange(magnetometer_RANGE_4_GAUSS);

  setOperationMode(magnetometer_CONTINUOUSMODE);

  return true;
}

/**************************************************************************/
/*!

@brief  Performs a software reset
*/
/**************************************************************************/
void Magnetometer::reset(void) {
  Adafruit_BusIO_Register CTRL_REG2 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG2, 1);
  Adafruit_BusIO_RegisterBits resetbits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG2, 1, 2);
  resetbits.write(0x1);
  delay(10);
}

/**************************************************************************/
/*!
  @brief  Read the XYZ data from the magnetometer and store in the internal
  x, y and z (and x_g, y_g, z_g) member variables.
*/
/**************************************************************************/

void Magnetometer::read(void) {
  uint8_t buffer[6];

  Adafruit_BusIO_Register XYZDataReg = Adafruit_BusIO_Register(
      NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC, magnetometer_REG_OUT_X_L, 6);
  XYZDataReg.read(buffer, 6);
  x = buffer[0];
  x |= buffer[1] << 8;
  y = buffer[2];
  y |= buffer[3] << 8;
  z = buffer[4];
  z |= buffer[5] << 8;

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
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event, Adafruit Unified Sensor format
    @param  event Pointer to an Adafruit Unified sensor_event_t object that
   we'll fill in
    @returns True on successful read
*/
/**************************************************************************/
//bool Magnetometer::getEvent(sensors_event_t *event) {
  /* Clear the event */
  /*memset(event, 0, sizeof(sensors_event_t));

  event->version = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type = SENSOR_TYPE_MAGNETIC_FIELD;
  event->timestamp = millis();

  read();

  event->magnetic.x = x_gauss * 100; // microTesla per gauss
  event->magnetic.y = y_gauss * 100; // microTesla per gauss
  event->magnetic.z = z_gauss * 100; // microTesla per gauss

  return true;
}*/

/**************************************************************************/
/*!
    @brief  Gets the sensor_t device data, Adafruit Unified Sensor format
    @param  sensor Pointer to an Adafruit Unified sensor_t object that we'll
   fill in
*/
/**************************************************************************/
//void Magnetometer::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  //memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  /*strncpy(sensor->name, "magnetometer", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_MAGNETIC_FIELD;
  sensor->min_delay = 0;
  sensor->min_value = -1600;  // -16 gauss in uTesla
  sensor->max_value = 1600;   // +16 gauss in uTesla
  sensor->resolution = 0.015; // 100/6842 uTesla per LSB at +-4 gauss range
}*/

/**************************************************************************/
/*!
    @brief Set the performance mode, magnetometer_LOWPOWERMODE, magnetometer_MEDIUMMODE,
    magnetometer_HIGHMODE or magnetometer_ULTRAHIGHMODE
    @param mode Enumerated magnetometer_performancemode_t
*/
/**************************************************************************/
void Magnetometer::setPerformanceMode(magnetometer_performancemode_t mode) {
  // write xy
  Adafruit_BusIO_Register CTRL_REG1 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG1, 1);
  Adafruit_BusIO_RegisterBits performancemodebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG1, 2, 5);
  performancemodebits.write((uint8_t)mode);

  // write z
  Adafruit_BusIO_Register CTRL_REG4 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG4, 1);
  Adafruit_BusIO_RegisterBits performancemodezbits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG4, 2, 2);
  performancemodezbits.write((uint8_t)mode);
}

/**************************************************************************/
/*!
    @brief Get the performance mode
    @returns Enumerated magnetometer_performancemode_t, magnetometer_LOWPOWERMODE,
    magnetometer_MEDIUMMODE, magnetometer_HIGHMODE or magnetometer_ULTRAHIGHMODE
*/
/**************************************************************************/
magnetometer_performancemode_t Magnetometer::getPerformanceMode(void) {
  Adafruit_BusIO_Register CTRL_REG1 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG1, 1);
  Adafruit_BusIO_RegisterBits performancemodebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG1, 2, 5);
  return (magnetometer_performancemode_t)performancemodebits.read();
}

/**************************************************************************/
/*!
    @brief  Sets the data rate for the magnetometer (controls power consumption)
    from 0.625 Hz to 80Hz
    @param dataRate Enumerated magnetometer_dataRate_t
*/
/**************************************************************************/
void Magnetometer::setDataRate(magnetometer_dataRate_t dataRate) {
  if (dataRate == magnetometer_DATARATE_155_HZ) {
    // set OP to UHP
    setPerformanceMode(magnetometer_ULTRAHIGHMODE);
  }
  if (dataRate == magnetometer_DATARATE_300_HZ) {
    // set OP to HP
    setPerformanceMode(magnetometer_HIGHMODE);
  }
  if (dataRate == magnetometer_DATARATE_560_HZ) {
    // set OP to MP
    setPerformanceMode(magnetometer_MEDIUMMODE);
  }
  if (dataRate == magnetometer_DATARATE_1000_HZ) {
    // set OP to LP
    setPerformanceMode(magnetometer_LOWPOWERMODE);
  }
  delay(10);
  Adafruit_BusIO_Register CTRL_REG1 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG1, 1);
  Adafruit_BusIO_RegisterBits dataratebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG1, 4, 1); // includes FAST_ODR
  dataratebits.write((uint8_t)dataRate);
}

/**************************************************************************/
/*!
    @brief  Gets the data rate for the magnetometer (controls power consumption)
    @return Enumerated magnetometer_dataRate_t from 0.625 Hz to 80Hz
*/
/**************************************************************************/
magnetometer_dataRate_t Magnetometer::getDataRate(void) {
  Adafruit_BusIO_Register CTRL_REG1 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG1, 1);
  Adafruit_BusIO_RegisterBits dataratebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG1, 4, 1); // includes FAST_ODR
  return (magnetometer_dataRate_t)dataratebits.read();
}

/**************************************************************************/
/*!
    @brief Set the operation mode, magnetometer_CONTINUOUSMODE,
    magnetometer_SINGLEMODE or magnetometer_POWERDOWNMODE
    @param mode Enumerated magnetometer_operationmode_t
*/
/**************************************************************************/
void Magnetometer::setOperationMode(magnetometer_operationmode_t mode) {
  // write x and y
  Adafruit_BusIO_Register CTRL_REG3 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG3, 1);
  Adafruit_BusIO_RegisterBits opmodebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG3, 2, 0);
  opmodebits.write((uint8_t)mode);
}

/**************************************************************************/
/*!
    @brief Get the operation mode
    @returns Enumerated magnetometer_operationmode_t, magnetometer_CONTINUOUSMODE,
    magnetometer_SINGLEMODE or magnetometer_POWERDOWNMODE
*/
/**************************************************************************/
magnetometer_operationmode_t Magnetometer::getOperationMode(void) {
  Adafruit_BusIO_Register CTRL_REG3 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG3, 1);
  Adafruit_BusIO_RegisterBits opmodebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG3, 2, 0);
  return (magnetometer_operationmode_t)opmodebits.read();
}

/**************************************************************************/
/*!
    @brief Set the resolution range: +-4 gauss, 8 gauss, 12 gauss, or 16 gauss.
    @param range Enumerated magnetometer_range_t
*/
/**************************************************************************/
void Magnetometer::setRange(magnetometer_range_t range) {
  Adafruit_BusIO_Register CTRL_REG2 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG2, 1);
  Adafruit_BusIO_RegisterBits rangebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG2, 2, 5);
  rangebits.write((uint8_t)range);
}

/**************************************************************************/
/*!
    @brief Read the resolution range: +-4 gauss, 8 gauss, 12 gauss, or 16 gauss.
    @returns Enumerated magnetometer_range_t
*/
/**************************************************************************/
magnetometer_range_t Magnetometer::getRange(void) {
  Adafruit_BusIO_Register CTRL_REG2 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG2, 1);
  Adafruit_BusIO_RegisterBits rangebits =
      Adafruit_BusIO_RegisterBits(&CTRL_REG2, 2, 5);
  return (magnetometer_range_t)rangebits.read();
}

/**************************************************************************/
/*!
    @brief Set the interrupt threshold value
    @param value 16-bit unsigned raw value
*/
/**************************************************************************/
void Magnetometer::setIntThreshold(uint16_t value) {
  value &= 0x7FFF; // high bit must be 0!
  Adafruit_BusIO_Register INT_THS =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_INT_THS_L, 2);
  INT_THS.write(value);
}

/**************************************************************************/
/*!
    @brief Get the interrupt threshold value
    @returns 16-bit unsigned raw value
*/
/**************************************************************************/
uint16_t Magnetometer::getIntThreshold(void) {
  Adafruit_BusIO_Register INT_THS =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_INT_THS_L, 2);
  return INT_THS.read();
}

/**************************************************************************/
/*!
    @brief Configure INT_CFG
    @param enableX Enable interrupt generation on X-axis
    @param enableY Enable interrupt generation on Y-axis
    @param enableZ Enable interrupt generation on Z-axis
    @param polarity Sets the polarity of the INT output logic
    @param latch If true (latched) the INT pin remains in the same state
    until INT_SRC is read.
    @param enableInt Interrupt enable on INT pin
*/
/**************************************************************************/
void Magnetometer::configInterrupt(bool enableX, bool enableY, bool enableZ,
                                       bool polarity, bool latch,
                                       bool enableInt) {
  uint8_t value = 0x08; // set default bits, see table 36
  value |= enableX << 7;
  value |= enableY << 6;
  value |= enableZ << 5;
  value |= polarity << 2;
  value |= latch << 1;
  value |= enableInt;

  Adafruit_BusIO_Register INT_CFG = Adafruit_BusIO_Register(
      NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC, magnetometer_REG_INT_CFG, 1);
  INT_CFG.write(value);
}

/**************************************************************************/
/*!
    @brief Enable or disable self-test
    @param flag If true, enable self-test

*/
/**************************************************************************/
void Magnetometer::selfTest(bool flag) {
  Adafruit_BusIO_Register CTRL_REG1 =
      Adafruit_BusIO_Register(NULL, spi_dev, AD8_HIGH_TOREAD_AD7_HIGH_TOINC,
                              magnetometer_REG_CTRL_REG1, 1);

  Adafruit_BusIO_RegisterBits stbit =
      Adafruit_BusIO_RegisterBits(&CTRL_REG1, 1, 0);

  stbit.write(flag);
}
