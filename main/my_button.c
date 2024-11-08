#include "my_button.h"
#include "esp_log.h"
#include "my_led.h"
#include "my_mqtt.h"
#include "esp_timer.h"

bool bProv = false;

static const char *TAG = "my_button";

// static void button_single_click_cb(void *arg,void *usr_data)
// {
//     static int ledstate = 0;

//     ESP_LOGI(TAG, "LEFT_CLICK");
//     ledstate = 1 - ledstate;
//     set_led(10, ledstate);
// }

int pins_in[8] = {27, 33, 32, 23, 19, 22, 14, 18};

static void but_timer_cb(void* arg)
{
    static int laststate = 0;
    char msg[12];
    int state = 0;
    for (int n=0; n<8; n++) {
        state >>= 1;
        state = state | (gpio_get_level(pins_in[n]) ? 0x80 : 0);
    }

    if (state != laststate)
    {
        start_clock();
        ESP_LOGI(TAG, "state= %d", state);
        snprintf(msg, 3, "%02X", state);
        msg[2] = '\0';
        ESP_LOGI(TAG, "msg= %s", msg);
        publish_msg(msg);
        laststate = state;
    }
}

// static void but_timer_cb(void* arg)
// {
//     static int laststate = 0;

//     int state = gpio_get_level(33);
//     if (state != laststate)
//     {
//         int64_t from = esp_timer_get_time();
//         start_clock();
//         if (state)
//             publish_msg("1");
//         else
//             publish_msg("0");
//         laststate = state;
//         int64_t to = esp_timer_get_time();
//         ESP_LOGI(TAG, "time elapsed: %lld us", to - from);
//     }
// }

void my_button_init()
{
    // // create gpio button
    // button_config_t gpio_btn_cfg = {
    //     .type = BUTTON_TYPE_GPIO,
    //     .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
    //     .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
    //     .gpio_button_config = {
    //         .gpio_num = 27,
    //         .active_level = 0,
    //     },
    // };
    // button_handle_t left_btn = iot_button_create(&gpio_btn_cfg);
    // if(NULL == left_btn) {
    //     ESP_LOGE(TAG, "Button left create failed");
    // }
    // iot_button_register_cb(left_btn, BUTTON_SINGLE_CLICK, button_single_click_cb, NULL);

//+++++++++++++++++++++++++++++++++++++++++++

// https://github.com/espressif/esp-idf/blob/master/examples/peripherals/gpio/generic_gpio/main/gpio_example_main.c
    for (int n=0; n<8; n++) {
        gpio_reset_pin(pins_in[n]);
        gpio_set_direction(pins_in[n], GPIO_MODE_INPUT);
    }

    // gpio_reset_pin(33);
    // gpio_set_direction(33, GPIO_MODE_INPUT);

    const esp_timer_create_args_t but_timer_args = {
        .callback = &but_timer_cb,
    };
    esp_timer_handle_t but_timer;
    ESP_ERROR_CHECK(esp_timer_create(&but_timer_args, &but_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(but_timer, 1000));

    bProv = !gpio_get_level(pins_in[0]);
}
