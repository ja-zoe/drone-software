#ifndef QMC5883_H
#define QMC5883_H

// Define oversampling rate 1
typedef enum {
  OSR1_8 = 0b00,
  OSR1_4 = 0b01,
  OSR1_2 = 0b10,
  OSR1_1 = 0b11
} osr1_t;

// Define oversampling rate 2
typedef enum {
  OSR2_1 = 0b00,
  OSR2_2 = 0b01,
  OSR2_4 = 0b10,
  OSR2_8 = 0b11
} osr2_t;

// Define output data rate
typedef enum {
  ODR_10HZ  = 0b00,
  ODR_50HZ  = 0b01,
  ODR_100HZ = 0b10,
  ODR_200HZ = 0b11
} odr_t;

typedef enum {
  SET_AND_RESET = 0b00,
  SET_ONLY = 0b01,
  ALL_OFF = 0b10
} set_reset_mode_t;

typedef enum {
  QMC_30_GAUSS,
  QMC_12_GAUSS,
  QMC_8_GAUSS,
  QMC_2_GAUSS
} qmc5883_range_t;

// Configuration struct
typedef struct {
  odr_t odr;
  osr1_t osr1;
  osr2_t osr2;
  set_reset_mode_t set_reset_mode;
  qmc5883_range_t qmc_range;
} qmc5883_cfg_t;

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} qmc5883_data_t;

esp_err_t qmc5883_status(uint8_t *status);

esp_err_t qmc5883_init(i2c_master_bus_handle_t bus_handle, qmc5883_cfg_t *qmc_cfg)

esp_err_t qmc5883_read_data(qmc5883_data_t *out_data) 

#endif // QMC5883_H
