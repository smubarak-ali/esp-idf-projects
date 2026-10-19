idf_component_register(
        SRCS "main.c"

        INCLUDE_DIRS "."

        PRIV_REQUIRES led_blink temperature water_level_reader
)
