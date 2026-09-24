#include "bsp.h"

typedef struct
{
    TIM_TypeDef *tim;
    IRQn_Type irq;         
} timer_type_t;

static const timer_type_t timer_table[] =
{
    {TIM1, TIM1_UP_TIM10_IRQn},
    {TIM2, TIM2_IRQn},
    {TIM3, TIM3_IRQn},
    {TIM4, TIM4_IRQn},
    {TIM5, TIM5_IRQn},
    {TIM6, TIM6_DAC_IRQn},
    {TIM7, TIM7_IRQn},
    {TIM8, TIM8_UP_TIM13_IRQn},
    {TIM9, TIM1_BRK_TIM9_IRQn},
    {TIM10, TIM1_UP_TIM10_IRQn},
    {TIM11, TIM1_TRG_COM_TIM11_IRQn},
    {TIM12, TIM8_BRK_TIM12_IRQn},
    {TIM13, TIM8_UP_TIM13_IRQn},
    {TIM14, TIM8_TRG_COM_TIM14_IRQn},
};

static cb_t timer_cb[TIMER_MAX];

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

void timer_init(timer_t timer, uint32_t time, uint32_t irqp, cb_t func)
{
    uint16_t period = 0;
    uint16_t prescaler = 0;
    TIM_HandleTypeDef htim = {0};

    switch (timer)
    {
        case TIMER1 : __HAL_RCC_TIM1_CLK_ENABLE(); break;
        case TIMER2 : __HAL_RCC_TIM2_CLK_ENABLE(); break;
        case TIMER3 : __HAL_RCC_TIM3_CLK_ENABLE(); break;
        case TIMER4 : __HAL_RCC_TIM4_CLK_ENABLE(); break;
        case TIMER5 : __HAL_RCC_TIM5_CLK_ENABLE(); break;
        case TIMER6 : __HAL_RCC_TIM6_CLK_ENABLE(); break;
        case TIMER7 : __HAL_RCC_TIM7_CLK_ENABLE(); break;
        case TIMER8 : __HAL_RCC_TIM8_CLK_ENABLE(); break;
        case TIMER9 : __HAL_RCC_TIM9_CLK_ENABLE(); break;
        case TIMER10 : __HAL_RCC_TIM10_CLK_ENABLE(); break;
        case TIMER11 : __HAL_RCC_TIM11_CLK_ENABLE(); break;
        case TIMER12 : __HAL_RCC_TIM12_CLK_ENABLE(); break;
        case TIMER13 : __HAL_RCC_TIM13_CLK_ENABLE(); break;
        case TIMER14 : __HAL_RCC_TIM14_CLK_ENABLE(); break;
        default : return; break;
    }

    time_factorization(time, SystemCoreClock, &period, &prescaler);
    htim.Instance = timer_table[timer].tim;
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
    HAL_NVIC_SetPriority(timer_table[timer].irq, irqp, 0);
	HAL_NVIC_EnableIRQ(timer_table[timer].irq);
    timer_cb[timer] = func; 
}

void timer_cmd(timer_t timer, bool en)
{
    if (en)
    {
        SET_BIT(timer_table[timer].tim->CR1, TIM_CR1_CEN);
    }
    else
    {
        CLEAR_BIT(timer_table[timer].tim->CR1, TIM_CR1_CEN);
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
    TIM_IRQHANDLER(TIM10, 9);
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
    TIM_IRQHANDLER(TIM13, 12);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM9, 8);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM11, 10);
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM12, 11);
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    TIM_IRQHANDLER(TIM14, 13);
}