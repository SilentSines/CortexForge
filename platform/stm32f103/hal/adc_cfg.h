#ifndef ADC_CFG_H
#define ADC_CFG_H

#define ADC1_EN 1
#define ADC2_EN 0
#define ADC3_EN 0 

/********ADC1 DMA SEL********/
#define ADC1_DMA_EN  1
#define ADC1_DMA_IRQ_PRIORITY 10

#define ADC1_DMA_CH         DMA1_Channel1
#define ADC1_DMA_IRQ        DMA1_Channel1_IRQn   
#define ADC1_DMA_IRQHANDLER DMA1_Channel1_IRQHandler  
/********************************/

/********ADC3 DMA SEL********/
#define ADC3_DMA_EN  0
#define ADC3_DMA_IRQ_PRIORITY 10

#define ADC3_DMA_CH         DMA2_Channel5
#define ADC3_DMA_IRQ        DMA2_Channel4_5_IRQn   
#define ADC3_DMA_IRQHANDLER DMA2_Channel4_5_IRQHandler  
/********************************/

typedef struct
{
    gpio_pin_t pin;
    uint32_t channel;
} adc_pin_t;

typedef struct
{
    ADC_HandleTypeDef handle;
    DMA_HandleTypeDef *hdma;
    adc_ch_t sampleone_ch;
    bool sampling; 
    cb_t func;
} adc_drv_t;

#endif