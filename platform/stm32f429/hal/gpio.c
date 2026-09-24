#include "bsp.h"

const GPIO_TypeDef *gpio_port_table[] = 
{
#ifdef GPIOA
    GPIOA,
#endif

#ifdef GPIOB
    GPIOB,
#endif

#ifdef GPIOC
    GPIOC,
#endif

#ifdef GPIOD
    GPIOD,
#endif

#ifdef GPIOE
    GPIOE,
#endif

#ifdef GPIOF
    GPIOF,
#endif

#ifdef GPIOG
    GPIOG,
#endif

#ifdef GPIOH
    GPIOH,
#endif

#ifdef GPIOI
    GPIOI,
#endif
};

const uint16_t gpio_pin_table[16] =
{
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
};

static cb_t exti_cb[16];

void gpio_pin_init(gpio_pin_t pin, gpio_mode_t mode, uint32_t af)
{
    GPIO_InitTypeDef gpio_cfg = {0};
    GPIO_TypeDef *gpio_port;
    uint32_t gpio_pin;

    if (pin >= GPIO_MAX)
    {
        return;
    }

    gpio_port = (GPIO_TypeDef *)gpio_port_table[pin >> 4];
    gpio_pin = gpio_pin_table[pin & 0x0F];

    gpio_cfg.Pin = gpio_pin;
    gpio_cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_cfg.Alternate = af;
    gpio_cfg.Pull = GPIO_NOPULL;

    switch (mode)
    {
        case GPIO_IN_FLOATING : gpio_cfg.Mode = GPIO_MODE_INPUT;
                                break;

        case GPIO_IN_ANALOG :   gpio_cfg.Mode = GPIO_MODE_ANALOG;
                                break;

        case GPIO_IN_PULLUP :   gpio_cfg.Mode = GPIO_MODE_INPUT;
                                gpio_cfg.Pull = GPIO_PULLUP;
                                break;

        case GPIO_IN_PULLDOWN : gpio_cfg.Mode = GPIO_MODE_INPUT;
                                gpio_cfg.Pull = GPIO_PULLDOWN;
                                break;

        case GPIO_OUT_PP :      gpio_cfg.Mode = GPIO_MODE_OUTPUT_PP;
                                break;

        case GPIO_OUT_PP_HIGH : gpio_cfg.Mode = GPIO_MODE_OUTPUT_PP;
                                GPIO_OUT_H(pin);
                                break;

        case GPIO_OUT_PP_LOW :  gpio_cfg.Mode = GPIO_MODE_OUTPUT_PP;
                                GPIO_OUT_L(pin);
                                break;
        case GPIO_OUT_OD :      gpio_cfg.Mode = GPIO_MODE_OUTPUT_OD;
                                break;

        case GPIO_OUT_OD_HIGH : gpio_cfg.Mode = GPIO_MODE_OUTPUT_OD;
                                GPIO_OUT_H(pin);
                                break;

        case GPIO_OUT_OD_LOW :  gpio_cfg.Mode = GPIO_MODE_OUTPUT_OD;
                                GPIO_OUT_L(pin);
                                break;

        case GPIO_AF_PP :       gpio_cfg.Mode = GPIO_MODE_AF_PP;
                                break;

        case GPIO_AF_PP_PULLUP :    gpio_cfg.Mode = GPIO_MODE_AF_PP;
                                    gpio_cfg.Pull = GPIO_PULLUP;
                                    break;

        case GPIO_AF_PP_PULLDOWN :  gpio_cfg.Mode = GPIO_MODE_AF_PP;
                                    gpio_cfg.Pull = GPIO_PULLDOWN;
                                    break;

        case GPIO_AF_OD :       gpio_cfg.Mode = GPIO_MODE_AF_OD;
                                break;

        case GPIO_AF_OD_PULLUP :    gpio_cfg.Mode = GPIO_MODE_AF_OD;
                                    gpio_cfg.Pull = GPIO_PULLUP;
                                    break;

        case GPIO_AF_OD_PULLDOWN :  gpio_cfg.Mode = GPIO_MODE_AF_OD;
                                    gpio_cfg.Pull = GPIO_PULLDOWN;
                                    break;

        default : return;
    }

    HAL_GPIO_Init(gpio_port, &gpio_cfg);
}

void gpio_exti_init(gpio_pin_t pin, exti_edge_t edge, uint32_t irqp, cb_t func)
{
    GPIO_InitTypeDef gpio_cfg = {0};
    GPIO_TypeDef *gpio_port;
    uint32_t gpio_pin;
    uint32_t pin_idx;
    IRQn_Type irq;

    if (pin >= GPIO_MAX)
    {
        return;
    }

    pin_idx = pin & 0x0F;
    gpio_port = (GPIO_TypeDef *)gpio_port_table[pin >> 4];
    gpio_pin = gpio_pin_table[pin_idx];

    gpio_cfg.Pin = gpio_pin;
    gpio_cfg.Pull = GPIO_NOPULL;
    switch (edge)
    {
        case EXTI_RISING : gpio_cfg.Mode = GPIO_MODE_IT_RISING; break;
        case EXTI_FALLING : gpio_cfg.Mode = GPIO_MODE_IT_FALLING; break;
        case EXTI_BOTH : gpio_cfg.Mode = GPIO_MODE_IT_RISING_FALLING; break;
    }
    HAL_GPIO_Init(gpio_port, &gpio_cfg);

    exti_cb[pin_idx] = func;

    if (pin_idx <= 4) 
    {
        irq = (IRQn_Type)(EXTI0_IRQn + pin_idx);
    } 
    else if (pin_idx <= 9) 
    {
        irq = EXTI9_5_IRQn;
    } 
    else 
    {
        irq = EXTI15_10_IRQn;
    }
    HAL_NVIC_SetPriority(irq, irqp, 0);
    HAL_NVIC_EnableIRQ(irq);
}

#define EXTI_IRQHANDLER(x)                               \
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_##x) != RESET) \
    {                                                    \
        if (exti_cb[x])                                  \
        {                                                \
            exti_cb[x]();                                \
        }                                                \
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_##x);        \
    }

void EXTI0_IRQHandler(void)
{
    EXTI_IRQHANDLER(0);
}

void EXTI1_IRQHandler(void)
{
    EXTI_IRQHANDLER(1);
}

void EXTI2_IRQHandler(void)
{
    EXTI_IRQHANDLER(2);
}

void EXTI3_IRQHandler(void)
{
    EXTI_IRQHANDLER(3);
}

void EXTI4_IRQHandler(void)
{
    EXTI_IRQHANDLER(4);
}

void EXTI9_5_IRQHandler(void)
{
    EXTI_IRQHANDLER(5);
    EXTI_IRQHANDLER(6);
    EXTI_IRQHANDLER(7);
    EXTI_IRQHANDLER(8);
    EXTI_IRQHANDLER(9);
}

void EXTI15_10_IRQHandler(void)
{
    EXTI_IRQHANDLER(10);
    EXTI_IRQHANDLER(11);
    EXTI_IRQHANDLER(12);
    EXTI_IRQHANDLER(13);
    EXTI_IRQHANDLER(14);
    EXTI_IRQHANDLER(15);
}