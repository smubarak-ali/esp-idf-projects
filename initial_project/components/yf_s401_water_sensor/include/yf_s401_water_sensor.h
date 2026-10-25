#pragma once

#ifndef YF_S401_H
#define YF_S401_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define YF_S401_DEFAULT_K_FACTOR 98.0f

    typedef struct yf_s401_sensor *yf_s401_handle_t;

    typedef struct
    {
        int gpio_num;

        // Pulse frequency (Hz) per L/min.
        // Use 0 to select the default value of 98.
        float calibration_factor;

    } yf_s401_config_t;

    typedef struct
    {
        uint32_t interval_pulses;
        uint64_t total_pulses;

        float flow_lpm;
        double total_liters;

    } yf_s401_reading_t;

    /**
     * Initialize and start the sensor's PCNT peripheral.
     */
    esp_err_t yf_s401_create(
        const yf_s401_config_t *config,
        yf_s401_handle_t *out_sensor);

    /**
     * Sample the sensor.
     *
     * Calculates flow using pulses accumulated since
     * the previous sample and the actual elapsed time.
     *
     * Call from one task only, typically every second.
     */
    esp_err_t yf_s401_read(
        yf_s401_handle_t sensor,
        yf_s401_reading_t *reading);

    /**
     * Stop the sensor and release PCNT resources.
     */
    void yf_s401_delete(yf_s401_handle_t sensor);

#ifdef __cplusplus
}
#endif

#endif
