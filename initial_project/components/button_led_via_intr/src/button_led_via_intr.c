
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#define BUTTON_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_19

static const char *TAG = "BUTTON_IRQ";

// Handle of the FreeRTOS task that processes button events
static TaskHandle_t button_task_handle = NULL;

// Executed in interrupt context
static void button_isr_handler(void *arg)
{
    BaseType_t higher_priority_task_woken = pdFALSE;

    // Wake up the button task
    vTaskNotifyGiveFromISR(
        button_task_handle,
        &higher_priority_task_woken);

    if (higher_priority_task_woken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

// Executed in normal FreeRTOS task context
static void button_task(void *arg)
{
    bool led_on = false;

    while (1)
    {
        // Sleep until an interrupt notifies this task
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Wait for mechanical button bouncing to settle
        vTaskDelay(pdMS_TO_TICKS(30));

        if (gpio_get_level(BUTTON_PIN) == 0)
        {
            led_on = !led_on;

            gpio_set_level(LED_PIN, led_on);

            ESP_LOGI(TAG, "LED: %s",
                     led_on ? "ON" : "OFF");

            // Wait until the button is released
            while (gpio_get_level(BUTTON_PIN) == 0)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            // Discard extra interrupts from bouncing
            ulTaskNotifyTake(pdTRUE, 0);
        }
    }
}

void button_led_init(void)
{
    // 1. Configure LED output
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    ESP_ERROR_CHECK(gpio_config(&led_config));
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 0));

    // 2. Configure button input
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE};

    ESP_ERROR_CHECK(gpio_config(&button_config));

    // 3. Create the task BEFORE enabling its ISR
    BaseType_t created = xTaskCreate(
        button_task,
        "button_task",
        2048,
        NULL,
        10,
        &button_task_handle);
    configASSERT(created == pdPASS);

    // 4. Install shared GPIO ISR dispatcher
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // 5. Register our button handler
    ESP_ERROR_CHECK(gpio_isr_handler_add(
        BUTTON_PIN,
        button_isr_handler,
        NULL));

    ESP_LOGI(TAG, "Ready. Press the button!");
}
