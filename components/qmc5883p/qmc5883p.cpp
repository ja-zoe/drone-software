#include "qmc5883p.h"
#include "Adafruit_QMC5883P.h"

extern "C" {
  #include "esp_err.h"
  #include "esp_log.h"
}

static const char* TAG = "MyModule";

esp_err_t QMC5883PDriver::init(qmc5883p_init_params_t init_params) {
  if(!dev_.begin()) {
    ESP_LOGE(TAG, "Failed to begin comms with QMC5883P. Initiation failed.");
    return ESP_FAIL;
  }
  dev_.setMode(init_params.mode);
  dev_.setODR(init_params.odr);
  dev_.setOSR(init_params.osr);
  dev_.setDSR(init_params.dsr);
  dev_.setRange(init_params.range);
  dev_.setSetResetMode(init_params.setResetMode);
  return ESP_OK;
};

esp_err_t QMC5883PDriver::read_raw(int16_t* x, int16_t* y, int16_t* z) {
  if(!dev_.isDataReady()) {
    ESP_LOGW(TAG, "Raw magnetic data not ready");
  };
  
  int16_t xRaw, yRaw, zRaw;
  if(!dev_.getRawMagnetic(&xRaw,&yRaw,&zRaw)) {
    ESP_LOGE(TAG, "Failed to read raw magnetic field");
    return ESP_FAIL;
  };

  *x = xRaw - offsetX;
  *y = yRaw - offsetY;
  *z = zRaw - offsetZ;
  return ESP_OK;
};

esp_err_t QMC5883PDriver::read_gauss(float* x, float* y, float* z) {
  if(!dev_.isDataReady()) {
    ESP_LOGW(TAG, "Gauss magnetic data not ready");
  };
  
  float xGauss, yGauss, zGauss;
  if(!dev_.getGaussField(&xGauss,&yGauss,&zGauss)) {
    ESP_LOGE(TAG, "Failed to read gauss magnetic field");
    return ESP_FAIL;
  };

  *x = xGauss - offsetGaussX;
  *y = yGauss - offsetGaussY;
  *z = zGauss - offsetGaussZ;
  return ESP_OK;
};

esp_err_t QMC5883PDriver::calibrate() {
    int32_t xSum = 0, ySum = 0, zSum = 0;

    for (uint16_t i = 0; i < 500; i++) {
        int16_t xr, yr, zr;
        if (read_raw(&xr, &yr, &zr) == ESP_OK) {
            xSum += xr;
            ySum += yr;
            zSum += zr;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    offsetX = xSum / 500;
    offsetY = ySum / 500;
    offsetZ = zSum / 500;

    qmc5883p_odr_t odr = dev_.getODR();
    float lsb_g = 0;

    switch(odr) {
      case QMC5883P_RANGE_30G:
        lsb_g = 1000;
        break ;
      case QMC5883P_RANGE_12G:
        lsb_g = 2500;
        break ;
      case QMC5883P_RANGE_8G:
        lsb_g = 3750;
        break ;
      case QMC5883P_RANGE_2G:
        lsb_g = 15000;
        break ;
    }

    offsetGaussX = offsetX / lsb_g;
    offsetGaussY = offsetY / lsb_g;
    offsetGaussZ = offsetZ / lsb_g;
    return ESP_OK;
}

