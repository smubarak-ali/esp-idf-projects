idf_component_register(
        SRCS "src/led_blink.c"

        INCLUDE_DIRS "include"

        PRIV_REQUIRES esp_driver_gpio
)
