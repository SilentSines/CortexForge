#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    PAGE_STATE_IDLE,
    PAGE_STATE_LOAD,
    PAGE_STATE_APPEAR,
    PAGE_STATE_ACTIVITY,
    PAGE_STATE_DISAPPEAR,
    PAGE_STATE_UNLOAD,
} page_state_t;

typedef void (*page_cb_t)(void);

typedef struct
{
    bool load;
    bool delete;
    page_state_t state;
    page_cb_t view_load;
    page_cb_t view_appear;
    page_cb_t view_disappear;  //需在合适时机调用page_view_disappear_complete
    page_cb_t view_unload;  
} page_t;

#define PAGE_CREATE(x) page_t page_##x = {false, false, PAGE_STATE_IDLE, view_load, view_appear,  view_disappear,  view_unload};

bool page_replace(page_t *page);
bool page_push(page_t *page);
bool page_pop(void);
bool page_pop_to(page_t *page);
void page_view_disappear_complete(page_t *page);
page_t *page_cur(void);

#endif
