#ifndef WDT_H
#define WDT_H

void wdt_init(uint32_t time);
void wdt_feed(void);

#endif