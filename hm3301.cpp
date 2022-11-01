#include "hm3301.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

HM330X::HM330X(uint8_t IIC_ADDR) {
    _i2c_address = IIC_ADDR;
}

HM330XErrorCode HM330X::select_comm(i2c_dev_t *dev) {
    uint8_t outdata[1];
    outdata[0] = HM330X_SELECT_ADDR;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t ret1 = i2c_master_write_byte(cmd, (dev->addr << 1) | I2C_MASTER_WRITE, true);
    ESP_LOGI("HM330X", "i2c_master_write_byte send -> 0x%x: %s", HM330X_SELECT_ADDR, esp_err_to_name(ret1));
    esp_err_t ret2;

    for (int retryCount = 3; retryCount>0; retryCount--) {
       i2c_master_start(cmd);
       ret2 = i2c_master_write(cmd, (uint8_t*) outdata, 1, ACK_CHECK_EN);
       ESP_LOGI("HM330X", "Retry %d i2c_master_write: %s", retryCount, esp_err_to_name(ret2));
       
       i2c_master_stop(cmd);
       if (ret2 != 0) {
        ESP_LOGE("HM330X", "Error sending 0x88 select command");
        return ERROR_COMM;
       }
       vTaskDelay(150 / portTICK_PERIOD_MS);
    }
    
    return NO_ERROR;
}

HM330XErrorCode HM330X::init(i2c_dev_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio) {
    dev->port = port;
    dev->addr = _i2c_address;
    dev->sda_io_num = sda_gpio;
    dev->scl_io_num = scl_gpio;
    dev->clk_speed = 20000;
    esp_err_t i2c_init = i2c_master_init(port, sda_gpio, scl_gpio, dev->clk_speed);

    ESP_LOGI("HM330X", "I2C Addr:%x SDA:%d SCL:%d FREQ:%lu", _i2c_address, sda_gpio, scl_gpio,  dev->clk_speed);

    if (i2c_init != 0) {
        ESP_LOGE("HM330X", "i2c_master_init: %d desc: %s", i2c_init, esp_err_to_name(i2c_init));
        return ERROR_COMM;
    }
    return select_comm(dev);
}

HM330XErrorCode HM330X::read_sensor_value(i2c_dev_t *dev, uint8_t* data) {
    esp_err_t res = i2c_dev_read_reg(dev, HM330X_READ_ADDR, data, 29);
    _dataresult = data;
    if (res != ESP_OK) {
        ESP_LOGE("HM330X", "read_sensor_value: %x failed", HM330X_READ_ADDR);
        return ERROR_COMM;
    }
    return NO_ERROR;
}

uint16_t HM330X::decode_uint16(uint8_t* data, int i) {
    return data[i * 2] * 0x100 + data[i * 2 + 1];
}
    // Concentration based on CF=1 Standard Particulate (ug/m3)
uint16_t HM330X::getPM1_sp() {
    return decode_uint16(_dataresult, 2);
}
uint16_t HM330X::getPM2dot5_sp(){
    return decode_uint16(_dataresult, 3);
}
uint16_t HM330X::getPM10_sp(){
    return decode_uint16(_dataresult, 4);
}

// Concentration based on the pollutants in the air (ug/m3)
uint16_t HM330X::getPM1_pol() {
    return decode_uint16(_dataresult, 5);
}
uint16_t HM330X::getPM2dot5_pol(){
    return decode_uint16(_dataresult, 6);
}
uint16_t HM330X::getPM10_pol(){
    return decode_uint16(_dataresult, 7);
}

// Number of particles with the specified diameter or above in 0.1L of air
uint16_t HM330X::getP0dot3() {
    return decode_uint16(_dataresult, 8);
}
uint16_t HM330X::getP0dot5(){
    return decode_uint16(_dataresult, 9);
}
uint16_t HM330X::getP1(){
    return decode_uint16(_dataresult, 10);
}
uint16_t HM330X::getP2dot5() {
    return decode_uint16(_dataresult, 11);
}
uint16_t HM330X::getP5(){
    return decode_uint16(_dataresult, 12);
}
uint16_t HM330X::getP10(){
    return decode_uint16(_dataresult, 13);
}


  // The data sheet says "Standard particulate" and "Atmospheric environment", but what they mean?
  // Explained by https://github.com/tomoto
  // https://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301 says:
  //   The standard particulate matter mass concentration value refers to the mass concentration
  //   value obtained by density conversion of industrial metal particles as equivalent particles,
  //   and is suitable for use in industrial production workshops and the like.
  //   The concentration of particulate matter in the atmospheric environment is
  //   converted by the density of the main pollutants in the air as equivalent particles,
  //   and is suitable for ordinary indoor and outdoor atmospheric environments.