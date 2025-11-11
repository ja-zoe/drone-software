#include "qmc5883.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#define QMC5883_STATUS_DRDY (1 << 0)
#define QMC5883_STATUS_OVFLW (1 << 1)
#define QMC5883_MODE_CONTINUOUS 0b00

static const char *TAG = "QMC5883";

static const uint8_t QMC5883_ADDR = 0x2C;

static const uint8_t QMC5883_REG_CTRL_1 = 0x0A;
static const uint8_t QMC5883_REG_CTRL_2 = 0x0B;
static const uint8_t QMC5883_REG_STATUS = 0X09;
static const uint8_t QMC5883_REG_X_LSB = 0X01;

static float qmc5883_scale = 0;

static i2c_master_dev_handle_t i2c_dev;
static bool is_init = false;

static const i2c_device_config_t i2c_dev_cfg = {
  .dev_addr_length = 7,
  .device_address = QMC5883_ADDR,
  .scl_speed_hz = 400000,
  .scl_wait_us = 0,
  .disable_ack_check = false
};

static esp_err_t esp_ret;

// Initialize QMC5883
esp_err_t qmc5883_init(i2c_master_bus_handle_t bus_handle, qmc5883_cfg_t *qmc_cfg) {
    // Add QMC5883 to the main i2c bus
    esp_ret = i2c_master_bus_add_device(bus_handle, &i2c_dev_cfg, &i2c_dev);
    if(esp_ret != ESP_OK) return esp_ret;

    uint8_t write_data[2];

    // Configure QMC5883
    write_data[0] = QMC5883_REG_CTRL_1;
    write_data[1] = (qmc_cfg->osr2 << 6) | (qmc_cfg->osr1 << 4) | (qmc_cfg->odr << 2) | QMC5883_MODE_CONTINUOUS;
    esp_ret = i2c_master_transmit(i2c_dev, write_data, sizeof(write_data), -1);
    if(esp_ret != ESP_OK) return esp_ret;

    // Configure Gyroscope sensitivity
    write_data[0] = QMC5883_REG_CTRL_2;
    write_data[1] = (qmc_cfg->qmc_range << 2) | qmc_cfg->set_reset_mode;
    esp_ret = i2c_master_transmit(i2c_dev, write_data, sizeof(write_data), -1);
    if(esp_ret != ESP_OK) return esp_ret;

    // Set global qmc range reference for global use
    switch (qmc_cfg->qmc_range) {
      case 0: qmc5883_scale = 30.0f / 32768; break;
      case 1: qmc5883_scale = 12.0f / 32768; break;
      case 2: qmc5883_scale = 8.0f / 32768; break;
      case 3: qmc5883_scale = 2.0f / 32768; break;
      default: return ESP_ERR_INVALID_ARG;
    }

    is_init = true;
    return ESP_OK;
}

// Automatically find offset to zero out sensor
esp_err_t calibrate_mpu6050() {

}

// Read x,y,z magnetometer data (Gauss)
esp_err_t qmc5883_read_data(qmc5883_data_t *out_data) {
    if(!is_init) return ESP_ERR_INVALID_STATE;

    // Check if data is ready before reading data
    uint8_t status = 0;
    uint8_t retries = 0;
    do {
      esp_ret = qmc5883_status(&status);
      if (esp_ret != ESP_OK) return esp_ret;

      if (status & QMC5883_STATUS_DRDY) break;

      vTaskDelay(pdMS_TO_TICKS(5));
      retries++;
    } while (retries < 100);

    if (!(status & QMC5883_STATUS_DRDY)) {
      ESP_LOGE(TAG, "Timeout waiting for DRDY");
      return ESP_ERR_TIMEOUT;
    }

    // Read data once ready
    uint8_t raw[6];

    esp_ret = i2c_master_transmit_receive(
      i2c_dev, 
      &QMC5883_REG_X_LSB, 
      1, 
      raw, 
      6, 
      -1
    );
    if (esp_ret != ESP_OK) return esp_ret;

    out_data->x  = ((float)(int16_t)((raw[1] << 8) | raw[0])) * qmc5883_scale;
    out_data->y  = ((float)(int16_t)((raw[3] << 8) | raw[2])) * qmc5883_scale;
    out_data->z  = ((float)(int16_t)((raw[5] << 8) | raw[4])) * qmc5883_scale;

    return ESP_OK;
}

esp_err_t qmc5883_status(uint8_t *status) {  
    esp_ret = i2c_master_transmit_receive(
      i2c_dev,
      &QMC5883_REG_STATUS,
      1,
      status,
      1,
      -1
    );
    if (esp_ret != ESP_OK) return esp_ret;
  
    return ESP_OK;
}