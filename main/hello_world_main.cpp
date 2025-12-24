/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "Arduino.h"
#include "qmc5883p.h"
#include <Wire.h>
#include "motorControlTest.h"

extern "C" {
  #include <stdio.h>
  #include <inttypes.h>
  #include "sdkconfig.h"
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "esp_chip_info.h"
  #include "esp_flash.h"
  #include "esp_system.h"
  #include "esp_err.h"
}

extern "C" {
  void app_main(void);
}

esp_err_t esp_ret;

static const char* TAG = "Main";

extern "C" void app_main(void);

DShotRMT esc1, esc2, esc3, esc4;

void app_main() {
    // Install/init ESCs once
    esc1.install(GPIO_NUM_2, RMT_CHANNEL_0); esc1.init(); esc1.beep();

    // Create a task that handles throttle ramps
    xTaskCreatePinnedToCore([](void*){
        for (uint16_t i = 48; i < 2047; i += 20) {
            esc1.sendThrottle(i);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }, "motor_task", 8192, nullptr, 1, nullptr, 1);
}


