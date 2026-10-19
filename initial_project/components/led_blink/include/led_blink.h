#pragma once

#include <stdbool.h>
#include "esp_err.h"

esp_err_t status_led_init(int gpio_pin);
esp_err_t status_led_set(bool on);
