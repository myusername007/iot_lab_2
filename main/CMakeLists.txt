idf_component_register(
    SRCS
        "app_main.c"
        "wifi/wifi.c"
        "mqtt_handler/mqtt_handler.c"
        "button/button.c"
        "led/led.c"
    INCLUDE_DIRS
        "."
        "wifi"
        "mqtt_handler"
        "button"
        "led"
    REQUIRES
        nvs_flash esp_wifi mqtt esp_event esp_netif led_strip cjson
)
