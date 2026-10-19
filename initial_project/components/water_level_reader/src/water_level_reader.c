#include <stdio.h>
#include "stdbool.h"
#include "water_level_reader.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

#define SENSOR_ADC_CHANNEL ADC_CHANNEL_4 // GPIO32
#define READ_INTERVAL_MS 1000

adc_oneshot_unit_handle_t adc_handle; // Global handle for ADC unit

// Calibration handle (optional)
adc_cali_handle_t cali_handle = NULL;
bool do_calibration = false;

static const char *TAG = "ADC_SENSOR";

esp_err_t water_level_reader_init()
{
    // Step 1: Initialize ADC1
    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    ESP_ERROR_CHECK(
        adc_oneshot_new_unit(&unit_config, &adc_handle));

    // Step 2: Configure GPIO32 as ADC input
    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };

    ESP_ERROR_CHECK(
        adc_oneshot_config_channel(
            adc_handle,
            SENSOR_ADC_CHANNEL,
            &channel_config));

    // 3. Setup Calibration (Optional but highly recommended for millivolt conversion)
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    // Check if calibration scheme is supported/burned into eFuse
    esp_err_t cali_ret = adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle);
    if (cali_ret == ESP_OK)
    {
        do_calibration = true;
        ESP_LOGI(TAG, "Calibration initialized successfully.");
    }
    else
    {
        ESP_LOGW(TAG, "Calibration initialization failed. Proceeding without it.");
    }

    return ESP_OK;
}

esp_err_t water_level_reader_read()
{
    // Step 4: Continuously read analog values
    while (1)
    {
        int raw_value = 0;
        int voltage_mv = 0;

        esp_err_t result = adc_oneshot_read(
            adc_handle,
            SENSOR_ADC_CHANNEL,
            &raw_value);

        if (result == ESP_OK)
        {
            ESP_LOGI(TAG, "ADC Raw Value: %d", raw_value);

            // Convert raw to millivolts using the calibration handle
            if (do_calibration)
            {
                ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_value, &voltage_mv));
                ESP_LOGI(TAG, "Calibrated Voltage: %d mV", voltage_mv);
            }
        }
        else
        {
            ESP_LOGE(TAG, "ADC read failed: %s",
                     esp_err_to_name(result));
        }

        // Delay without busy-waiting
        vTaskDelay(pdMS_TO_TICKS(READ_INTERVAL_MS));

        // 5. Clean up (Tear down handles if you ever exit the loop)
        // ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
        // if (do_calibration) {
        //     ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(cali_handle));
        // }
    }
}