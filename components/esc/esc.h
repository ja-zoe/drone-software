#pragma once
#include "DShotRMT.h"

extern "C" {
  #include "esp_err.h"
}



class ESC {
  public:
    esp_err_t init();
    esp_err_t sendThrottle();
  private:

};