#include "mpu6050.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

const int SDA_PIN = 5;
const int SCL_PIN = 6;

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

  i2c_new_master_bus(&i2c_config, &bus_handle);


  i2c_master_bus_add_device()
}