#ifndef MSG_MANAGER_H
#define MSG_MANAGER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MSG_ACCOUNT_PUBLISHER_MAX  10
#define MSG_ACCOUNT_SUBSCRIBER_MAX 10

typedef enum
{
    MSG_EVENT_NONE,
    MSG_EVENT_PUB_PUBLISH, 
    MSG_EVENT_SUB_PULL,    
    MSG_EVENT_NOTIFY,     
    MSG_EVENT_MAX
} msg_event_t;

typedef enum
{
    MSG_RES_OK                  =  0,
    MSG_RES_UNKNOW              = -1,
    MSG_RES_SIZE_MISMATCH       = -2,
    MSG_RES_UNSUPPORTED_REQUEST = -3,
    MSG_RES_NO_CALLBACK         = -4,
    MSG_RES_NO_CACHE            = -5,
    MSG_RES_NO_COMMITED         = -6,
    MSG_RES_NOT_FOUND           = -7,
    MSG_RES_PARAM_ERROR         = -8
} msg_res_t;

typedef struct
{
    msg_event_t event; 
    struct msg_account* send;    
    struct msg_account* recv;    
    void* data;    
    uint32_t size;   
} msg_event_param_t;

typedef struct msg_account
{
    msg_res_t (*event_cb)(msg_event_param_t*);
    struct msg_account *publishers[MSG_ACCOUNT_PUBLISHER_MAX];
    uint16_t publisher_num;
    struct msg_account *subscribers[MSG_ACCOUNT_SUBSCRIBER_MAX];
    uint16_t subscriber_num;
} msg_account_t;

#define MSG_ACCOUNT_CREATE(x) msg_account_t account_##x = {msg_handle, {NULL}, 0, {NULL}, 0}

bool msg_subscribe(msg_account_t *subscriber, msg_account_t *publisher);
bool msg_unsubscribe(msg_account_t *subscriber, msg_account_t *publisher);
bool msg_publish(msg_account_t *publisher, void *data, uint32_t size);
bool msg_pull(msg_account_t *subscriber, msg_account_t *publisher, void *data, uint32_t size);
bool msg_notify(msg_account_t *subscriber, msg_account_t *publisher, void *data, uint32_t size);

#endif