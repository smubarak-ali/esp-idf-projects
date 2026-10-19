#include <stdio.h>
#include "esp_err.h"
#include "led_blink.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "water_level_reader.h"

void app_main(void)
{
    ESP_ERROR_CHECK(status_led_init(4));
    ESP_ERROR_CHECK(water_level_reader_init());
    ESP_ERROR_CHECK(water_level_reader_read());

    while (1)
    {
        // ESP_ERROR_CHECK(status_led_set(true));

        vTaskDelay(pdMS_TO_TICKS(2000));
        status_led_set(true);

        vTaskDelay(pdMS_TO_TICKS(2000));
        status_led_set(false);

        // ESP_ERROR_CHECK(status_led_set(false));
    }
}
