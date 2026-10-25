idf_component_register(
    SRCS "src/yf_s401_water_sensor.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES
        esp_driver_pcnt
        esp_timer
)
