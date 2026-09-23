#include "msg_manager.h"

static bool is_subscribed(msg_account_t *subscriber, msg_account_t *publisher)
{
    for (uint8_t i = 0; i < publisher->subscriber_num; i++) 
    {
        if (publisher->subscribers[i] == subscriber) 
        {
            return true;
        }
    }
    return false;
}

bool msg_subscribe(msg_account_t *subscriber, msg_account_t *publisher)
{
    if (subscriber == NULL || publisher == NULL) 
    {
        return false;
    }

    if (subscriber == publisher) 
    {
        return false;
    }
    
    for (uint8_t i = 0; i < subscriber->publisher_num; i++) 
    {
        if (subscriber->publishers[i] == publisher) 
        {
            return false; 
        }
    }
    
    if (subscriber->publisher_num >= MSG_ACCOUNT_PUBLISHER_MAX) 
    {
        return false;
    }
    if (publisher->subscriber_num >= MSG_ACCOUNT_SUBSCRIBER_MAX) 
    {
        return false;
    }

    subscriber->publishers[subscriber->publisher_num++] = publisher;
    publisher->subscribers[publisher->subscriber_num++] = subscriber;
    
    return true;
}

bool msg_unsubscribe(msg_account_t *subscriber, msg_account_t *publisher)
{
    if (subscriber == NULL || publisher == NULL) 
    {
        return false;
    }

    if (subscriber == publisher) 
    {
        return false;
    }

    bool found = false;

    for (uint8_t i = 0; i < subscriber->publisher_num; i++) 
    {
        if (subscriber->publishers[i] == publisher) 
        {
            subscriber->publishers[i] = subscriber->publishers[subscriber->publisher_num - 1];
            subscriber->publishers[subscriber->publisher_num - 1] = NULL;
            subscriber->publisher_num--;
            found = true;
            break;
        }
    }

    if (!found) 
    {
        return false;
    }

    for (uint8_t i = 0; i < publisher->subscriber_num; i++) 
    {
        if (publisher->subscribers[i] == subscriber) 
        {
            publisher->subscribers[i] = publisher->subscribers[publisher->subscriber_num - 1];
            publisher->subscribers[publisher->subscriber_num - 1] = NULL;
            publisher->subscriber_num--;
            break;
        }
    }

    return true;
}

bool msg_publish(msg_account_t *publisher, void *data, uint32_t size)
{
    if (publisher == NULL || data == NULL || size == 0) 
    {
        return false;
    }

    msg_event_param_t param;
    param.event = MSG_EVENT_PUB_PUBLISH;
    param.send = publisher;
    param.data = data;
    param.size = size;

    for (uint8_t i = 0; i < publisher->subscriber_num; i++) 
    {
        msg_account_t *sub = publisher->subscribers[i];
        if (sub && sub->event_cb) 
        {
            param.recv = sub;
            sub->event_cb(&param);
        }
    }
    return true;
}

bool msg_pull(msg_account_t *subscriber, msg_account_t *publisher, void *data, uint32_t size)
{
    if (subscriber == NULL || publisher == NULL || data == NULL || size == 0) 
    {
        return false;
    }
    if (publisher->event_cb == NULL) 
    {
        return false;
    }
    if (!is_subscribed(subscriber, publisher)) 
    {
        return false;
    }

    msg_event_param_t param;
    param.event = MSG_EVENT_SUB_PULL;
    param.send = subscriber;
    param.recv = publisher;
    param.data = data;
    param.size = size;

    return publisher->event_cb(&param) == MSG_RES_OK;
}

bool msg_notify(msg_account_t *subscriber, msg_account_t *publisher, void *data, uint32_t size)
{
    if (subscriber == NULL || publisher == NULL || data == NULL || size == 0) 
    {
        return false;
    }
    if (publisher->event_cb == NULL) 
    {
        return false;
    }
    if (!is_subscribed(subscriber, publisher)) 
    {
        return false;
    }

    msg_event_param_t param;
    param.event = MSG_EVENT_NOTIFY;
    param.send = subscriber;
    param.recv = publisher;
    param.data = data;
    param.size = size;

    return publisher->event_cb(&param) == MSG_RES_OK;
}