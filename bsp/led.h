#ifndef LED_H
#define LED_H

typedef enum
{
    LED_D1,
    LED_NUM,
} led_id_t;

typedef enum
{
    LED_OFF,
    LED_ON,
} led_state_t;

void led_init(void);
void led_on(led_id_t id);
void led_off(led_id_t id);
void led_toggle(led_id_t id);
led_state_t led_get_state(led_id_t id);

#endif