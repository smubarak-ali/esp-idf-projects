idf_component_register(SRCS "blinky.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_driver_gpio
                    PRIV_REQUIRES freertos)
