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
    }
}   
