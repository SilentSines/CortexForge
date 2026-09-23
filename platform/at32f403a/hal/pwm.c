#include "hal.h"

typedef struct
{
    gpio_pin_t pin;
    uint32_t remap;
    tmr_type *tmr;
    uint32_t channel;
    bool invert;
} pwm_type_t;

static const pwm_type_t pwm_table[] =
{
    /****************Positive****************/
    {GPIO_PA8, 0,              TMR1, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PE9, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PA9, 0,               TMR1, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PE11, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PA10, 0,              TMR1, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PE13, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PA11, 0,              TMR1, TMR_SELECT_CHANNEL_4, false},
    {GPIO_PE14, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_4, false},
    /***************Inverting***************/
    {GPIO_PB13, 0,             TMR1, TMR_SELECT_CHANNEL_1, true},
    {GPIO_PA7, TMR1_GMUX_0001, TMR1, TMR_SELECT_CHANNEL_1, true},
    {GPIO_PE8, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_1, true},
    {GPIO_PB14, 0,              TMR1, TMR_SELECT_CHANNEL_2, true},
    {GPIO_PB0, TMR1_GMUX_0001,  TMR1, TMR_SELECT_CHANNEL_2, true},
    {GPIO_PE10, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_2, true},
    {GPIO_PB15, 0,              TMR1, TMR_SELECT_CHANNEL_3, true},
    {GPIO_PB1, TMR1_GMUX_0001,  TMR1, TMR_SELECT_CHANNEL_3, true},
    {GPIO_PE12, TMR1_GMUX_0011, TMR1, TMR_SELECT_CHANNEL_3, true},

    /****************Positive****************/
    {GPIO_PA0, 0,             TMR2, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PA15, TMR2_GMUX_01, TMR2, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PA1, 0,            TMR2, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PB3, TMR2_GMUX_01, TMR2, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PA2, 0,             TMR2, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PB10, TMR2_GMUX_10, TMR2, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PA3, 0,             TMR2, TMR_SELECT_CHANNEL_4, false},
    {GPIO_PB11, TMR2_GMUX_10, TMR2, TMR_SELECT_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PA6, 0,              TMR3, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PB4, TMR3_GMUX_0010, TMR3, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PC6, TMR3_GMUX_0011, TMR3, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PA7, 0,              TMR3, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PB5, TMR3_GMUX_0010, TMR3, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PC7, TMR3_GMUX_0011, TMR3, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PB0, 0,              TMR3, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PC8, TMR3_GMUX_0011, TMR3, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PB1, 0,              TMR3, TMR_SELECT_CHANNEL_4, false},
    {GPIO_PC9, TMR3_GMUX_0011, TMR3, TMR_SELECT_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PB6,  0,              TMR4, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PD12, TMR4_GMUX_0001, TMR4, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PB7,  0,              TMR4, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PD13, TMR4_GMUX_0001, TMR4, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PB8,  0,              TMR4, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PD14, TMR4_GMUX_0001, TMR4, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PB9,  0,              TMR4, TMR_SELECT_CHANNEL_4, false},
    {GPIO_PD15, TMR4_GMUX_0001, TMR4, TMR_SELECT_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PA0, 0, TMR5, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PA1, 0, TMR5, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PA2, 0, TMR5, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PA3, 0, TMR5, TMR_SELECT_CHANNEL_4, false},

    /****************Positive****************/
    {GPIO_PC6, 0, TMR8, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PC7, 0, TMR8, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PC8, 0, TMR8, TMR_SELECT_CHANNEL_3, false},
    {GPIO_PC9, 0, TMR8, TMR_SELECT_CHANNEL_4, false},
    /***************Inverting***************/
    {GPIO_PA7, 0, TMR8, TMR_SELECT_CHANNEL_1, true},
    {GPIO_PB0, 0, TMR8, TMR_SELECT_CHANNEL_2, true},
    {GPIO_PB1, 0, TMR8, TMR_SELECT_CHANNEL_3, true},

    /****************Positive****************/
    {GPIO_PA2, 0,         TMR9, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PE5, TMR9_GMUX, TMR9, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PA3, 0,         TMR9, TMR_SELECT_CHANNEL_2, false},
    {GPIO_PE6, TMR9_GMUX, TMR9, TMR_SELECT_CHANNEL_2, false},

    /****************Positive****************/
    {GPIO_PB8, 0, TMR10, TMR_SELECT_CHANNEL_1, false},

    /****************Positive****************/
    {GPIO_PB9, 0, TMR11, TMR_SELECT_CHANNEL_1, false},

    /****************Positive****************/
    {GPIO_PB14, 0, TMR12, TMR_SELECT_CHANNEL_1, false},
    {GPIO_PB15, 0, TMR12, TMR_SELECT_CHANNEL_2, false},

    /****************Positive****************/
    {GPIO_PA6, 0, TMR13, TMR_SELECT_CHANNEL_1, false},

    /****************Positive****************/
    {GPIO_PA7, 0, TMR14, TMR_SELECT_CHANNEL_1, false},
};

void pwm_init(pwm_pin_t pin, uint32_t res, uint32_t freq, pwm_polarity_t polarity)
{
    uint32_t arr, psc;
    gpio_init_type gpio_cfg = {0};
    tmr_output_config_type oc_config = {0};
    const pwm_type_t *pwm = &pwm_table[pin];

    if (res == 0 || freq == 0 || (res * freq) > SystemCoreClock)
    {
        return;
    }
    
    if (pin <= TMR1_PWM_CH3N_PE12)
    {
        crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR2_PWM_CH4_PB11)
    {
        crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR3_PWM_CH4_PC9)
    {
        crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR4_PWM_CH4_PD15)
    {
        crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR5_PWM_CH4_PA3)
    {
        crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR8_PWM_CH3N_PB1)
    {
        crm_periph_clock_enable(CRM_TMR8_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR9_PWM_CH2_PE6)
    {
        crm_periph_clock_enable(CRM_TMR9_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR10_PWM_CH1_PB8)
    {
        crm_periph_clock_enable(CRM_TMR10_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR11_PWM_CH1_PB9)
    {
        crm_periph_clock_enable(CRM_TMR11_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR12_PWM_CH2_PB15)
    {
        crm_periph_clock_enable(CRM_TMR12_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR13_PWM_CH1_PA6)
    {
        crm_periph_clock_enable(CRM_TMR13_PERIPH_CLOCK, TRUE);
    }
    else if (pin <= TMR14_PWM_CH1_PA7)
    {
        crm_periph_clock_enable(CRM_TMR14_PERIPH_CLOCK, TRUE);
    }
    else
    {
        return;
    }
    
    gpio_pin_init(pwm->pin, GPIO_AF_PP, GPIO_AF_TMR);
    if (pwm->remap)
    {
        gpio_pin_remap_config(pwm->remap, TRUE);
    }

    arr = res;
    psc = SystemCoreClock / res / freq;
    tmr_base_init(pwm->tmr, arr - 1, psc - 1);
    tmr_period_buffer_enable(pwm->tmr, FALSE);
    tmr_cnt_dir_set(pwm->tmr, TMR_COUNT_UP);
    tmr_clock_source_div_set(pwm->tmr, TMR_CLOCK_DIV1);
    if (pin <= TMR8_PWM_CH3N_PB1)
    {
        tmr_primary_mode_select(pwm->tmr, TMR_PRIMARY_SEL_RESET);
    }
    if (pin <= TMR9_PWM_CH2_PE6 || (pwm->tmr == TMR12))
    {
        tmr_sub_sync_mode_set(pwm->tmr, FALSE);
    }
    if (pwm->tmr == TMR1 || pwm->tmr == TMR8)
    {
        tmr_repetition_counter_set(pwm->tmr, 0);
        tmr_output_enable(pwm->tmr, TRUE);
    }

    oc_config.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    if (polarity == PWM_POLARITY_HIGH) 
    {
        oc_config.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
        oc_config.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    }
    else
    {
        oc_config.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
        oc_config.occ_polarity = TMR_OUTPUT_ACTIVE_LOW;
    }
    oc_config.oc_idle_state = FALSE;
    oc_config.occ_idle_state = FALSE;
    if (pwm->invert)
    {
        oc_config.oc_output_state = FALSE;
        oc_config.occ_output_state = TRUE;
    }
    else
    {
        oc_config.oc_output_state = TRUE;
        oc_config.occ_output_state = FALSE;
    }
    tmr_output_channel_config(pwm->tmr, pwm->channel, &oc_config);
    tmr_channel_value_set(pwm->tmr, pwm->channel, 0);
    tmr_output_channel_buffer_enable(pwm->tmr, pwm->channel, FALSE);
    tmr_output_channel_immediately_set(pwm->tmr, pwm->channel, FALSE);
    tmr_channel_enable(pwm->tmr, pwm->channel + pwm->invert, FALSE);
}

void pwm_set_freq(pwm_pin_t pin, uint32_t freq)
{
    uint32_t arr, psc;
    tmr_type *tmr;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    tmr = pwm_table[pin].tmr;
    arr = tmr_period_value_get(tmr) + 1;
    if (arr == 0 || freq == 0 || (arr * freq) > SystemCoreClock)
    {
        return;
    }
    psc = SystemCoreClock / arr / freq;
    tmr_div_value_set(tmr, psc - 1);
}

void pwm_set_duty(pwm_pin_t pin, uint32_t duty)
{
    uint32_t arr;
    const pwm_type_t *pwm;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }
    
    pwm = &pwm_table[pin];
    arr = tmr_period_value_get(pwm->tmr);
    tmr_channel_value_set(pwm->tmr, pwm->channel, (duty > arr) ? arr : duty);
}

void pwm_start(pwm_pin_t pin)
{
    const pwm_type_t *pwm;
    tmr_type *tmr;
    tmr_channel_select_type tmr_channel;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    pwm = &pwm_table[pin];
    tmr = pwm->tmr;

    tmr_channel = (tmr_channel_select_type)(pwm->channel + pwm->invert);
    tmr_channel_enable(tmr, tmr_channel, TRUE);

    tmr_counter_enable(pwm->tmr, TRUE);
}

void pwm_stop(pwm_pin_t pin)
{
    const pwm_type_t *pwm;
    tmr_type *tmr;
    tmr_channel_select_type tmr_channel;

    if (pin >= PWM_PIN_MAX)
    {
        return;
    }

    pwm = &pwm_table[pin];
    tmr = pwm->tmr;

    tmr_channel = (tmr_channel_select_type)(pwm->channel + pwm->invert);
    tmr_channel_enable(tmr, tmr_channel, FALSE);

    if ((tmr->cctrl & 0x1555) == 0)
    {
       tmr_counter_enable(pwm->tmr, FALSE);
    }
}  