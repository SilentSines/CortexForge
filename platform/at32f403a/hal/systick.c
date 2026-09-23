#include "hal.h"

enum
{
  TICK_FREQ_10HZ         = 100U,
  TICK_FREQ_100HZ        = 10U,
  TICK_FREQ_1KHZ         = 1U,
  TICK_FREQ_DEFAULT      = TICK_FREQ_1KHZ
};

__IO uint32_t tick;

#define SYSTICK_PRIORITY 15

void systick_init(void)
{
	systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
	SysTick_Config(SystemCoreClock / (1000U / TICK_FREQ_DEFAULT));
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SYSTICK_PRIORITY, 0));
}

void delay_ms(uint32_t ms)
{
	uint32_t prev = millis();

	while (1)
	{
		if ((millis() - prev) >= ms)
		{
			break;
		}
	}
}

void delay_us(uint32_t us)
{
	uint32_t tnow;
	uint32_t ticks = SystemCoreClock / 1000000 * us;
	uint32_t tcnt = 0;
	uint32_t told = SysTick->VAL;
	uint32_t reload = SysTick->LOAD;

	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
			{
				tcnt += told - tnow;
			}
			else
			{
				tcnt += reload - tnow + told;
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break;
			}
		}
	}
}

uint32_t millis(void)
{
	return tick;
}

uint32_t micros(void)
{
	return (tick * 1000 + ((SystemCoreClock / (1000U / (uint32_t)TICK_FREQ_DEFAULT)) - SysTick->VAL) / (SystemCoreClock / 1000000));
}

void SysTick_Handler(void)
{
	tick += (uint32_t)TICK_FREQ_DEFAULT;
}