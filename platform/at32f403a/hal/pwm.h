#ifndef PWM_H
#define PWM_H

typedef enum
{
    /****Positive****/
    TMR1_PWM_CH1_PA8,
    TMR1_PWM_CH1_PE9,
    TMR1_PWM_CH2_PA9,
    TMR1_PWM_CH2_PE11,
    TMR1_PWM_CH3_PA10,
    TMR1_PWM_CH3_PE13,
    TMR1_PWM_CH4_PA11,
    TMR1_PWM_CH4_PE14,
    /***Inverting***/
    TMR1_PWM_CH1N_PB13,
    TMR1_PWM_CH1N_PA7,
    TMR1_PWM_CH1N_PE8,
    TMR1_PWM_CH2N_PB14,
    TMR1_PWM_CH2N_PB0,
    TMR1_PWM_CH2N_PE10,
    TMR1_PWM_CH3N_PB15,
    TMR1_PWM_CH3N_PB1,
    TMR1_PWM_CH3N_PE12,

    /****Positive****/
    TMR2_PWM_CH1_PA0,
    TMR2_PWM_CH1_PA15,
    TMR2_PWM_CH2_PA1,
    TMR2_PWM_CH2_PB3,
    TMR2_PWM_CH3_PA2,
    TMR2_PWM_CH3_PB10,
    TMR2_PWM_CH4_PA3,
    TMR2_PWM_CH4_PB11,

    /****Positive****/
    TMR3_PWM_CH1_PA6,
    TMR3_PWM_CH1_PB4,
    TMR3_PWM_CH1_PC6,
    TMR3_PWM_CH2_PA7,
    TMR3_PWM_CH2_PB5,
    TMR3_PWM_CH2_PC7,
    TMR3_PWM_CH3_PB0,
    TMR3_PWM_CH3_PC8,
    TMR3_PWM_CH4_PB1,
    TMR3_PWM_CH4_PC9,

    /****Positive****/
    TMR4_PWM_CH1_PB6,
    TMR4_PWM_CH1_PD12,
    TMR4_PWM_CH2_PB7,
    TMR4_PWM_CH2_PD13,
    TMR4_PWM_CH3_PB8,
    TMR4_PWM_CH3_PD14,
    TMR4_PWM_CH4_PB9,
    TMR4_PWM_CH4_PD15,

    /****Positive****/
    TMR5_PWM_CH1_PA0,
    TMR5_PWM_CH2_PA1,
    TMR5_PWM_CH3_PA2,
    TMR5_PWM_CH4_PA3,

    /****Positive****/
    TMR8_PWM_CH1_PC6,
    TMR8_PWM_CH2_PC7,
    TMR8_PWM_CH3_PC8,
    TMR8_PWM_CH4_PC9,
    /***Inverting***/
    TMR8_PWM_CH1N_PA7,
    TMR8_PWM_CH2N_PB0,
    TMR8_PWM_CH3N_PB1,

    /****Positive****/
    TMR9_PWM_CH1_PA2,
    TMR9_PWM_CH1_PE5,
    TMR9_PWM_CH2_PA3,
    TMR9_PWM_CH2_PE6,

    /****Positive****/
    TMR10_PWM_CH1_PB8,

    /****Positive****/
    TMR11_PWM_CH1_PB9,

    /****Positive****/
    TMR12_PWM_CH1_PB14,
    TMR12_PWM_CH2_PB15,

    /****Positive****/
    TMR13_PWM_CH1_PA6,

    /****Positive****/
    TMR14_PWM_CH1_PA7,

    PWM_PIN_MAX,
} pwm_pin_t;

typedef enum
{
    PWM_POLARITY_HIGH = 0,
    PWM_POLARITY_LOW = 1,
} pwm_polarity_t;

void pwm_init(pwm_pin_t pin, uint32_t res, uint32_t freq, pwm_polarity_t polarity);
void pwm_set_freq(pwm_pin_t pin, uint32_t freq);
void pwm_set_duty(pwm_pin_t pin, uint32_t duty);
void pwm_start(pwm_pin_t pin);
void pwm_stop(pwm_pin_t pin);

#endif
