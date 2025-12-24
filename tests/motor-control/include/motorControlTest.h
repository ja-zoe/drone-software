#pragma once
#include "DShotRMT.h"

extern "C" {
  #include "esp_err.h"
}

esp_err_t initialize_motor_test(
  gpio_num_t esc1_gpio, 
  gpio_num_t esc2_gpio, 
  gpio_num_t esc3_gpio, 
  gpio_num_t esc4_gpio,
  rmt_channel_t esc1_rmtChannel,
  rmt_channel_t esc2_rmtChannel,
  rmt_channel_t esc3_rmtChannel,
  rmt_channel_t esc4_rmtChannel
);