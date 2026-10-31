#include "bsp.h"

#define BTN_COUNT_BASE 5	
#define BTN_FILTER_TIME 30 
#define BTN_FIFO_SIZE 10	

#define BTN_PRESS     1
#define BTN_RELEASE   2
#define BTN_LONG      3
#define BTN_SHORT     4
#define BTN_EVEN_MAX  4

typedef struct
{
	btn_state_t state;			
	uint16_t filter_cnt;			 
	uint16_t long_cnt;	 
	uint16_t long_time;	 
	uint16_t repeat_cnt; 
	uint16_t repeat_speed; 
#if BTN_USE_CALLBACK == 1
	btn_cb_t cb;
#endif
} btn_dev_t;

static const gpio_pin_t btn_pin_table[BTN_NUM] =
{
	GPIO_PB9,
};

static btn_dev_t btn[BTN_NUM]; 

#if BTN_USE_FIFO
static struct
{
	uint8_t buf[BTN_FIFO_SIZE]; 
	__IO uint8_t read;							
	__IO uint8_t write;							
} btn_fifo;	
#endif		 

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

void btn_monitor(void)
{
	btn_dev_t *dev;
	uint8_t i;

	for (i = 0; i < BTN_NUM; i++)
	{
		dev = &btn[i];
		if (btn_pin_active(i)) 
		{
			if (dev->filter_cnt < BTN_FILTER_TIME)
			{
				dev->filter_cnt = BTN_FILTER_TIME;
			}
			else if (dev->filter_cnt < 2 * BTN_FILTER_TIME)
			{
				dev->filter_cnt += BTN_COUNT_BASE;
			}
			else
			{
				if (dev->state == BTN_STATE_RELEASE)
				{
					dev->state = BTN_STATE_PRESS;

				#if BTN_USE_FIFO
					btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_PRESS));
				#endif

				#if BTN_USE_CALLBACK
					if (dev->cb)
					{
						dev->cb(BTN_PRESS);
					}
				#endif
				}
				if (dev->long_time > 0)
				{
					if (dev->long_cnt < dev->long_time)
					{
						dev->long_cnt += BTN_COUNT_BASE;
						if (dev->long_cnt == dev->long_time)
						{
						#if BTN_USE_FIFO
							btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_LONG));
						#endif

						#if BTN_USE_CALLBACK
							if (dev->cb)
							{
								dev->cb(BTN_LONG);
							}
						#endif
						}
					}
					else
					{
						if (dev->repeat_speed > 0)
						{
							dev->repeat_cnt += BTN_COUNT_BASE;
							if (dev->repeat_cnt >= dev->repeat_speed)
							{
								dev->repeat_cnt = 0;

							#if BTN_USE_FIFO
								btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_PRESS));
							#endif

							#if BTN_USE_CALLBACK
								if (dev->cb)
								{
									dev->cb(BTN_PRESS);
								}
							#endif
							}
						}
					}
				}
			}
		}
		else
		{
			if (dev->filter_cnt > BTN_FILTER_TIME)
			{
				dev->filter_cnt = BTN_FILTER_TIME;
			}
			else if (dev->filter_cnt != 0)
			{
				dev->filter_cnt -= BTN_COUNT_BASE;
			}
			else
			{
				if (dev->state == BTN_STATE_PRESS)
				{
					dev->state = BTN_STATE_RELEASE;
					if ((dev->long_time) && (dev->long_cnt < dev->long_time))
					{
					#if BTN_USE_FIFO
						btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_SHORT));
					#endif

					#if BTN_USE_CALLBACK
						if (dev->cb)
						{
							dev->cb(BTN_SHORT);
						}
					#endif
					}

				#if BTN_USE_FIFO	
					btn_put_event((uint8_t)(BTN_EVEN_MAX * i + BTN_RELEASE));
				#endif

				#if BTN_USE_CALLBACK
					if (dev->cb)
					{
						dev->cb(BTN_RELEASE);
					}
				#endif

					dev->long_cnt = 0;
					dev->repeat_cnt = 0;
				}
			}
		}
	}
}

btn_state_t btn_get_state(btn_id_t id)
{
	if (id >= BTN_NUM)
	{
		return BTN_RELEASE;
	}
	
	return btn[id].state;
}

void btn_set_cfg(btn_id_t id, uint16_t long_time, uint16_t repeat_speed)
{
	if (id >= BTN_NUM)
	{
		return;
	}

	btn[id].long_time = long_time;			
	btn[id].repeat_speed = repeat_speed; 
	btn[id].repeat_cnt = 0;			
}

#if BTN_USE_FIFO == 1
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

void btn_clr_fifo(void)
{
	btn_fifo.read = btn_fifo.write;
}
#endif

#if BTN_USE_CALLBACK == 1
void btn_register_cb(btn_id_t id, btn_cb_t cb)
{
	if (id >= BTN_NUM)
	{
		return;
	}

	btn[id].cb = cb;
}
#endif