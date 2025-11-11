#include "qmc5883.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

const int SDA_PIN = 5;
const int SCL_PIN = 6;

esp_err_t esp_ret;

void app_main() {
  i2c_master_bus_config_t i2c_config = {
    .i2c_port = -1, // Automatically select 1 of the 2 i2c peripherals in the esp32
    .sda_io_num = SDA_PIN,
    .scl_io_num = SCL_PIN,
    .clk_source = I2C_CLK_SRC_APB,
    .glitch_ignore_cnt = 7,
    .intr_priority = 0,
    .trans_queue_depth = 10,
    .enable_internal_pullup = 0,
    .allow_pd = 1
  }
  i2c_master_bus_handle_t bus_handle;

  uint8_t retries = 0;
  do {
    esp_ret = i2c_new_master_bus(&i2c_config, &bus_handle);
    if (esp_ret == ESP_OK) break;
    vTaskDelay(pdMS_TO_TICKS(5));
    retries++;
  } while (retries < 50);

  if (!(esp_ret == ESP_OK)) {
    ESP_LOGE(TAG, "Timeout waiting for new master bus creation");
    return;
  }  

  qmc5883_cfg_t qmc5883_cfg = {
    ODR_100HZ,
    OSR1_8,
    OSR2_4,
    SET_AND_RESET,
    QMC_12_GAUSS
  }

  esp_ret = qmc5883_init(bus_handle, &qmc5883_cfg)

}