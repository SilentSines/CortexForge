#include "bsp.h"

#define LED_ON_LEVEL 1

static const gpio_pin_t led_pin_table[LED_NUM]=
{
    GPIO_PB4
};

void led_init(void)
{
	uint8_t i;
	
	for (i = 0; i < LED_NUM; i++)
	{
        led_off(i);

		gpio_pin_init(led_pin_table[i], GPIO_OUT_PP, GPIO_AF_NONE);
	}
}

void led_on(led_id_t id)
{
#if LED_ON_LEVEL
	GPIO_OUT_H(led_pin_table[id]);
#else
	GPIO_OUT_L(led_pin_table[id]);
#endif
}

void led_off(led_id_t id)
{
#if LED_ON_LEVEL
    GPIO_OUT_L(led_pin_table[id]);
#elif
	GPIO_OUT_H(led_pin_table[id]);
#endif
}

void led_toggle(led_id_t id)
{
	GPIO_TOGGLE(led_pin_table[id]);
}

led_state_t led_get_state(led_id_t id)
{
#if LED_ON_LEVEL
	return ((GPIO_READ(led_pin_table[id])) ? LED_ON : LED_OFF);
#elif
	return ((GPIO_READ(led_pin_table[id])) ? LED_OFF : LED_ON);
#endif
}