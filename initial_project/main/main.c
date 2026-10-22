#include <stdio.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "esp_err.h"
#include "led_blink.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "water_level_reader.h"
#include "temperature.h"
#include "driver/gpio.h"
#include "relay.h"
#include "esp_log.h"

static const char *TAG = "RelayExample";

// #define LOW_TANK_PIN 4
// #define HIGH_TANK_PIN 19
#define RELAY_PIN 19

void app_main(void)
{
    gpio_config_t io_conf_low = {
        .pin_bit_mask = (1ULL << RELAY_PIN),   // Select GPIO 4
        .mode = GPIO_MODE_OUTPUT,              // Set as output
        .pull_up_en = GPIO_PULLUP_DISABLE,     // Enable internal pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
        .intr_type = GPIO_INTR_DISABLE         // Disable interrupts
    };
    gpio_config(&io_conf_low);

    gpio_set_level(RELAY_PIN, 0);

    while (1)
    {
        printf("Turn Relay ON\n");
        gpio_set_level(RELAY_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait 2 seconds

        printf("Turn Relay OFF\n");
        gpio_set_level(RELAY_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait 2 seconds
    }
}

// // OK code for float sensor
// void app_main(void)
// {
//     gpio_config_t io_conf_low = {
//         .pin_bit_mask = (1ULL << LOW_TANK_PIN), // Select GPIO 4
//         .mode = GPIO_MODE_INPUT,                // Set as input
//         .pull_up_en = GPIO_PULLUP_ENABLE,       // Enable internal pull-up
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
//         .intr_type = GPIO_INTR_DISABLE          // Disable interrupts
//     };
//     gpio_config(&io_conf_low);

//     gpio_config_t io_conf_high = {
//         .pin_bit_mask = (1ULL << HIGH_TANK_PIN), // Select GPIO 19
//         .mode = GPIO_MODE_INPUT,                 // Set as input
//         .pull_up_en = GPIO_PULLUP_ENABLE,        // Enable internal pull-up
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,   // Disable pull-down
//         .intr_type = GPIO_INTR_DISABLE           // Disable interrupts
//     };
//     gpio_config(&io_conf_high);

//     while (1)
//     {
//         // Read GPIO level (current state)
//         int level = gpio_get_level(LOW_TANK_PIN);
//         if (level == 0)
//         {
//             printf("Low Tank Detected!\n");
//         }
//         else
//         {
//             printf("Low Tank Not Detected...\n");
//         }

//         level = gpio_get_level(HIGH_TANK_PIN);
//         if (level == 0)
//         {
//             printf("High Tank Detected!\n");
//         }
//         else
//         {
//             printf("High Tank Not Detected...\n");
//         }

//         vTaskDelay(3000 / portTICK_PERIOD_MS); // Delay 3 seconds
//     }
// }
// // OK code for float sensor - end

// ESP_ERROR_CHECK(status_led_init(4));
// // ESP_ERROR_CHECK(water_level_reader_init());
// // ESP_ERROR_CHECK(water_level_reader_read());

// // #
// //  1. Disable Wi-Fi and Bluetooth if active
// //  esp_wifi_stop();

// // 2. Configure a timer wakeup for 5 seconds (5000000 microseconds)
// esp_sleep_enable_timer_wakeup(20000000);

// // 3. Optional: Check wakeup cause after waking up
// esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
// printf("Wakeup cause: %d\n", wakeup_cause);

// //// Spin up the asynchronous task
// // xTaskCreate(async_led, "async_led", 4096, NULL, 5, NULL);
// ESP_ERROR_CHECK(status_led_set(true));
// vTaskDelay(pdMS_TO_TICKS(5000));
// ESP_ERROR_CHECK(status_led_set(false));

// ESP_ERROR_CHECK(read_temperature(3000));

// esp_deep_sleep_start();
// #

// while (1)
// {
// ESP_ERROR_CHECK(status_led_set(true));

// vTaskDelay(pdMS_TO_TICKS(5000));
// status_led_set(true);

//     vTaskDelay(pdMS_TO_TICKS(2000));
//     status_led_set(false);

//     ESP_ERROR_CHECK(read_temperature(3000));

// ESP_ERROR_CHECK(status_led_set(false));
// }
// }
