# Misc code

Scan WiFi APs:
```c
#include <string.h> // memset?

    int DEFAULT_SCAN_LIST_SIZE = 10;
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_scan_start(NULL, true);
    ESP_LOGI(TAG, "Max AP number ap_info can hold = %u", number);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, number);
    for (int i = 0; i < number; i++) {
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        ESP_LOGI(TAG, "Channel \t\t%d", ap_info[i].primary);
    }
```

Reason for STA disconnected:
```c
                wifi_event_sta_disconnected_t *xxx = (wifi_event_sta_disconnected_t *)event_data;
                ESP_LOGE(TAG, "Reason: %d", xxx->reason);
                if (xxx->reason == WIFI_REASON_NO_AP_FOUND) {
                    ESP_LOGI(TAG, "WIFI_REASON_NO_AP_FOUND");

                    wifi_config_t wifi_cfg_empty;
                    memset(&wifi_cfg_empty, 0, sizeof(wifi_config_t));
                    esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg_empty);

                    esp_restart();
                }
                
                esp_err_t err;
                err = wifi_prov_mgr_reset_provisioning();
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);                
```

Get WiFi config:
```c
    ESP_LOGI(TAG, "Provisioned: %s", provisioned ? "true" : "false");
    wifi_config_t wifi_cfg;
    esp_wifi_get_config(WIFI_IF_STA, &wifi_cfg);
    ESP_LOGI(TAG, "Provisioned SSID: %s", wifi_cfg.sta.ssid);
    ESP_LOGI(TAG, "bProv: %s", bProv ? "true" : "false");
```
