idf_component_register(
    SRCS "app_main.c"
         "wifi/wifi.c"
         "mqtt_handler/mqtt_handler.c"
         "button/button.c"
         "led/led.c"
    REQUIRES esp_wifi esp_event nvs_flash driver esp_driver_gpio
    INCLUDE_DIRS "."
                 "wifi"
                 "mqtt_handler"
                 "button"
                 "led"
)