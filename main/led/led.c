#include "led.h"
#include "led_strip.h"
#include "esp_log.h"

#define TAG        "led"
#define LED_GPIO   GPIO_NUM_48
#define LED_COUNT  1

static led_strip_handle_t s_strip = NULL;

void led_init(void)
{
    led_strip_config_t cfg = {
        .strip_gpio_num = LED_GPIO,
        .max_leds       = LED_COUNT,
    };
    led_strip_rmt_config_t rmt = {
        .resolution_hz = 10 * 1000 * 1000,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&cfg, &rmt, &s_strip));
    led_set_rgb(0, 0, 0);
}

void led_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    if (!s_strip) return;
    led_strip_set_pixel(s_strip, 0, r, g, b);
    led_strip_refresh(s_strip);
    ESP_LOGI(TAG, "RGB(%u,%u,%u)", r, g, b);
}
