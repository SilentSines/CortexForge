#ifndef BTN_H
#define BTN_H

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

void btn_init(void);
void btn_monitor(void);
void btn_put_event(uint8_t event);
uint8_t btn_get_event(void);
btn_state_t btn_get_state(btn_id_t id);
void btn_set_cfg(btn_id_t id, uint16_t long_time, uint16_t repeat_speed);
void btn_clr_fifo(void);

#endif