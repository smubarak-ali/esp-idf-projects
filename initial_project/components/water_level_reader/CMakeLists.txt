idf_component_register(
        SRCS "src/water_level_reader.c"
        INCLUDE_DIRS "include"
        PRIV_REQUIRES esp_adc yf_s401_water_sensor
)
