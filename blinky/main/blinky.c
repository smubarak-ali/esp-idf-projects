#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_PIN 27

static const gpio_num_t led_pin = GPIO_NUM_4;

void app_main(void)
{
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT); // Set the pin as output mode

    // Configure GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),   // Select GPIO 4
        .mode = GPIO_MODE_INPUT,                // Set as input
        .pull_up_en = GPIO_PULLUP_ENABLE,       // Enable internal pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
        .intr_type = GPIO_INTR_DISABLE          // Disable interrupts
    };
    gpio_config(&io_conf);

    while (1) {
        // Read GPIO level (current state)
        int level = gpio_get_level(BUTTON_PIN);
        if (level == 0) {
            printf("Button Pressed!\n");
            gpio_set_level(led_pin, 1);
        } else {
            printf("Button Released...\n");
            gpio_set_level(led_pin, 0);
        }
        vTaskDelay(50 / portTICK_PERIOD_MS); // Delay 50ms
    }

    // gpio_reset_pin(led_pin);                       // Reset the pin
    // gpio_set_direction(led_pin, GPIO_MODE_OUTPUT); // Set the pin as output mode
    //
    // while (1) {
    //     gpio_set_level(led_pin, 1);      // Set the pin to high level, turn on the LED
    //     printf("LED state: ON\n");       // Print the current state of the LED
    //     vTaskDelay(pdMS_TO_TICKS(1000)); // Task delay of 1000 milliseconds (1 second)
    //
    //     gpio_set_level(led_pin, 0);      // Set the pin to low level, turn off the LED
    //     printf("LED state: OFF\n");      // Print the current state of the LED
    //     vTaskDelay(pdMS_TO_TICKS(1000)); // Delay the task again for 1000 milliseconds (1 second)
    // }
}
