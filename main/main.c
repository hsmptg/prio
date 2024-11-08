/*
 * Copyright (c) 2024 Helio Mendonca.
 * Use of this source code is governed by the GPL v3 license
 * that can be found in the LICENSE file.
 */

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "my_led.h"
#include "my_button.h"

#include "my_wifi.h"
#include "my_mqtt.h"

static const char *TAG = "myproj";

void app_main(void)
{
    ESP_LOGI(TAG, "myproj stated with SSL!!!");

    my_led_init();
    my_button_init();

    my_wifi_start();
    mqtt_app_start();

    test_leds();

    while (true)
    {
        printf("tick\n");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
