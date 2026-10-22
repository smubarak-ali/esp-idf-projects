#include "stdio.h"
#include "temperature.h"
#include <dht.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_NUM GPIO_NUM_19

esp_err_t read_temperature(int delay_ms)
{
    float temperature, humidity;
    esp_err_t ret = dht_read_float_data(SENSOR_TYPE, GPIO_NUM, &humidity, &temperature);
    if (ret == ESP_OK)
        printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
    else
        printf("Could not read data from sensor\n");

    return ret;
}
