idf_component_register(
        SRCS "main.c"

        INCLUDE_DIRS "."

        PRIV_REQUIRES 
                led_blink 
                temperature 
                yf_s401_water_sensor
                water_level_reader 
                button_control_led 
                button_led_via_intr
)
