#include "hal.h"
#include "bsp.h"

static void btn_led_test(btn_event_t e)
{
    switch (e)
    {
    case BTN_EVENT_PRESS: led_toggle(LED_D1);
        /* code */
        break;
    
    default:
        break;
    }
}

int main(void)
{
    hal_init();
    bsp_init();

    btn_register_cb(BTN_B1, btn_led_test);

    while (1)
    {
  
        btn_monitor();
        delay_ms(5);
    }
}   
