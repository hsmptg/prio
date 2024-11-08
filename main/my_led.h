#include "led_indicator.h"

typedef enum {
    BLINK_HEART,
    BLINK_NUM,
} led_blink_type_t;

static const blink_step_t heart_blink[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 100},
    {LED_BLINK_HOLD, LED_STATE_OFF, 100},
    {LED_BLINK_HOLD, LED_STATE_ON, 100},
    {LED_BLINK_HOLD, LED_STATE_OFF, 700},
    {LED_BLINK_LOOP, 0, 0},
};

void my_led_init(void);

void set_led(int pin, bool state);
void set_leds(char *data);
void set_heart(bool state);
void test_leds(void);