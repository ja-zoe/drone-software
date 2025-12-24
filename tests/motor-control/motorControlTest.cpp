#include "motorControlTest.h"
#include "DShotRMT.h"

extern "C" {
  #include "esp_err.h"
  #include "esp_log.h"
}

static const char* TAG = "Motor_Control_Test";

esp_err_t initialize_motor_test(
  gpio_num_t esc1_gpio, 
  gpio_num_t esc2_gpio, 
  gpio_num_t esc3_gpio, 
  gpio_num_t esc4_gpio,
  rmt_channel_t esc1_rmtChannel,
  rmt_channel_t esc2_rmtChannel,
  rmt_channel_t esc3_rmtChannel,
  rmt_channel_t esc4_rmtChannel
) {
  DShotRMT esc1;
  DShotRMT esc2;
  DShotRMT esc3;
  DShotRMT esc4;

  esp_err_t esp_ret;

  /*
    ---------- Install ESCs ----------
  */
  esp_ret = esc1.install(esc1_gpio, esc1_rmtChannel);
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to install ESC 1");
  };

  esp_ret = esc2.install(esc2_gpio, esc2_rmtChannel);
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to install ESC 2");
  };

  esp_ret = esc3.install(esc3_gpio, esc3_rmtChannel);
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to install ESC 3");
  };

  esp_ret = esc4.install(esc4_gpio, esc4_rmtChannel);
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to install ESC 4");
  };

  /*
    ---------- Initialize ESCs ----------
  */
  esp_ret = esc1.init();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to initialize ESC 1");
  };

  esp_ret = esc2.init();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to initialize ESC 2");
  };

  esp_ret = esc3.init();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to initialize ESC 3");
  };

  esp_ret = esc4.init();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to initialize ESC 4");
  };

  /*
    ---------- Beep ESCs ----------
  */
  esp_ret = esc1.beep();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to beep ESC 1");
  };

  esp_ret = esc2.beep();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to beep ESC 2");
  };

  esp_ret = esc3.beep();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to beep ESC 3");
  };

  esp_ret = esc4.beep();
  if (esp_ret != ESP_OK) {
    ESP_LOGE(TAG,"Failed to beep ESC 4");
  };

  /*
    ---------- Throttle ESCs ----------
  */
  ESP_LOGE(TAG,"Throttling ESCs");
  for (uint16_t i = 48; i < 2047; i += 20) {
    esc1.sendThrottle(i);
    vTaskDelay(pdMS_TO_TICKS(10));
  };

  return ESP_OK;
}