/*!
 * @file     Magnetometer.h
 *
 */

#ifndef Magnetometer_H
#define Magnetometer_H

#include <Adafruit_BusIO_Register.h>
//#include <Adafruit_SPIDevice.h>
//#include <Adafruit_Sensor.h>


#define magnetometer_REG_WHO_AM_I 0x0F  ///< Register that contains the part ID
#define magnetometer_REG_CTRL_REG1 0x20 ///< Register address for control 1
#define magnetometer_REG_CTRL_REG2 0x21 ///< Register address for control 2
#define magnetometer_REG_CTRL_REG3 0x22 ///< Register address for control 3
#define magnetometer_REG_CTRL_REG4 0x23 ///< Register address for control 3
#define magnetometer_REG_OUT_X_L 0x28   ///< Register address for X axis lower byte
#define magnetometer_REG_INT_CFG 0x30   ///< Interrupt configuration register
#define magnetometer_REG_INT_THS_L 0x32 ///< Low byte of the irq threshold

/** The magnetometer ranges */
typedef enum {
  magnetometer_RANGE_4_GAUSS = 0b00,  ///< +/- 4g (default value)
  magnetometer_RANGE_8_GAUSS = 0b01,  ///< +/- 8g
  magnetometer_RANGE_12_GAUSS = 0b10, ///< +/- 12g
  magnetometer_RANGE_16_GAUSS = 0b11, ///< +/- 16g
} magnetometer_range_t;

/** The magnetometer data rate, includes FAST_ODR bit */
typedef enum {
  magnetometer_DATARATE_0_625_HZ = 0b0000, ///<  0.625 Hz
  magnetometer_DATARATE_1_25_HZ = 0b0010,  ///<  1.25 Hz
  magnetometer_DATARATE_2_5_HZ = 0b0100,   ///<  2.5 Hz
  magnetometer_DATARATE_5_HZ = 0b0110,     ///<  5 Hz
  magnetometer_DATARATE_10_HZ = 0b1000,    ///<  10 Hz
  magnetometer_DATARATE_20_HZ = 0b1010,    ///<  20 Hz
  magnetometer_DATARATE_40_HZ = 0b1100,    ///<  40 Hz
  magnetometer_DATARATE_80_HZ = 0b1110,    ///<  80 Hz
  magnetometer_DATARATE_155_HZ = 0b0001,   ///<  155 Hz (FAST_ODR + UHP)
  magnetometer_DATARATE_300_HZ = 0b0011,   ///<  300 Hz (FAST_ODR + HP)
  magnetometer_DATARATE_560_HZ = 0b0101,   ///<  560 Hz (FAST_ODR + MP)
  magnetometer_DATARATE_1000_HZ = 0b0111,  ///<  1000 Hz (FAST_ODR + LP)
} magnetometer_dataRate_t;

/** The magnetometer performance mode */
typedef enum {
  magnetometer_LOWPOWERMODE = 0b00,  ///< Low power mode
  magnetometer_MEDIUMMODE = 0b01,    ///< Medium performance mode
  magnetometer_HIGHMODE = 0b10,      ///< High performance mode
  magnetometer_ULTRAHIGHMODE = 0b11, ///< Ultra-high performance mode
} magnetometer_performancemode_t;

/** The magnetometer operation mode */
typedef enum {
  magnetometer_CONTINUOUSMODE = 0b00, ///< Continuous conversion
  magnetometer_SINGLEMODE = 0b01,     ///< Single-shot conversion
  magnetometer_POWERDOWNMODE = 0b11,  ///< Powered-down mode
} magnetometer_operationmode_t;

/** Class for hardware interfacing with an magnetometer*/
class Magnetometer{
public:
  Magnetometer(void);
  bool init(uint8_t cs_pin, SPIClass *theSPI = &SPI);

  void reset(void);

  void setPerformanceMode(magnetometer_performancemode_t mode);
  magnetometer_performancemode_t getPerformanceMode(void);
  void setOperationMode(magnetometer_operationmode_t mode);
  magnetometer_operationmode_t getOperationMode(void);
  void setDataRate(magnetometer_dataRate_t dataRate);
  magnetometer_dataRate_t getDataRate(void);
  void setRange(magnetometer_range_t range);
  magnetometer_range_t getRange(void);
  void setIntThreshold(uint16_t value);
  uint16_t getIntThreshold(void);
  void configInterrupt(bool enableX, bool enableY, bool enableZ, bool polarity,
                       bool latch, bool enableInt);
  void selfTest(bool flag);

  void read();
  //bool getEvent(sensors_event_t *event);
  //void getSensor(sensor_t *sensor);

  int16_t x,     ///< The last read X mag in raw units
      y,         ///< The last read Y mag in raw units
      z;         ///< The last read Z mag in raw units
  float x_gauss, ///< The last read X mag in 'gauss'
      y_gauss,   ///< The last read Y mag in 'gauss'
      z_gauss;   ///< The last read Z mag in 'gauss'
      

private:
  bool _init(void);

  Adafruit_SPIDevice *spi_dev = NULL;

  int32_t _sensorID;
};

#endif
