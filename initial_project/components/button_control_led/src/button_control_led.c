#include <stdio.h>
#include "driver/gpio.h"
#include "button_control_led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_PIN 4
#define LED_PIN 19

void button_control_led_init(void)
{
    // Configure button pin as input
    gpio_config_t io_conf_button = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    ESP_ERROR_CHECK(gpio_config(&io_conf_button));

    // Configure LED pin as output
    gpio_config_t io_conf_led = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    ESP_ERROR_CHECK(gpio_config(&io_conf_led));

    while (1)
    {
        // Read button state
        int button_state = gpio_get_level(BUTTON_PIN);
        if (button_state == 0) // Button pressed
        {
            printf("Button Pressed! Turning LED OFF\n");
            gpio_set_level(LED_PIN, 0); // Turn LED ON
        }
        else // Button not pressed
        {
            printf("Button Not Pressed. Turning LED ON\n");
            gpio_set_level(LED_PIN, 1); // Turn LED OFF
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for debouncing
    }
}