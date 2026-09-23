#include "bsp.h"

#define ENC_FIFO_SIZE 10	
#define ENC_FILTER 20

#define HALF_WAVE 1
#define FULL_WAVE 0

#define ENC_PHASE_A_PIN	GPIO_PB7
#define ENC_PHASE_B_PIN GPIO_PB6

static struct
{
	int8_t buf[ENC_FIFO_SIZE]; 
	__IO uint8_t read;							
	__IO uint8_t write;							
} encoder_fifo;	

static bool encoder_en = true;

void encoder_init(void)
{
	gpio_exti_init(ENC_PHASE_A_PIN, EXTI_BOTH, 0, encoder_monitor);
	gpio_pin_init(ENC_PHASE_B_PIN, GPIO_IN_FLOATING, GPIO_DRIVE_STRENGTH_MODERATE);
}

void encoder_monitor(void)
{
#if HALF_WAVE
    static uint32_t time_per = 0;
    uint32_t time_cur = millis();
	uint8_t phase_a, phase_b;
	int8_t dir;

    if ((time_cur - time_per) > ENC_FILTER)
    {
        time_per = time_cur;
		phase_a = GPIO_READ(ENC_PHASE_A_PIN);
		phase_b = GPIO_READ(ENC_PHASE_B_PIN);
        dir = (phase_a == phase_b) ? ENC_DIR_CW : ENC_DIR_CCW;
		if (encoder_en)
		{
			encoder_put_event(dir);
		}
    }
#elif FULL_WAVE
	static int8_t dir_prev = ENC_DIR_NONE;
	static uint8_t debounce_cnt = 0;
	uint8_t phase_a, phase_b;
	int8_t dir_cur;

	phase_a = GPIO_READ(ENC_PHASE_A_PIN);
	phase_b = GPIO_READ(ENC_PHASE_B_PIN);
	dir_cur = (phase_a == phase_b) ? ENC_DIR_CW : ENC_DIR_CCW;
	if (dir_cur != dir_prev)
	{
		debounce_cnt = 1;
		dir_prev = dir_cur;
	}
	else
	{
		debounce_cnt++;
		if (debounce_cnt == 2)
		{
			debounce_cnt = 0;
			dir_prev = ENC_DIR_NONE;
			if (encoder_en)
			{
				encoder_put_event(dir_cur);
			}
		}
	}
#endif
}

void encoder_put_event(int8_t event)
{
    encoder_fifo.buf[encoder_fifo.write] = event;
	if (++encoder_fifo.write >= ENC_FIFO_SIZE)
	{
		encoder_fifo.write = 0;
	}
}

int8_t encoder_get_event(void)
{
    int8_t event;

	if (encoder_fifo.read == encoder_fifo.write)
	{
		return ENC_DIR_NONE;
	}
	else
	{
		event = encoder_fifo.buf[encoder_fifo.read];
		if (++encoder_fifo.read >= ENC_FIFO_SIZE)
		{
			encoder_fifo.read = 0;
		}
		return event;
	}
}

void encoder_clr_fifo(void)
{
    encoder_fifo.read = encoder_fifo.write;
}

void encoder_cmd(bool en)
{
	encoder_en = en;
	if (!encoder_en)
	{
		encoder_clr_fifo(); 
	}
}