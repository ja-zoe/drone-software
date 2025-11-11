#pragma once
#include "esp_err.h"
#include "driver/i2c_master.h"

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float temp; // degrees celsius
} mpu6050_data_t;

/**
 * Configuration options for accelerometer and gyroscope sensitivities(precision)
 *
 * Accelerometer
 * 0: ± 2g
 * 1: ± 4g
 * 2: ± 8g
 * 3: ± 16g
 *
 * Gyroscope
 * 0: ± 250 °/s
 * 1: ± 500 °/s
 * 2: ± 1000 °/s
 * 3: ± 2000 °/s
 */
enum lsb_sensitivity_cfg {
  0,
  1,
  2,
  3
}

enum dlpf_cfg {
  0,
  1,
  2,
  3,
  4,
  5,
  6
}

/**
 * @brief Initialize the MPU6050 sensor and configure its sensitivity and filters.
 *
 * Registers the MPU6050 on the provided I2C master bus, sets the accelerometer and
 * gyroscope full-scale sensitivity ranges, and configures the digital low-pass filter (DLPF).
 *
 * The accelerometer and gyroscope each have selectable full-scale ranges that determine
 * both measurement limits and resolution (LSB sensitivity). A smaller range yields higher
 * precision but saturates sooner at higher forces or angular rates.
 *
 * Accelerometer sensitivity options (AFS_SEL):
 *   0 → ±2g   (16,384 LSB/g)
 *   1 → ±4g   ( 8,192 LSB/g)
 *   2 → ±8g   ( 4,096 LSB/g)
 *   3 → ±16g  ( 2,048 LSB/g)
 *
 * Gyroscope sensitivity options (FS_SEL):
 *   0 → ±250  °/s  (131 LSB/°/s)
 *   1 → ±500  °/s  (65.5 LSB/°/s)
 *   2 → ±1000 °/s  (32.8 LSB/°/s)
 *   3 → ±2000 °/s  (16.4 LSB/°/s)
 *
 * Choosing higher sensitivity (lower full-scale range) provides finer resolution,
 * while lower sensitivity (higher range) allows measurement of larger motions without overflow.
 *
 * @param bus_handle             Handle to the I2C master bus.
 * @param accel_sensitivity_cfg  Accelerometer sensitivity configuration (AFS_SEL).
 * @param gyro_sensitivity_cfg   Gyroscope sensitivity configuration (FS_SEL).
 * @param dlpf_cfg               Digital low-pass filter configuration.
 *
 * @return ESP_OK on success, or an ESP_ERR_* code on failure.
 */
esp_err_t mpu6050_init(i2c_master_bus_handle_t bus_handle, lsb_sensitivity_cfg accel_sensitivity_cfg, lsb_sensitivity_cfg gyro_sensitivity_cfg, dlpf_cfg dlpf_cfg);

/**
 * @brief Calibrate the MPU6050 by estimating and storing sensor offsets.
 *
 * Samples accelerometer and gyroscope data while the device is stationary
 * to determine zero-level offsets. These offsets can be subtracted from
 * future readings to improve accuracy and reduce drift.
 *
 * @return ESP_OK on success, or an ESP_ERR_* code if calibration fails.
 */
esp_err_t calibrate_mpu6050();

/**
 * @brief Read raw sensor data from the MPU6050 and convert it to physical units.
 *
 * Retrieves 14 bytes starting at the ACCEL_XOUT_H register, containing the
 * accelerometer (X, Y, Z), temperature, and gyroscope (X, Y, Z) data. Each
 * raw 16-bit 2’s complement reading is scaled by the configured sensitivity
 * to yield acceleration in g, angular rate in °/s, and temperature in °C.
 *
 * @param bus_handle  Handle to the I2C master bus.
 * @param out_data    Pointer to a struct where the scaled sensor data will be stored.
 *
 * @return ESP_OK on success, or an ESP_ERR_* code if I2C communication fails.
 */
esp_err_t mpu6050_read_data(i2c_master_bus_handle_t bus_handle, mpu6050_data_t *data);
