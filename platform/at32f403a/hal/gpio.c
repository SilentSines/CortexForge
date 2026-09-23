#include "hal.h"

gpio_type *gpio_port_table[] = 
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
    GPIO_PINS_0,
    GPIO_PINS_1,
    GPIO_PINS_2,
    GPIO_PINS_3,
    GPIO_PINS_4,
    GPIO_PINS_5,
    GPIO_PINS_6,
    GPIO_PINS_7,
    GPIO_PINS_8,
    GPIO_PINS_9,
    GPIO_PINS_10,
    GPIO_PINS_11,
    GPIO_PINS_12,
    GPIO_PINS_13,
    GPIO_PINS_14,
    GPIO_PINS_15,
};

static cb_t exti_cb[16];

void gpio_pin_init(gpio_pin_t pin, gpio_mode_t mode, uint8_t af)
{
    gpio_init_type gpio_cfg = {0};
    gpio_type *gpio_port;
    uint32_t gpio_pin;

    if (pin >= GPIO_MAX)
    {
        return;
    }

    UNUSED(af);
    gpio_port = (gpio_type *)gpio_port_table[pin >> 4];
    gpio_pin = gpio_pin_table[pin & 0x0F];

    gpio_cfg.gpio_pins = gpio_pin;
    gpio_cfg.gpio_pull = GPIO_PULL_NONE;
    gpio_cfg.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_cfg.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;

    switch (mode)
    {
        case GPIO_IN_FLOATING : gpio_cfg.gpio_mode = GPIO_MODE_INPUT;
                                break;

        case GPIO_IN_ANALOG :   gpio_cfg.gpio_mode = GPIO_MODE_ANALOG;
                                break;

        case GPIO_IN_PULLUP :   gpio_cfg.gpio_mode = GPIO_MODE_INPUT;
                                gpio_cfg.gpio_pull = GPIO_PULL_UP;
                                break;

        case GPIO_IN_PULLDOWN : gpio_cfg.gpio_mode = GPIO_MODE_INPUT;
                                gpio_cfg.gpio_pull = GPIO_PULL_DOWN;
                                break;

        case GPIO_OUT_PP :      gpio_cfg.gpio_mode = GPIO_MODE_OUTPUT;
                                break;

        case GPIO_OUT_PP_HIGH : gpio_cfg.gpio_mode = GPIO_MODE_OUTPUT;
                                GPIO_OUT_H(pin);
                                break;

        case GPIO_OUT_PP_LOW :  gpio_cfg.gpio_mode = GPIO_MODE_OUTPUT;
                                GPIO_OUT_L(pin);
                                break;

        case GPIO_OUT_OD :      gpio_cfg.gpio_mode = GPIO_MODE_OUTPUT;
                                gpio_cfg.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
                                break;

        case GPIO_OUT_OD_HIGH : gpio_cfg.gpio_mode = GPIO_MODE_OUTPUT;
                                gpio_cfg.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
                                GPIO_OUT_H(pin);
                                break;

        case GPIO_OUT_OD_LOW :  gpio_cfg.gpio_mode = GPIO_MODE_OUTPUT;
                                gpio_cfg.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
                                GPIO_OUT_L(pin);
                                break;

        case GPIO_AF_PP :       gpio_cfg.gpio_mode = GPIO_MODE_MUX;
                                break;

        case GPIO_AF_PP_PULLUP :    gpio_cfg.gpio_mode = GPIO_MODE_MUX;
                                    gpio_cfg.gpio_pull = GPIO_PULL_UP;
                                    break;

        case GPIO_AF_PP_PULLDOWN :  gpio_cfg.gpio_mode = GPIO_MODE_MUX;
                                    gpio_cfg.gpio_pull = GPIO_PULL_DOWN;
                                    break;

        case GPIO_AF_OD :       gpio_cfg.gpio_mode = GPIO_MODE_MUX;
                                gpio_cfg.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
                                break;

        case GPIO_AF_OD_PULLUP :    gpio_cfg.gpio_mode = GPIO_MODE_MUX;
                                    gpio_cfg.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
                                    gpio_cfg.gpio_pull = GPIO_PULL_UP;
                                    break;

        case GPIO_AF_OD_PULLDOWN :  gpio_cfg.gpio_mode = GPIO_MODE_MUX;
                                    gpio_cfg.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
                                    gpio_cfg.gpio_pull = GPIO_PULL_DOWN;
                                    break;

        default : return;
    }

    gpio_init(gpio_port, &gpio_cfg);
}

void gpio_exti_init(gpio_pin_t pin, exti_edge_t edge, uint32_t irqp, cb_t func)
{
    exint_init_type exint_cfg = {0};
    gpio_port_source_type exti_port; 
    gpio_pins_source_type exti_pin;
    IRQn_Type irq;

    if (pin >= GPIO_MAX)
    {
        return;
    }

    exti_port = (gpio_port_source_type)(pin >> 4);
    exti_pin = (gpio_pins_source_type)(pin & 0x0F);
    
    gpio_exint_line_config(exti_port, exti_pin);
    switch (edge)
    {
        case EXTI_RISING : exint_cfg.line_polarity = EXINT_TRIGGER_RISING_EDGE; break;
        case EXTI_FALLING : exint_cfg.line_polarity = EXINT_TRIGGER_FALLING_EDGE; break;
        case EXTI_BOTH : exint_cfg.line_polarity = EXINT_TRIGGER_BOTH_EDGE; break;
    }
    exint_cfg.line_select = (1 << exti_pin);
    exint_cfg.line_enable = TRUE;
    exint_cfg.line_mode = EXINT_LINE_INTERRUPT;
    exint_init(&exint_cfg);

    exti_cb[exti_pin] = func;

    if (exti_pin <= GPIO_PINS_SOURCE4) 
    {
        irq = (IRQn_Type)(EXINT0_IRQn + exti_pin);
    } 
    else if (exti_pin <= GPIO_PINS_SOURCE9) 
    {
        irq = EXINT9_5_IRQn;
    } 
    else 
    {
        irq = EXINT15_10_IRQn;
    }
    nvic_irq_enable(irq, irqp, 0);
}

#define EXTI_IRQHANDLER(x)                                 \
    if (exint_interrupt_flag_get(EXINT_LINE_##x) != RESET) \
    {                                                      \
        if (exti_cb[x])                                    \
        {                                                  \
            exti_cb[x]();                                  \
        }                                                  \
        exint_flag_clear(EXINT_LINE_##x);                  \
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