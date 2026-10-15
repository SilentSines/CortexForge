#include "bsp.h"

typedef struct
{
    TIM_TypeDef *instance;
    IRQn_Type irq;         
} timer_map_t;

static const timer_map_t timer_map_table[] =
{
    {TIM1, TIM1_UP_TIM10_IRQn},
    {TIM2, TIM2_IRQn},
    {TIM3, TIM3_IRQn},
    {TIM4, TIM4_IRQn},
    {TIM5, TIM5_IRQn},
    {TIM6, TIM6_DAC_IRQn},
    {TIM7, TIM7_IRQn},
    {TIM8, TIM8_UP_TIM13_IRQn},
};

static cb_t timer_cb[TIMER_UNIT_MAX];

static void time_factorization(uint32_t time, uint32_t clock, uint16_t *pPeriod, uint16_t *pPrescaler) 
{
    uint32_t cycles_permicros;
    uint32_t product;
    uint16_t period, prescaler;

    cycles_permicros = clock / 1000000;
    product = time * cycles_permicros;
    if (product < cycles_permicros * 30)
    {
        period = 10;
        prescaler = product / 10;
    }
    else if (product < 65535 * 1000)
    {
        period = product / 1000;
        prescaler = product / period;
    }
    else
    {
        period = product / 20000;
        prescaler = product / period;
    }
    *pPeriod = period;
    *pPrescaler = prescaler;
}

void timer_init(timer_unit_t timer, uint32_t time, uint32_t irqp, cb_t func)
{
    uint16_t period = 0;
    uint16_t prescaler = 0;
    TIM_HandleTypeDef htim = {0};

    switch (timer)
    {
        case TIMER_UNIT_1 : __HAL_RCC_TIM1_CLK_ENABLE(); break;
        case TIMER_UNIT_2 : __HAL_RCC_TIM2_CLK_ENABLE(); break;
        case TIMER_UNIT_3 : __HAL_RCC_TIM3_CLK_ENABLE(); break;
        case TIMER_UNIT_4 : __HAL_RCC_TIM4_CLK_ENABLE(); break;
        case TIMER_UNIT_5 : __HAL_RCC_TIM5_CLK_ENABLE(); break;
        case TIMER_UNIT_6 : __HAL_RCC_TIM6_CLK_ENABLE(); break;
        case TIMER_UNIT_7 : __HAL_RCC_TIM7_CLK_ENABLE(); break;
        case TIMER_UNIT_8 : __HAL_RCC_TIM7_CLK_ENABLE(); break;
        default : return; break;
    }

    const timer_map_t *map = &timer_map_table[timer];

    time_factorization(time, SystemCoreClock, &period, &prescaler);
    htim.Instance = map->instance;
    htim.Init.Prescaler = prescaler - 1;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = period - 1;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim) != HAL_OK)
    {
        hal_error_handler();
    }
    __HAL_TIM_ENABLE_IT(&htim, TIM_IT_UPDATE);
    HAL_NVIC_SetPriority(map->irq, irqp, 0);
	HAL_NVIC_EnableIRQ(map->irq);
    timer_cb[timer] = func; 
}

void timer_cmd(timer_unit_t timer, bool en)
{
    if (en)
    {
        SET_BIT(timer_map_table[timer].instance->CR1, TIM_CR1_CEN);
    }
    else
    {
        CLEAR_BIT(timer_map_table[timer].instance->CR1, TIM_CR1_CEN);
    }
}

#define TIM_IRQHANDLER(x, y)          \
    if (READ_BIT(x->SR, TIM_SR_UIF))  \
    {                                 \
        CLEAR_BIT(x->SR, TIM_SR_UIF); \
        if (timer_cb[y] != NULL)      \
        {                             \
            timer_cb[y]();            \
        }                             \
    }

void TIM1_UP_TIM10_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM1, 0);
}

void TIM2_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM2, 1);
}

void TIM3_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM3, 2);
}

void TIM4_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM4, 3);
}

void TIM5_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM5, 4);
}

void TIM6_DAC_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM6, 5);
}

void TIM7_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM7, 6);
}

void TIM8_UP_TIM13_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM8, 7);
}