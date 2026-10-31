#ifndef HAL_H
#define HAL_H

#include "at32f403a_407.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CRITICAL_SECTION(...)                   \
    do                                          \
    {                                           \
        uint32_t primask_bit = __get_PRIMASK(); \
        __disable_irq();                        \
        __VA_ARGS__;                            \
        __set_PRIMASK(primask_bit);             \
    } while (0)

#define WDT_EN 0

typedef void (*cb_t)(void);           
typedef void (*cb_arg_t)(void*);

#include "adc.h"
#include "dma.h"
#include "flash.h"
#include "gpio.h"
#include "i2c.h"    
#include "i2s.h"
#include "pwm.h"
#include "spi.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"
#include "wdt.h"

void hal_init(void);
void hal_error_handler(void);

#endif