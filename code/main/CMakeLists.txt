idf_component_register(SRCS "main.c"
                    REQUIRES sgtl5000 sa868 bt_hfp typedefs esp_driver_i2c esp_driver_i2s esp_driver_ledc esp_driver_gpio nvs_flash bt console
                    INCLUDE_DIRS ".")