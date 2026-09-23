#include "bsp.h"

#define BAT_AD_FILTER 10
#define BAT_STATE_FILTER 3

static struct 
{
    uint32_t sum;
    uint16_t min;
    uint16_t max;
    uint16_t cnt;
    uint16_t val;
} bat_ad;

static struct 
{
    bat_state_t cur;
    bat_state_t trans;
    uint16_t cnt;
} bat_state;

void bat_init(void)
{
    gpio_pin_init(BAT_EN_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);

#if BAT_CHARGE_DETECT_EN
    gpio_pin_init(BAT_CHG_PIN, GPIO_IN_PULLUP, GPIO_DRIVE_STRENGTH_MODERATE);
    gpio_pin_init(BAT_STB_PIN, GPIO_IN_PULLUP, GPIO_DRIVE_STRENGTH_MODERATE);
#endif

    adc_setup_channel(BAT_AD_CH);

    bat_ad.val = BAT_AD_INVALID;
    bat_ad.min = 0xFFFF;

    bat_state.cur = BAT_STATE_DISCHRG;
    bat_state.trans = BAT_STATE_MAX;
}

void bat_monitor(void)
{
    uint16_t temp;

    temp = adc_sample_one(BAT_ADC, BAT_AD_CH, ADC_SAMPLETIME_239_5);
    if (temp != 0xFFFF)
    {
        if (temp > bat_ad.max)
        {
            bat_ad.max = temp;
        }
        if (temp < bat_ad.min)
        {
            bat_ad.min = temp;
        }
        bat_ad.sum += temp;
        if (++bat_ad.cnt >= BAT_AD_FILTER)
        {
            bat_ad.sum -= (bat_ad.max + bat_ad.min);
            bat_ad.val = bat_ad.sum / (BAT_AD_FILTER - 2);

            bat_ad.sum = 0;
            bat_ad.max = 0;
            bat_ad.min = 0xFFFF;
            bat_ad.cnt = 0;
        }
    }

#if BAT_CHARGE_DETECT_EN
    bat_state_t state;

    bool chr = GPIO_READ(BAT_CHG_PIN);
    bool stb = GPIO_READ(BAT_STB_PIN);
    if (chr && stb)
    {
        state = BAT_STATE_DISCHRG;
    }
    if (!chr && stb)
    {
        state = BAT_STATE_CHRGING;
    }
    if (chr && !stb)
    {
        state = BAT_STATE_FULL;
    }
    if (state != bat_state.cur)
    {
        if (state == bat_state.trans)
        {
            if (++bat_state.cnt >= BAT_STATE_FILTER)
            {
                bat_state.cur = state;
                bat_state.trans = BAT_STATE_MAX;
                bat_state.cnt = 0;
            }
        }
        else
        {
            bat_state.trans = state;
            bat_state.cnt = 0;
        }
    }
#endif
}

uint16_t bat_get_ad(void)
{
    uint16_t vlaue = bat_ad.val;

    bat_ad.val = BAT_AD_INVALID;
    return vlaue;
}

bat_state_t bat_get_state(void)
{
#if BAT_CHARGE_DETECT_EN
    return bat_state.cur;
#else
    return BAT_STATE_DISCHRG;
#endif
}