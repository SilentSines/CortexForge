#ifndef HAL_H
#define HAL_H

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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
typedef void (*cb_arg_t)(void *);

#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "flash.h"
#include "i2c.h"
#include "spi.h"

void hal_init(void);
void hal_error_handler(void);

#endif