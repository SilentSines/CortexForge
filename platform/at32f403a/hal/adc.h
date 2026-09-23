#ifndef ADC_H
#define ADC_H

typedef enum
{
    ADC123_CH0_PA0,
    ADC123_CH1_PA1,
    ADC123_CH2_PA2,
    ADC123_CH3_PA3,
    ADC12_CH4_PA4,
    ADC12_CH5_PA5,
    ADC12_CH6_PA6,
    ADC12_CH7_PA7,
    ADC12_CH8_PB0,
    ADC12_CH9_PB1,
    ADC123_CH10_PC0,
    ADC123_CH11_PC1,
    ADC123_CH12_PC2,
    ADC123_CH13_PC3,
    ADC12_CH14_PC4,
    ADC12_CH15_PC5,
    ADC_CH_MAX,
} adc_ch_t;

void adc_init(adc_type *adc, bool scan_en);
void adc_setup_channel(adc_ch_t ch);
uint16_t adc_sample_one(adc_type *adc, adc_ch_t ch, adc_sampletime_select_type sample_time);
void adc_dma_config(adc_type *adc, adc_ch_t *ch, uint8_t ch_num, adc_sampletime_select_type sample_time);
void adc_dma_start(adc_type *adc, uint16_t *buf, uint32_t len, cb_t func);
void adc_dma_stop(adc_type *adc);

#endif