
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_timer.h"

#include "yf_s401_water_sensor.h"
#include "water_level_reader.h"

#define FLOW_GPIO 27
#define SAMPLE_INTERVAL_MS 1000

static const char *TAG = "WATER_SYSTEM";

static int64_t get_time_ms(void)
{
    return esp_timer_get_time() / 1000;
}

void app_main(void)
{
    // Initialize the PCNT-based sensor.
    yf_s401_handle_t sensor = NULL;

    yf_s401_config_t sensor_cfg = {
        .gpio_num = FLOW_GPIO,
        .calibration_factor = 98.0f};

    ESP_ERROR_CHECK(yf_s401_create(
        &sensor_cfg, &sensor));

    // Initialize the flow monitor.
    water_flow_monitor_t monitor;

    water_flow_monitor_config_t monitor_cfg = {
        .flow_on_lpm = 1.2f,
        .flow_off_lpm = 0.8f,

        .startup_grace_ms = 4000,
        .flow_confirm_ms = 1000,
        .no_flow_confirm_ms = 2000,
        .max_sample_gap_ms = 2000};

    ESP_ERROR_CHECK(water_flow_monitor_init(
        &monitor, &monitor_cfg));

    // Demonstration: flow is expected from now.
    water_flow_monitor_start(
        &monitor, get_time_ms());

    while (1)
    {

        vTaskDelay(pdMS_TO_TICKS(
            SAMPLE_INTERVAL_MS));

        yf_s401_reading_t reading = {0};

        esp_err_t err = yf_s401_read(
            sensor, &reading);

        if (err != ESP_OK)
        {
            water_flow_monitor_sensor_error(
                &monitor);

            ESP_LOGE(TAG, "Sensor read failed");
            break;
        }

        water_flow_state_t previous =
            monitor.state;

        err = water_flow_monitor_update(
            &monitor,
            &reading,
            get_time_ms());

        if (err != ESP_OK)
        {
            ESP_LOGE(TAG,
                     "Flow monitor error: %s",
                     esp_err_to_name(err));
            break;
        }

        // Always display the measured flow.
        ESP_LOGI(TAG,
                 "Flow: %.2f L/min | State: %s",
                 reading.flow_lpm,
                 water_flow_monitor_state_name(
                     monitor.state));

        // React only when the state changes.
        if (previous != monitor.state)
        {

            switch (monitor.state)
            {

            case WATER_FLOW_FLOWING:
                ESP_LOGI(TAG,
                         "Water flow confirmed");
                break;

            case WATER_FLOW_NO_FLOW:
                ESP_LOGW(TAG,
                         "Insufficient water flow!");

                // Notify application logic.
                // No automatic restart.
                break;

            default:
                break;
            }
        }
    }

    yf_s401_delete(sensor);
}

//// DONT DELETE...
// #include <stdio.h>
// #include "esp_sleep.h"
// #include "driver/rtc_io.h"
// #include "esp_err.h"
// #include "led_blink.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "temperature.h"
// #include "driver/gpio.h"
// #include "relay.h"
// #include "esp_log.h"
// #include "button_led_via_intr.h"

// static const char *TAG = "RelayExample";

// // #define LOW_TANK_PIN 4
// // #define HIGH_TANK_PIN 19

// void app_main(void)
// {

//     button_led_init();
// }

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

///// DONT DELETE...