#include "hal.h"
#include "bsp.h"
#include "usbd.h"

int main(void)
{
    hal_init();
    bsp_init();
    usbd_init();

    while (1)
    {
        usbd_monitor();
        static uint32_t last = 0;
        uint32_t now = millis();

        if (now - last >= 1000)
        {
            last = now;
            printf("Hello World\n");
        }
    }
}   
