#include "hal.h"

void wdt_init(uint32_t time)
{
    uint8_t i;
    uint32_t reload_value;
    wdt_division_type division;

    for(i = 0; i < (WDT_CLK_DIV_256 + 1); i++)
    {
        reload_value = (uint64_t)time * 40000 / (4 << i) / 1000;

        if(reload_value <= 0xFFF)
        {
            division = (wdt_division_type)i;
            break;
        }
    }

    if(reload_value > 0xFFF)
    {
        return;
    }

    wdt_register_write_enable(TRUE);
    wdt_divider_set(division);
    wdt_reload_value_set(reload_value - 1);
    wdt_counter_reload();
    wdt_enable();
}

void wdt_feed(void)
{
    wdt_counter_reload();
}