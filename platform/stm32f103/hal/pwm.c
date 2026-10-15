#include "bsp.h"

typedef struct
{
    gpio_pin_t pin;
    uint8_t remap;
    TIM_TypeDef *tim;
    uint32_t channel;
    bool invert;
} pwm_map_t;

static const pwm_map_t pwm_map_table[] =
{
    /****************Positive****************/
    {GPIO_PA8,  0, TIM1, TIM_CHANNEL_1, false},
    {GPIO_PE9,  1, TIM1, TIM_CHANNEL_1, false},
    {GPIO_PA9,  0, TIM1, TIM_CHANNEL_2, false},
    {GPIO_PE11, 1, TIM1, TIM_CHANNEL_2, false},
    {GPIO_PA10, 0, TIM1, TIM_CHANNEL_3, false},
    {GPIO_PE13, 1, TIM1, TIM_CHANNEL_3, false},
    {GPIO_PA11, 0, TIM1, TIM_CHANNEL_4, false},
    {GPIO_PE14, 1, TIM1, TIM_CHANNEL_4, false},
    /***************Inverting***************/
    {GPIO_PB13, 0, TIM1, TIM_CHANNEL_1, true},
    {GPIO_PA7,  2, TIM1, TIM_CHANNEL_1, true},
    {GPIO_PE8,  1, TIM1, TIM_CHANNEL_1, true},
    {GPIO_PB14, 0, TIM1, TIM_CHANNEL_2, true},
    {GPIO_PB0,  2, TIM1, TIM_CHANNEL_2, true},
    {GPIO_PE10, 1, TIM1, TIM_CHANNEL_2, true},
    {GPIO_PB15, 0, TIM1, TIM_CHANNEL_3, true},
    {GPIO_PB1,  2, TIM1, TIM_CHANNEL_3, true},
    {GPIO_PE12, 1, TIM1, TIM_CHANNEL_3, true},

    /****************Positive****************/
    {GPIO_PA0,  0, TIM2, TIM_CHANNEL_1, false},
    {GPIO_PA15, 1, TIM2, TIM_CHANNEL_1, false},
    {GPIO_PA1,  0, TIM2, TIM_CHANNEL_2, false},
    {GPIO_PB3,  1, TIM2, TIM_CHANNEL_2, false},
    {GPIO_PA2,  0, TIM2, TIM_CHANNEL_3, false},
    {GPIO_PB10, 1, TIM2, TIM_CHANNEL_3, false},
    {GPIO_PA3,  0, TIM2, TIM_CHANNEL_4, false},
    {GPIO_PB11, 1, TIM2, TIM_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PA6, 0, TIM3, TIM_CHANNEL_1, false},
    {GPIO_PB4, 2, TIM3, TIM_CHANNEL_1, false},
    {GPIO_PC6, 1, TIM3, TIM_CHANNEL_1, false},
    {GPIO_PA7, 0, TIM3, TIM_CHANNEL_2, false},
    {GPIO_PB5, 2, TIM3, TIM_CHANNEL_2, false},
    {GPIO_PC7, 1, TIM3, TIM_CHANNEL_2, false},
    {GPIO_PB0, 0, TIM3, TIM_CHANNEL_3, false},
    {GPIO_PC8, 1, TIM3, TIM_CHANNEL_3, false},
    {GPIO_PB1, 0, TIM3, TIM_CHANNEL_4, false},
    {GPIO_PC9, 1, TIM3, TIM_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PB6,  0, TIM4, TIM_CHANNEL_1, false},
    {GPIO_PD12, 1, TIM4, TIM_CHANNEL_1, false},
    {GPIO_PB7,  0, TIM4, TIM_CHANNEL_2, false},
    {GPIO_PD13, 1, TIM4, TIM_CHANNEL_2, false},
    {GPIO_PB8,  0, TIM4, TIM_CHANNEL_3, false},
    {GPIO_PD14, 1, TIM4, TIM_CHANNEL_3, false},
    {GPIO_PB9,  0, TIM4, TIM_CHANNEL_4, false},
    {GPIO_PD15, 1, TIM4, TIM_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PA0, 0, TIM5, TIM_CHANNEL_1, false},
    {GPIO_PA1, 0, TIM5, TIM_CHANNEL_2, false},
    {GPIO_PA2, 0, TIM5, TIM_CHANNEL_3, false},
    {GPIO_PA3, 0, TIM5, TIM_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PC6, 0, TIM8, TIM_CHANNEL_1, false},
    {GPIO_PC7, 0, TIM8, TIM_CHANNEL_2, false},
    {GPIO_PC8, 0, TIM8, TIM_CHANNEL_3, false},
    {GPIO_PC9, 0, TIM8, TIM_CHANNEL_4, false},
    /***************Inverting***************/
    {GPIO_PA7, 0, TIM8, TIM_CHANNEL_1, true},
    {GPIO_PB0, 0, TIM8, TIM_CHANNEL_2, true},
    {GPIO_PB1, 0, TIM8, TIM_CHANNEL_3, true},
};

void pwm_init(pwm_pin_t pin, uint32_t res, uint32_t freq, pwm_polarity_t polarity)
{
    uint32_t arr, psc;
    TIM_HandleTypeDef htim = {0};
    TIM_OC_InitTypeDef oc_config = {0};

    if (res == 0 || freq == 0 || (res * freq) > SystemCoreClock)
    {
        return;
    }
    
    if (pin >= PWM_PIN_MAX)
    {
        return;
    }
    
    const pwm_map_t *map = &pwm_map_table[pin];
    if (pin <= TMR1_PWM_CH3N_PE12)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();
        if (map->remap == 1)
        {
            __HAL_AFIO_REMAP_TIM1_ENABLE();
        }
        else if (map->remap == 2)
        {
            __HAL_AFIO_REMAP_TIM1_PARTIAL();
        }
    }
    else if (pin <= TMR2_PWM_CH4_PB11)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
        if (map->remap)
        {
            __HAL_AFIO_REMAP_TIM2_ENABLE();
        }
    }
    else if (pin <= TMR3_PWM_CH4_PC9)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        if (map->remap == 1)
        {
            __HAL_AFIO_REMAP_TIM3_ENABLE();
        }
        else if (map->remap == 2)
        {
            __HAL_AFIO_REMAP_TIM3_PARTIAL();
        }
    }
    else if (pin <= TMR4_PWM_CH4_PD15)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        if (map->remap)
        {
            __HAL_AFIO_REMAP_TIM4_ENABLE();
        }
    }
    else if (pin <= TMR5_PWM_CH4_PA3)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
    else if (pin <= TMR8_PWM_CH3N_PB1)
    {
        __HAL_RCC_TIM8_CLK_ENABLE();
    }
    else
    {
        return;
    }
    
    gpio_pin_init(map->pin, GPIO_AF_PP, 0);

    arr = res;
    psc = SystemCoreClock / res / freq;
    htim.Instance = map->tim;
    htim.Init.Prescaler = psc - 1;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = arr - 1;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim) != HAL_OK)
    {
        hal_error_handler();
    }

    oc_config.OCMode = TIM_OCMODE_PWM1;
    oc_config.Pulse = 0;
    if (polarity == PWM_POLARITY_HIGH) 
    {
        oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
        oc_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    } 
    else 
    {
        oc_config.OCPolarity = TIM_OCPOLARITY_LOW;
        oc_config.OCNPolarity = TIM_OCNPOLARITY_LOW;
    }
    oc_config.OCFastMode = TIM_OCFAST_DISABLE;
    oc_config.OCIdleState = TIM_OCIDLESTATE_RESET;
    oc_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim, &oc_config, map->channel) != HAL_OK)
    {
        hal_error_handler();
    }
}

void pwm_set_freq(pwm_pin_t pin, uint32_t freq)
{
    uint32_t arr, psc;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    TIM_TypeDef *tim = pwm_map_table[pin].tim;
    arr = tim->ARR + 1;
    if (arr == 0 || freq == 0 || (arr * freq) > SystemCoreClock)
    {
        return;
    }
    psc = SystemCoreClock / arr / freq;
    tim->PSC = psc - 1;
}

void pwm_set_duty(pwm_pin_t pin, uint32_t duty)
{
    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    TIM_TypeDef *tim = pwm_map_table[pin].tim;
    
    if (duty > tim->ARR) 
    {
        duty = tim->ARR;
    }
    switch (pwm_map_table[pin].channel)
    {
        case TIM_CHANNEL_1 : tim->CCR1 = duty; break;
        case TIM_CHANNEL_2 : tim->CCR2 = duty; break;
        case TIM_CHANNEL_3 : tim->CCR3 = duty; break;
        case TIM_CHANNEL_4 : tim->CCR4 = duty; break;
    }
}

void pwm_start(pwm_pin_t pin)
{
    uint32_t ccx_bit;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    const pwm_map_t *map = &pwm_map_table[pin];
    TIM_TypeDef *tim = map->tim;

    switch (map->channel) 
    {
        case TIM_CHANNEL_1: ccx_bit = map->invert ? TIM_CCER_CC1NE : TIM_CCER_CC1E;  break;
        case TIM_CHANNEL_2: ccx_bit = map->invert ? TIM_CCER_CC2NE : TIM_CCER_CC2E;  break;
        case TIM_CHANNEL_3: ccx_bit = map->invert ? TIM_CCER_CC3NE : TIM_CCER_CC3E;  break;
        case TIM_CHANNEL_4: ccx_bit = map->invert ? 0 : TIM_CCER_CC4E;  break;
        default: return;
    }

    tim->CCER |= ccx_bit;
    if (tim == TIM1 || tim == TIM8) 
    {
        tim->BDTR |= TIM_BDTR_MOE;
    }
    tim->CR1 |= TIM_CR1_CEN;
}

void pwm_stop(pwm_pin_t pin)
{
    uint32_t ccx_bit;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    const pwm_map_t *map = &pwm_map_table[pin];
    TIM_TypeDef *tim = map->tim;

    switch (map->channel) 
    {
        case TIM_CHANNEL_1: ccx_bit = map->invert ? TIM_CCER_CC1NE : TIM_CCER_CC1E;  break;
        case TIM_CHANNEL_2: ccx_bit = map->invert ? TIM_CCER_CC2NE : TIM_CCER_CC2E;  break;
        case TIM_CHANNEL_3: ccx_bit = map->invert ? TIM_CCER_CC3NE : TIM_CCER_CC3E;  break;
        case TIM_CHANNEL_4: ccx_bit = map->invert ? 0 : TIM_CCER_CC4E;  break;
        default: return;
    }

    tim->CCER &= ~ccx_bit;

    if ((tim->CCER & (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E |
                      TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE)) == 0) 
    {
        if (tim == TIM1 || tim == TIM8) 
        {
            tim->BDTR &= ~TIM_BDTR_MOE;
        }

        tim->CR1 &= ~TIM_CR1_CEN;
    }
}