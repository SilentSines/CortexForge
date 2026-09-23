#include "hal.h"

typedef struct
{
    tmr_type *tmr;
    crm_periph_clock_type clk;
    IRQn_Type irq;         
} timer_type_t;

static const timer_type_t timer_table[] =
{
    {TMR1, CRM_TMR1_PERIPH_CLOCK, TMR1_OVF_TMR10_IRQn},
    {TMR2, CRM_TMR2_PERIPH_CLOCK, TMR2_GLOBAL_IRQn},
    {TMR3, CRM_TMR3_PERIPH_CLOCK, TMR3_GLOBAL_IRQn},
    {TMR4, CRM_TMR4_PERIPH_CLOCK, TMR4_GLOBAL_IRQn},
    {TMR5, CRM_TMR5_PERIPH_CLOCK, TMR5_GLOBAL_IRQn},
    {TMR6, CRM_TMR6_PERIPH_CLOCK, TMR6_GLOBAL_IRQn},
    {TMR7, CRM_TMR7_PERIPH_CLOCK, TMR7_GLOBAL_IRQn},
    {TMR8, CRM_TMR8_PERIPH_CLOCK, TMR8_OVF_TMR13_IRQn},
    {TMR9, CRM_TMR9_PERIPH_CLOCK, TMR1_BRK_TMR9_IRQn},
    {TMR10, CRM_TMR10_PERIPH_CLOCK, TMR1_OVF_TMR10_IRQn},
    {TMR11, CRM_TMR11_PERIPH_CLOCK, TMR1_TRG_HALL_TMR11_IRQn},
    {TMR12, CRM_TMR12_PERIPH_CLOCK, TMR8_BRK_TMR12_IRQn},
    {TMR13, CRM_TMR13_PERIPH_CLOCK, TMR8_OVF_TMR13_IRQn},
    {TMR14, CRM_TMR14_PERIPH_CLOCK, TMR8_TRG_HALL_TMR14_IRQn},
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
    const timer_type_t *tmr_cfg;

    if (timer >= TIMER_MAX)
    {
        return;
    }
    
    tmr_cfg = &timer_table[timer];

    crm_periph_clock_enable(tmr_cfg->clk, TRUE);

    time_factorization(time, SystemCoreClock, &period, &prescaler);
    tmr_base_init(tmr_cfg->tmr, period - 1, prescaler - 1);
    tmr_period_buffer_enable(tmr_cfg->tmr, FALSE);
    if (timer <= TIMER8)
    {
        tmr_primary_mode_select(tmr_cfg->tmr, TMR_PRIMARY_SEL_RESET);
    }
    if (timer <= TIMER5 || timer >= TIMER8)
    {
        tmr_cnt_dir_set(tmr_cfg->tmr, TMR_COUNT_UP);
        tmr_clock_source_div_set(tmr_cfg->tmr, TMR_CLOCK_DIV1);
    }
    if (timer <= TIMER5 || timer == TIMER8 || timer == TIMER9 || timer == TIMER12)
    {
        tmr_sub_sync_mode_set(tmr_cfg->tmr, FALSE);
    }
    if (timer == TIMER1 || timer == TIMER8)
    {
        tmr_repetition_counter_set(tmr_cfg->tmr, 0);
    }
    tmr_overflow_request_source_set(tmr_cfg->tmr, TRUE);
    tmr_interrupt_enable(tmr_cfg->tmr, TMR_OVF_INT, TRUE);
    nvic_irq_enable(tmr_cfg->irq, irqp, 0);
    timer_cb[timer] = func; 
}

void timer_cmd(timer_t timer, bool en)
{
    if (timer >= TIMER_MAX)
    {
        return;
    }

    tmr_counter_enable(timer_table[timer].tmr, en ? TRUE : FALSE);
}

#define TMR_IRQHANDLER(x, y)                              \
    if (tmr_interrupt_flag_get(x, TMR_OVF_FLAG) != RESET) \
    {                                                     \
        tmr_flag_clear(x, TMR_OVF_FLAG);                  \
        if (timer_cb[y] != NULL)                          \
        {                                                 \
            timer_cb[y]();                                \
        }                                                 \
    }

void TMR1_OVF_TMR10_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR1, 0);
    TMR_IRQHANDLER(TMR10, 9);
}

void TMR2_GLOBAL_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR2, 1);
}

void TMR3_GLOBAL_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR3, 2);
}

void TMR4_GLOBAL_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR4, 3);
}

void TMR5_GLOBAL_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR5, 4);
}

void TMR6_GLOBAL_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR6, 5);
}

void TMR7_GLOBAL_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR7, 6);
}

void TMR8_OVF_TMR13_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR8, 7);
    TMR_IRQHANDLER(TMR13, 12);
}

void TMR1_BRK_TMR9_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR9, 8);
}

void TMR1_TRG_HALL_TMR11_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR11, 10);
}

void TMR8_BRK_TMR12_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR12, 11);
}

void TMR8_TRG_HALL_TMR14_IRQHandler(void)
{
    TMR_IRQHANDLER(TMR14, 13);
}