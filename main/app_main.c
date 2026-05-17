#include "nvs_flash.h"
#include "wifi.h"
#include "led.h"
#include "mqtt_handler.h"
#include "button.h"

void app_main(void)
{
    nvs_flash_init();
    led_init();
    wifi_init();
    mqtt_handler_init();
    button_init();
}
