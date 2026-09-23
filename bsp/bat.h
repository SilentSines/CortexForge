#ifndef BAT_H
#define BAT_H

#define BAT_EN_PIN      GPIO_PA1
#define BAT_EN()        GPIO_OUT_H(BAT_EN_PIN)
#define BAT_DIS()       GPIO_OUT_L(BAT_EN_PIN)

#define BAT_CHARGE_DETECT_EN 1

#define BAT_CHG_PIN     GPIO_PB1
#define BAT_STB_PIN     GPIO_PB2

#define BAT_ADC         ADC1
#define BAT_AD_CH       ADC123_CH0_PA0
#define BAT_AD_INVALID  0xFFFF

typedef enum 
{
    BAT_STATE_DISCHRG,
    BAT_STATE_CHRGING,     
    BAT_STATE_FULL,   
    BAT_STATE_MAX,         
} bat_state_t;

void bat_init(void);
void bat_monitor(void);
uint16_t bat_get_ad(void);
bat_state_t bat_get_state(void);

#endif