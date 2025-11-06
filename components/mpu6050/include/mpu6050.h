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
} mpu6050_data_t;

esp_err_t mpu6050_init(i2c_master_bus_handle_t bus_handle);
esp_err_t mpu6050_read_data(i2c_master_bus_handle_t bus_handle, mpu6050_data_t *data);
