#include "mpu6050.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

static const char *TAG = "MPU6050";

static const uint8_t MPU6050_ADDR = 0x68;
static const uint8_t MPU6050_REG_PWR_MGMT_1 = 0x6B;
static const uint8_t MPU6050_REG_CONFIG = 0x1A;
static const uint8_t MPU6050_REG_CONFIG_GYRO = 0x1B;
static const uint8_t MPU6050_REG_CONFIG_ACCELL = 0x1C;
static const uint8_t MPU6050_REG_ACCEL_X_H = 0x3B; // The begining of the raw sensor data registers

i2c_master_dev_handle_t i2c_dev;
i2c_master_bus_handle_t master_bus_handle;

static const i2c_device_config_t i2c_dev_cfg = {
  .dev_addr_length = 7,
  .device_address = MPU6050_ADDR,
  .scl_speed_hz = 400000,
  .scl_wait_us = 0,
  .disable_ack_check = false
};

static float accel_sensitivity;
static float gyro_sensitivity;
static esp_err_t esp_ret;

// Initialize MPU6050
esp_err_t mpu6050_init(i2c_master_bus_handle_t bus_handle, lsb_sensitivity_cfg accel_sensitivity_cfg, lsb_sensitivity_cfg gyro_sensitivity_cfg, dlpf_cfg dlpf_cfg) {
    master_bus_handle = bus_handle;

    esp_ret = verify_master_bus();
    if(esp_ret != ESP_OK) return esp_ret;


    esp_ret = assign_sensitivities(accel_sensitivity_cfg, gyro_sensitivity_cfg);
    if(esp_ret != ESP_OK) return esp_ret;

    // Add MPU6050 to the main i2c bus
    esp_ret = i2c_master_bus_add_device(bus_handle, &i2c_dev_cfg, &i2c_dev);
    if(esp_ret != ESP_OK) return esp_ret;

    uint8_t write_data[2];

    // Configure MPU6050 -- DLPF enabled
    write_data[0] = MPU6050_REG_CONFIG;
    write_data[1] = (uint8_t) dlpf_cfg;
    esp_ret = i2c_master_transmit(i2c_dev, write_data, sizeof(write_data), -1);
    if(esp_ret != ESP_OK) return esp_ret;

    // Configure Accelerometer sensitivity
    write_data[0] = MPU6050_REG_CONFIG_ACCELL;
    write_data[1] = (uint8_t)(accel_sensitivity_cfg << 3); // Bit shift automatically promotes 
    esp_ret = i2c_master_transmit(i2c_dev, write_data, sizeof(write_data), -1);
    if(esp_ret != ESP_OK) return esp_ret;

    // Configure Gyroscope sensitivity
    write_data[0] = MPU6050_REG_CONFIG_GYRO;
    write_data[1] = (uint8_t)(gyro_sensitivity_cfg << 3);
    esp_ret = i2c_master_transmit(i2c_dev, write_data, sizeof(write_data), -1);
    if(esp_ret != ESP_OK) return esp_ret;
    
    return ESP_OK;
}

// Automatically find offset to zero out sensor
esp_err_t calibrate_mpu6050() {

}

// Read x,y,z accel/gyro data
esp_err_t mpu6050_read_data(i2c_master_bus_handle_t bus_handle, mpu6050_data_t *out_data) {
    esp_ret = verify_master_bus();
    if(esp_ret != ESP_OK) return esp_ret;
  
    uint8_t raw[14];

    esp_ret = i2c_master_transmit_receive(
      i2c_dev, 
      &MPU6050_REG_ACCEL_X_H, 
      sizeof(MPU6050_REG_ACCEL_X_H), 
      raw, 
      sizeof(raw), 
      -1
    );

    if (esp_ret != ESP_OK) return esp_ret;

    // Acceleration
    out_data->accel_x = ((float)(int16_t)((raw[0] << 8) | raw[1])) / accel_sensitivity;
    out_data->accel_y = ((float)(int16_t)((raw[2] << 8) | raw[3])) / accel_sensitivity;
    out_data->accel_z = ((float)(int16_t)((raw[4] << 8) | raw[5])) / accel_sensitivity;
    // Temperature
    out_data->temp = ((int16_t)((raw[6] << 8) | raw[7])) / 340.0f + 36.53f;
    // Gyroscope
    out_data->gyro_x  = ((float)(int16_t)((raw[8] << 8) | raw[9])) / gyro_sensitivity;
    out_data->gyro_y  = ((float)(int16_t)((raw[10] << 8) | raw[11])) / gyro_sensitivity;
    out_data->gyro_z  = ((float)(int16_t)((raw[12] << 8) | raw[13])) / gyro_sensitivity;

    return ESP_OK;
}

esp_err_t verify_master_bus() {
  if (!master_bus_handle) {
    ESP_LOGE(TAG, "Master bus handle is NULL");
    return ESP_ERR_INVALID_STATE;
  }
  return ESP_OK;
}

esp_err_t assign_sensitivities(lsb_sensitivity_cfg accel_sensitivity_cfg, lsb_sensitivity_cfg gyro_sensitivity_cfg) {
  switch (accel_sensitivity_cfg) {
    case 0: accel_sensitivity = 16384.0f; break;
    case 1: accel_sensitivity = 8192.0f; break;
    case 2: accel_sensitivity = 4096.0f; break;
    case 3: accel_sensitivity = 2048.0f; break;
    default: return ESP_ERR_INVALID_ARG;
  }

  switch (gyro_sensitivity_cfg) {
    case 0: gyro_sensitivity = 131.0f; break;
    case 1: gyro_sensitivity = 65.5f; break;
    case 2: gyro_sensitivity = 32.8f; break;
    case 3: gyro_sensitivity = 16.4f; break;
    default: return ESP_ERR_INVALID_ARG;
  }

  return ESP_OK;
}