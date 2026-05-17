#include "button.h"
#include "mqtt_handler.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG         "button"
#define BUTTON_PIN  GPIO_NUM_0
#define DEBOUNCE_MS 200

static volatile uint32_t s_last_ms = 0;

static void IRAM_ATTR isr_handler(void *arg)
{
    uint32_t now = xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
    if ((now - s_last_ms) < DEBOUNCE_MS) return;
    s_last_ms = now;

    BaseType_t woken = pdFALSE;
    xTaskNotifyFromISR((TaskHandle_t)arg, 0, eNoAction, &woken);
    portYIELD_FROM_ISR(woken);
}

static void button_task(void *arg)
{
    gpio_config_t io = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .intr_type    = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&io);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, isr_handler, xTaskGetCurrentTaskHandle());

    ESP_LOGI(TAG, "Ready (GPIO %d)", BUTTON_PIN);

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ESP_LOGI(TAG, "Pressed — publishing");
        mqtt_publish_button_event();
    }
}

void button_init(void)
{
    xTaskCreate(button_task, "button_task", 4096, NULL, 10, NULL);
}
