#ifndef BTN_H
#define BTN_H

#define BTN_USE_FIFO		0   
#define BTN_USE_CALLBACK    1  

#if (BTN_USE_FIFO == 1) && (BTN_USE_CALLBACK == 1)
    #error "BTN_USE_FIFO and BTN_USE_CALLBACK cannot be enabled simultaneously!"
#endif

typedef enum
{
	BTN_STATE_RELEASE, 
	BTN_STATE_PRESS		
} btn_state_t;

typedef enum
{
	BTN_B1,
	BTN_NUM,
} btn_id_t;

#if BTN_USE_FIFO == 1
enum
{
#define BTN_EVENT_DEF(x) \
	x##_PRESS,			 \
	x##_RELEASE,		 \
	x##_LONG,			 \
	x##_SHORT,

	BTN_EVENT_NONE,
	BTN_EVENT_DEF(BTN1)
};
#endif

#if BTN_USE_CALLBACK == 1
typedef enum
{
	BTN_EVENT_NONE,
	BTN_EVENT_PRESS,			 
	BTN_EVENT_RELEASE,		 
	BTN_EVENT_LONG,			 
	BTN_EVENT_SHORT,
} btn_event_t;

typedef void (*btn_cb_t)(btn_event_t event);
#endif

void btn_init(void);
void btn_monitor(void);
btn_state_t btn_get_state(btn_id_t id);
void btn_set_cfg(btn_id_t id, uint16_t long_time, uint16_t repeat_speed);

#if BTN_USE_FIFO == 1
void btn_put_event(uint8_t event);
uint8_t btn_get_event(void);
void btn_clr_fifo(void);
#endif

#if BTN_USE_CALLBACK == 1
void btn_register_cb(btn_id_t id, btn_cb_t cb);
#endif

#endif