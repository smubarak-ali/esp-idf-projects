idf_component_register(
        SRCS "src/temperature.c"

        INCLUDE_DIRS "include"

        PRIV_REQUIRES esp-idf-lib__dht esp_driver_gpio
)
