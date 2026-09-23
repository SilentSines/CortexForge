#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void (*task_cb_t)(void);

bool task_register(task_cb_t func, int32_t repeat, uint32_t period);
bool task_logout(task_cb_t func);
bool task_set_repeat(task_cb_t func, int16_t repeat);
bool task_set_period(task_cb_t func, uint32_t period);
void task_running(uint32_t tick);

#endif
