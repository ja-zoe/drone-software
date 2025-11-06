#include "bmp2080.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

static const char *TAG = "BMP2080";

#define BMP2080_ADDR ?? // To be decided
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B


const i2c_device_config_t i2c_dev_config = {
  .dev_addr_length = 7,
  .device_address = BMP2080_ADDR,
  .scl_speed_hz ,  // To be set
  .scl_wait_us = 0,
  .disable_ack_check = false
};

i2c_master_dev_handle_t i2c_dev;

// Initialize MPU6050
esp_err_t bmp2080_init(i2c_master_bus_handle_t bus_handle) {
    uint8_t data[2] = { MPU6050_REG_PWR_MGMT_1, 0x00 }; // To be set correctly
    i2c_master_bus_add_device(&bus_handle, &i2c_dev_config, &i2c_dev);
    i2c_master_transmit(i2c_master_dev_handle_t i2c_dev, const uint8_t *write_buffer, 8, 0);
    return i2c_master_transmit(i2c_dev, MPU6050_ADDR, data, sizeof(data), -1);
}

esp_err_t bmp2080_read_data(i2c_master_bus_handle_t bus_handle, mpu6050_data_t *data) {
    uint8_t reg = MPU6050_REG_ACCEL_XOUT_H;
    uint8_t raw[14];
    esp_err_t err = i2c_master_transmit_receive(bus_handle, MPU6050_ADDR, &reg, 1, raw, sizeof(raw), -1);
    if (err != ESP_OK) return err;

    data->accel_x = (int16_t)((raw[0] << 8) | raw[1]);
    data->accel_y = (int16_t)((raw[2] << 8) | raw[3]);
    data->accel_z = (int16_t)((raw[4] << 8) | raw[5]);
    data->gyro_x  = (int16_t)((raw[8] << 8) | raw[9]);
    data->gyro_y  = (int16_t)((raw[10] << 8) | raw[11]);
    data->gyro_z  = (int16_t)((raw[12] << 8) | raw[13]);
    return ESP_OK;
}

// Automatically find offset to zero out sensor
esp_err_t calibrate_bmp2080(i2c_master_bus_handle_t bus_handle) {

}