/**
 * @file hm3301.h
 * @author your name (you@domain.com)
 * @briefDriver for Seeed PM2.5 Sensor(HM300)
 * Original driver in Seeed studio repository:
 * https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301
 * My repository will adapt the code for the ESP32 IDF framework
 * Using same MIT License as the original code
 */
#pragma once

#include "driver/i2c.h"

#include "i2cdev.h"

// HM330X error codes
typedef enum {
    NO_ERROR = 0,
    ERROR_PARAM = -1,
    ERROR_COMM = -2,
    ERROR_OTHERS = -128,
} HM330XErrorCode;

#define HM330X_I2C_ADDR  0x40

#define HM330X_SELECT_ADDR 0X88

#define HM330X_READ_ADDR   0x81

#define ACK_CHECK_EN 0x1                        /*! I2C master will check ack from slave */
#define ACK_CHECK_DIS 0x0                       /*! I2C master will not check ack from slave */

class HM330X {
  public:
    uint8_t buf[30];

    HM330X(uint8_t IIC_ADDR = HM330X_I2C_ADDR);

    HM330XErrorCode init(i2c_dev_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);

    HM330XErrorCode select_comm(i2c_dev_t *dev);

    HM330XErrorCode read_sensor_value(i2c_dev_t *dev, uint8_t* data); // uint32_t data_len

    // _sp Standard particles: 
    // Concentration based on CF=1 Standard Particulate (ug/m3)
    uint16_t getPM1_sp();
    uint16_t getPM2dot5_sp();
    uint16_t getPM10_sp();
    
    // _pol
    // Concentration based on the pollutants in the air (ug/m3)
    uint16_t getPM1_pol();
    uint16_t getPM2dot5_pol();
    uint16_t getPM10_pol();

    // Number of particles with the specified diameter or above in 0.1L of air
    uint16_t getP0dot3();
    uint16_t getP0dot5();
    uint16_t getP1();
    uint16_t getP2dot5();
    uint16_t getP5();
    uint16_t getP10();

  private:
    uint8_t _i2c_address = HM330X_I2C_ADDR;
    uint16_t decode_uint16(uint8_t* data, int i);
    // Last data result
    uint8_t* _dataresult;
};
