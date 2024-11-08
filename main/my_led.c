#include "my_led.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern bool bProv;

// // GREEN 5 - YELLOW 15 - RED 10
// #define LED 10

// static uint8_t s_led_state = 0;

static blink_step_t const *led_blink_lst[] = {
    [BLINK_HEART] = heart_blink,
    [BLINK_NUM] = NULL,
};

static led_indicator_handle_t led_handle_0 = NULL;

// void my_led_init(void)
// {
//     led_indicator_gpio_config_t led_indicator_gpio_config = {
//         .gpio_num = 5,
//         .is_active_level_high = 1,
//     };

//     led_indicator_config_t config = {
//         .led_indicator_gpio_config = &led_indicator_gpio_config,
//         .mode = LED_GPIO_MODE,
//         .blink_lists = led_blink_lst,
//         .blink_list_num = BLINK_NUM,
//     };

//     led_handle_0 = led_indicator_create(&config);
//     ESP_ERROR_CHECK(led_indicator_start(led_handle_0, BLINK_HEART));

//     gpio_reset_pin(10);
//     gpio_set_direction(10, GPIO_MODE_OUTPUT);
//     gpio_set_level(10, s_led_state);

//     gpio_reset_pin(15);
//     gpio_set_direction(15, GPIO_MODE_OUTPUT);
//     gpio_set_level(15, s_led_state);
// }

int pins_out[8] = {5, 15, 10, 2, 9, 4, 13, 12};

void my_led_init(void)
{
    for (int n=0; n<8; n++) {
        gpio_reset_pin(pins_out[n]);
        gpio_set_direction(pins_out[n], GPIO_MODE_OUTPUT);
        gpio_set_level(10, 0);
    }

    led_indicator_gpio_config_t led_indicator_gpio_config = {
        .gpio_num = 5,
        .is_active_level_high = 1,
    };

    led_indicator_config_t config = {
        .led_indicator_gpio_config = &led_indicator_gpio_config,
        .mode = LED_GPIO_MODE,
        .blink_lists = led_blink_lst,
        .blink_list_num = BLINK_NUM,
    };

    led_handle_0 = led_indicator_create(&config);   
}

void set_led(int pin, bool state)
{
    gpio_set_level(pin, state);
}

void set_leds(char *data)
{
    ESP_LOGI("TEST", "data= %s, len= %d", data, strlen(data));
    ESP_LOGI("TEST", "3rd char= %d", (int)data[2]);

    int dat = (int)strtol(data, NULL, 16);
    int state = 1;
    for (int n=0; n<8; n++) {
        gpio_set_level(pins_out[n], dat & state ? 1 : 0);
        state <<= 1;
    }    
}

void test_leds(void)
{
    // LEDs on
    gpio_set_level(pins_out[0], 1);
    gpio_set_level(pins_out[1], 1);
    gpio_set_level(pins_out[2], 1);
    // wait 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // LEDs off
    gpio_set_level(pins_out[0], 0);
    gpio_set_level(pins_out[1], 0);
    gpio_set_level(pins_out[2], 0);
}

void set_heart(bool state)
{
    if (state)
        ESP_ERROR_CHECK(led_indicator_start(led_handle_0, BLINK_HEART));
    else {
        ESP_ERROR_CHECK(led_indicator_stop(led_handle_0, BLINK_HEART));
        gpio_set_level(pins_out[0], 0);
    }
}