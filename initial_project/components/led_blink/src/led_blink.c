#include "led_blink.h"
#include <stdio.h>
#include "driver/gpio.h"

static int led_pin = -1;

esp_err_t status_led_init(int gpio_pin)
{
    if (!GPIO_IS_VALID_OUTPUT_GPIO(gpio_pin)) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = gpio_set_direction(
        (gpio_num_t)gpio_pin,
        GPIO_MODE_OUTPUT
    );

    if (err == ESP_OK) {
        led_pin = gpio_pin;
    }

    return err;
}

esp_err_t status_led_set(bool on)
{
    if (led_pin < 0) {
        return ESP_ERR_INVALID_STATE;
    }

    return gpio_set_level(
        (gpio_num_t)led_pin,
        on ? 1 : 0
    );
}
