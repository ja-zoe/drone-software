idf_component_register(
    SRCS "qmc5883.c"
    INCLUDE_DIRS "include"
    REQUIRES driver freertos esp_log
)