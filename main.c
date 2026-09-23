#include "hal.h"
#include "bsp.h"

int main(void)
{
    hal_init();
    bsp_init();

    while (1)
    {
        led_toggle(LED_D1);
        delay_ms(500);
    }
}   
