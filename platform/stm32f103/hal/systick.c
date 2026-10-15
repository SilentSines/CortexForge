#include "bsp.h"

#define SYSTICK_PRIORITY 15

void systick_init(void)
{
	HAL_SYSTICK_Config(SystemCoreClock / (1000U / (uint32_t)uwTickFreq));
	HAL_NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY, 0U);
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
	return uwTick;
}

uint32_t micros(void)
{
	return (uwTick * 1000 + ((SystemCoreClock / (1000U / (uint32_t)uwTickFreq)) - SysTick->VAL) / (SystemCoreClock / 1000000));
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}