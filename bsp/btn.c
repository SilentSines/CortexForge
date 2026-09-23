#include "bsp.h"

#define BTN_COUNT_BASE 5	
#define BTN_FILTER_TIME 30 
#define BTN_FIFO_SIZE 10	

typedef enum
{
	BTN_NONE = 0,
	BTN_PRESS,	 
	BTN_RELEASE, 
	BTN_LONG,		 
	BTN_SHORT,	
	BTN_EVEN_MAX = BTN_SHORT,
} btn_event_t;

typedef struct
{
	btn_state_t state;			
	uint16_t filter_cnt;			 
	uint16_t long_cnt;	 
	uint16_t long_time;	 
	uint16_t repeat_cnt; 
	uint16_t repeat_speed; 
} btn_cfg_t;

static const gpio_pin_t btn_pin_table[BTN_NUM] =
{
	GPIO_PB9,
};

static btn_cfg_t btn[BTN_NUM] =
{
	{BTN_STATE_RELEASE, 0, 0, 1000, 0, 0}, 
}; 

static struct
{
	uint8_t buf[BTN_FIFO_SIZE]; 
	__IO uint8_t read;							
	__IO uint8_t write;							
} btn_fifo;			 

static btn_state_t btn_pin_active(uint8_t id)
{
	if (GPIO_READ(btn_pin_table[id]))
	{
		return BTN_STATE_RELEASE;
	}
	else
	{
		return BTN_STATE_PRESS;
	}
}

void btn_init(void)
{
	for (uint8_t i = 0; i < BTN_NUM; i++)
	{
		gpio_pin_init(btn_pin_table[i], GPIO_IN_PULLUP, GPIO_AF_NONE);
	} 
}

void btn_put_event(uint8_t event)
{
	btn_fifo.buf[btn_fifo.write] = event;
	if (++btn_fifo.write >= BTN_FIFO_SIZE)
	{
		btn_fifo.write = 0;
	}
}

uint8_t btn_get_event(void)
{
	uint8_t event;

	if (btn_fifo.read == btn_fifo.write)
	{
		return BTN_EVENT_NONE;
	}
	else
	{
		event = btn_fifo.buf[btn_fifo.read];
		if (++btn_fifo.read >= BTN_FIFO_SIZE)
		{
			btn_fifo.read = 0;
		}
		return event;
	}
}

btn_state_t btn_get_state(btn_id_t id)
{
	return btn[id].state;
}

void btn_set_cfg(btn_id_t id, uint16_t long_time, uint16_t repeat_speed)
{
	btn[id].long_time = long_time;			
	btn[id].repeat_speed = repeat_speed; 
	btn[id].repeat_cnt = 0;			
}

void btn_clr_fifo(void)
{
	btn_fifo.read = btn_fifo.write;
}

void btn_monitor(void)
{
	btn_cfg_t *temp;
	uint8_t i;

	for (i = 0; i < BTN_NUM; i++)
	{
		temp = &btn[i];
		if (btn_pin_active(i)) 
		{
			if (temp->filter_cnt < BTN_FILTER_TIME)
			{
				temp->filter_cnt = BTN_FILTER_TIME;
			}
			else if (temp->filter_cnt < 2 * BTN_FILTER_TIME)
			{
				temp->filter_cnt += BTN_COUNT_BASE;
			}
			else
			{
				if (temp->state == BTN_STATE_RELEASE)
				{
					temp->state = BTN_STATE_PRESS;
					btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_PRESS));
				}
				if (temp->long_time > 0)
				{
					if (temp->long_cnt < temp->long_time)
					{
						temp->long_cnt += BTN_COUNT_BASE;
						if (temp->long_cnt == temp->long_time)
						{
							btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_LONG));
						}
					}
					else
					{
						if (temp->repeat_speed > 0)
						{
							temp->repeat_cnt += BTN_COUNT_BASE;
							if (temp->repeat_cnt >= temp->repeat_speed)
							{
								temp->repeat_cnt = 0;
								btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_PRESS));
							}
						}
					}
				}
			}
		}
		else
		{
			if (temp->filter_cnt > BTN_FILTER_TIME)
			{
				temp->filter_cnt = BTN_FILTER_TIME;
			}
			else if (temp->filter_cnt != 0)
			{
				temp->filter_cnt -= BTN_COUNT_BASE;
			}
			else
			{
				if (temp->state == BTN_STATE_PRESS)
				{
					temp->state = BTN_STATE_RELEASE;
					if ((temp->long_time) && (temp->long_cnt < temp->long_time))
					{
						btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_SHORT));
					}
					btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_RELEASE));
					temp->long_cnt = 0;
					temp->repeat_cnt = 0;
				}
			}
		}
	}
}