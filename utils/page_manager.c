#include "page_manager.h"

#define PAGE_NUM 20
#define PAGE_ID_INVALID 0xFF 

static struct 
{
    page_t *cur;
    page_t *prev;
    page_t *stack[PAGE_NUM];
    uint8_t count;
} page_mgr;

static uint8_t page_find(page_t *page)
{
    uint8_t i;

    for (i = 0; i < page_mgr.count; i++)
    {
        if (page_mgr.stack[i] == page)
        {
            return i;
        }
    }
    return PAGE_ID_INVALID;
}

static bool page_register(page_t *page)
{
    if ((page == NULL) || (page_mgr.count >= PAGE_NUM))
    {
        return false;
    }
    if (page_find(page) != PAGE_ID_INVALID)
    {
        return false;
    }
    page_mgr.stack[page_mgr.count++] = page;
    return true;
}

static bool page_logout(page_t *page)
{
    uint8_t i, id; 
    uint8_t index;

    if ((page == NULL) || (!page_mgr.count))
    {
        return false;
    }
    id = page_find(page); 
    if (id == PAGE_ID_INVALID)
    {
        return false;
    }
    index = page_mgr.count - 1;
    for (i = id; i < index; i++)  
    {
        page_mgr.stack[i] = page_mgr.stack[i + 1];
    }
    page_mgr.stack[index] = NULL;
    page_mgr.count--;
    return true;
}

static void page_state_update(page_t *page)
{
    if (page == NULL)
    {
        return;
    }
    switch (page->state)
    {
        case PAGE_STATE_LOAD :
        { 
            page->view_load();
            page->state = PAGE_STATE_APPEAR;
            page->load = true;
            page->delete = false;
            page_state_update(page);
            break;
        }

        case PAGE_STATE_APPEAR :
        {
            page->view_appear();
            page->state = PAGE_STATE_ACTIVITY;
            break;
        }

        case PAGE_STATE_ACTIVITY :
        {
            page->state = PAGE_STATE_DISAPPEAR;
            page_state_update(page);
            break;
        }

        case PAGE_STATE_DISAPPEAR :
        { 
            if(page->delete)
            {
                page->state = PAGE_STATE_UNLOAD;
            }
            else
            {
                page->state = PAGE_STATE_IDLE;
            }
            page->view_disappear();
            break;
        }

        case PAGE_STATE_UNLOAD :
        { 
            page->view_unload();
            page->state = PAGE_STATE_IDLE;
            page->load = false;
            break;
        }

        default : break;
    }
}

static bool page_switch(page_t *page, bool delet)
{
    if (page == NULL)
    {
        return false;
    }
    page_mgr.prev = page_mgr.cur;
    if ((page_mgr.prev != NULL) && delet)
    {
        page_mgr.prev->delete = true;
    }
    page_mgr.cur = page;
    if (page_mgr.cur->load)
    {
        page_mgr.cur->state = PAGE_STATE_APPEAR;
    }
    else
    {
        page_mgr.cur->state = PAGE_STATE_LOAD;
    }
    page_state_update(page_mgr.prev);
    page_state_update(page_mgr.cur);
    return true;
}

bool page_replace(page_t *page)
{
    if (page == NULL)
    {
        return false;
    }
    if (page_find(page) != PAGE_ID_INVALID)
    {
        return false;
    }
    if (page_logout(page_mgr.cur) == false)
    {
        return false;
    }
    if (page_register(page) == false)
    {
        return false;
    }
    return page_switch(page, true);
}

bool page_push(page_t *page)
{
    if (page == NULL)
    {
        return false;
    }
    if(page_register(page) == false)
    {
        return false;
    }
    return page_switch(page, false);
}

bool page_pop(void)
{
    if (page_mgr.count < 2)
    {
        return false;
    }
    if(page_logout(page_mgr.cur) == false)
    {
        return false;
    }
    return page_switch(page_mgr.stack[page_mgr.count - 1], true);
}

bool page_pop_to(page_t *page)
{
    uint8_t i;
    uint8_t index, count;
    page_t *delect_page;

    if (page == NULL)
    {
        return false;
    }
    if (page_mgr.count < 3)
    {
        return false;
    }
    if (page == page_mgr.cur)
    {
        return false;
    }
    index = page_find(page);
    if (index == PAGE_ID_INVALID)
    {
        return false;
    }
    count = page_mgr.count - 1;
    for (i = count; i > index; i--)
    {
        delect_page = page_mgr.stack[i];
        delect_page->load = false;
        if (i == count)
        {
            delect_page->state = PAGE_STATE_UNLOAD;
            delect_page->view_disappear();
        }
        else
        {
            delect_page->state = PAGE_STATE_IDLE;
            delect_page->view_unload();
        }
        page_mgr.stack[i] = NULL;
        page_mgr.count--;
    }
    page_mgr.cur = page_mgr.stack[index];
    page_mgr.prev = NULL;
    page_mgr.cur->view_appear();
    page_mgr.cur->state = PAGE_STATE_ACTIVITY;
    return true;
}

void page_view_disappear_complete(page_t *page)
{
    page_state_update(page);
}

page_t *page_cur(void)
{
    return page_mgr.cur;
}