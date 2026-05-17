#include "mqtt_handler.h"
#include "led.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

#define TAG          "mqtt"
#define TOPIC_BUTTON "/Nytka/button"
#define TOPIC_LED    "/Nytka/led"

static esp_mqtt_client_handle_t s_client = NULL;

static void handle_led(const char *data, int len)
{
    char *buf = strndup(data, len);
    cJSON *root = cJSON_Parse(buf);
    free(buf);
    if (!root) { ESP_LOGW(TAG, "Bad JSON"); return; }

    cJSON *r = cJSON_GetObjectItem(root, "r");
    cJSON *g = cJSON_GetObjectItem(root, "g");
    cJSON *b = cJSON_GetObjectItem(root, "b");

    if (cJSON_IsNumber(r) && cJSON_IsNumber(g) && cJSON_IsNumber(b)) {
        led_set_rgb(r->valueint, g->valueint, b->valueint);
    } else {
        ESP_LOGW(TAG, "Missing r/g/b");
    }
    cJSON_Delete(root);
}

static void mqtt_event_handler(void *arg, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t e = event_data;

    switch (event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Connected");
        esp_mqtt_client_subscribe(s_client, TOPIC_LED, 1);
        esp_mqtt_client_publish(s_client, TOPIC_BUTTON,
            "{\"event\":\"startup\",\"surname\":\"Nytka\"}", 0, 1, 0);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "TOPIC=%.*s DATA=%.*s",
                 e->topic_len, e->topic, e->data_len, e->data);
        if (strncmp(e->topic, TOPIC_LED, e->topic_len) == 0) {
            handle_led(e->data, e->data_len);
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "Disconnected");
        break;

    default:
        break;
    }
}

void mqtt_handler_init(void)
{
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI,
    };
    s_client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(s_client);
}

void mqtt_publish_button_event(void)
{
    if (!s_client) return;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "event", "button_press");
    cJSON_AddStringToObject(root, "surname", "Nytka");
    char *payload = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    esp_mqtt_client_publish(s_client, TOPIC_BUTTON, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Published: %s", payload);
    free(payload);
}
