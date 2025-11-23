/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "Arduino.h"
#include "qmc5883p.h"
#include <Wire.h>

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

void app_main(void)
{
    xTaskCreatePinnedToCore([](void*){
        initArduino();
        Wire.begin(GPIO_NUM_5, GPIO_NUM_6);
        delay(10);
        QMC5883PDriver qmc5883p;

        qmc5883p.init({
          .mode = QMC5883P_MODE_CONTINUOUS,
          .odr = QMC5883P_ODR_200HZ,
          .osr = QMC5883P_OSR_4,
          .dsr = QMC5883P_DSR_2,
          .range = QMC5883P_RANGE_8G,
          .setResetMode = QMC5883P_SETRESET_ON
        });
        printf("hi");

        for (;;) {
          int16_t xg = 0, yg = 0, zg = 0;
          
          for (int i = 0; i < 10; i++) {
            esp_ret = qmc5883p.read_raw(&xg,&yg,&zg);
            if(esp_ret != ESP_OK) {
              ESP_LOGI(TAG, "Failed to read gauss");
            };
            printf("---------------------------------------\n");
            printf("%d | %d | %d\n", xg, yg, zg);
            vTaskDelay(pdMS_TO_TICKS(500));
          }

          esp_ret = qmc5883p.calibrate();

          for (int i = 0; i < 10; i++) {
            esp_ret = qmc5883p.read_raw(&xg,&yg,&zg);
            if(esp_ret != ESP_OK) {
              ESP_LOGI(TAG, "Failed to read gauss");
            };
            printf("---------------------------------------\n");
            printf("%d | %d | %d\n", xg, yg, zg);
            vTaskDelay(pdMS_TO_TICKS(500));
          };
        }

    }, "arduino", 8192, nullptr, 1, nullptr, 1);
}


