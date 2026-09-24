#ifndef BSP_H
#define BSP_H

#include "hal.h"

#include "log.h"
#include "basic_lib.h"

#include "led.h"
#include "bat.h"
#include "btn.h"
#include "encoder.h"
#include "ir.h"
#include "wm8978.h"
#include "kt_rx.h"

#include "tft.h"
#include "oled.h"

void bsp_init(void);

#endif