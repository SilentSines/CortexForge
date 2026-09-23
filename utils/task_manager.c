#include "task_manager.h"

#define TASK_NUM 20
#define TASK_ID_INVALID 0xFF 
#define PRIORITY_ENABLE 0
#define AUTO_LOGOUT_ENABLE 0

typedef struct 
{
    task_cb_t func;  
    int32_t repeat;           // -1：永久， 0：停止， 其他: 对应次数   
    uint32_t period;           
    uint32_t last_time;  
} task_item_t;

static struct 
{
    task_item_t pool[TASK_NUM];
    uint8_t count;
} task_mgr;

static uint8_t task_find(task_cb_t func)
{
    uint8_t i;

    for (i = 0; i < task_mgr.count; i++)
    {
        if (task_mgr.pool[i].func == func)
        {
            return i;
        }
    }
    return TASK_ID_INVALID;
}

bool task_register(task_cb_t func, int32_t repeat, uint32_t period)
{
    uint8_t id; 
    uint8_t index;
    
    if (func == NULL || (task_mgr.count >= TASK_NUM))
    {
        return false;
    }
    id = task_find(func);   
    if (id == TASK_ID_INVALID)
    {
        index = task_mgr.count;
        task_mgr.pool[index].func = func;
        task_mgr.pool[index].repeat = repeat;
        task_mgr.pool[index].period = period;
        task_mgr.pool[index].last_time = 0;
        task_mgr.count++;
    }
    else
    {
        task_mgr.pool[id].repeat = repeat;
        task_mgr.pool[id].period = period;
    }
    return true;
}

bool task_logout(task_cb_t func)
{
    uint8_t i, id; 
    uint8_t index;

    if ((func == NULL) || (!task_mgr.count))
    {
        return false;
    }
    id = task_find(func); 
    if (id == TASK_ID_INVALID)
    {
        return false;
    }
    index = task_mgr.count - 1;
    for (i = id; i < index; i++)
    {
        task_mgr.pool[i] = task_mgr.pool[i + 1];
    }
    task_mgr.pool[index].func = NULL;
    task_mgr.count--;
    return true;
}

bool task_set_repeat(task_cb_t func, int16_t repeat)
{
    uint8_t id;

    if (func == NULL)
    {
        return false;
    }
    id = task_find(func);   
    if (id == TASK_ID_INVALID)
    {
        return false;
    }
    task_mgr.pool[id].repeat = repeat;
    return true;
}

bool task_set_period(task_cb_t func, uint32_t period)
{
    uint8_t id;

    if (func == NULL)
    {
        return false;
    }
    id = task_find(func);   
    if (id == TASK_ID_INVALID)
    {
        return false;
    }
    task_mgr.pool[id].period = period;
    return true;
}

void task_running(uint32_t tick)
{
    uint8_t i;
    uint32_t tick_elaps;
    task_item_t *item;

    for (i = 0; i < task_mgr.count; i++)
    {
        item = &(task_mgr.pool[i]);
        if ((item->repeat == 0) || (item->func == NULL))
        {
            continue;
        }
        tick_elaps = tick - item->last_time;
        if (tick_elaps >= item->period)
        {
            item->last_time = tick;
            item->func();
            if (item->repeat > 0)
            {
                item->repeat--;
            }
        #if PRIORITY_ENABLE
            break;
        #endif       
        } 
    }
}