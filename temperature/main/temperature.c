#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <dht.h>

#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_NUM GPIO_NUM_19

void app_main(void)
{
    float temperature, humidity;

    while (1)
    {
        if (dht_read_float_data(SENSOR_TYPE, GPIO_NUM, &humidity, &temperature) == ESP_OK)
            printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
        else
            printf("Could not read data from sensor\n");

        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
