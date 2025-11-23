#pragma once

#include "Adafruit_QMC5883P.h"

typedef struct {
  qmc5883p_mode_t mode; 
  qmc5883p_odr_t odr; 
  qmc5883p_osr_t osr; 
  qmc5883p_dsr_t dsr; 
  qmc5883p_range_t range; 
  qmc5883p_setreset_t setResetMode;
} qmc5883p_init_params_t;

class QMC5883PDriver {
public:
    QMC5883PDriver() = default;

    esp_err_t init(qmc5883p_init_params_t init_params);
    esp_err_t read_raw(int16_t* x, int16_t* y, int16_t* z);
    esp_err_t read_gauss(float* x, float* y, float* z);
    esp_err_t calibrate();

private:
    Adafruit_QMC5883P dev_;
    int16_t offsetX;
    int16_t offsetY;
    int16_t offsetZ;
    float offsetGaussX;
    float offsetGaussY;
    float offsetGaussZ;
};
