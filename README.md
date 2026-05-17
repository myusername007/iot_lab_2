# IoT Lab 2 — MQTT (Variant 7, Nytka)

## Topics
| Topic | Direction | Payload |
|---|---|---|
| `/Nytka/button` | ESP32 → Broker | `{"event":"button_press","surname":"Nytka"}` |
| `/Nytka/led` | Broker → ESP32 | `{"r":255,"g":0,"b":0}` |

## Setup
1. Menuconfig → Example Configuration → set WiFi SSID/Password and Broker URI
2. Build → Flash → Monitor
