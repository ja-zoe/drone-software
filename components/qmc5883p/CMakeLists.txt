  idf_component_register(SRCS "qmc5883p.cpp"
                          INCLUDE_DIRS "include"
                          REQUIRES Adafruit_QMC5883P driver
                          )