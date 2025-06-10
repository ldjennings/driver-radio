idf_component_register( SRCS "sgtl5000.c"
                        REQUIRES typedefs esp_driver_i2c esp_driver_i2s
                        INCLUDE_DIRS "include")