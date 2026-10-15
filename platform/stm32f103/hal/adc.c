#include "hal.h"
#include "adc_cfg.h"

static const adc_pin_t adc_map_table[] =   
{
    {GPIO_PA0, ADC_CHANNEL_0},
    {GPIO_PA1, ADC_CHANNEL_1},
    {GPIO_PA2, ADC_CHANNEL_2},
    {GPIO_PA3, ADC_CHANNEL_3},

    {GPIO_PA4, ADC_CHANNEL_4},
    {GPIO_PA5, ADC_CHANNEL_5},
    {GPIO_PA6, ADC_CHANNEL_6},
    {GPIO_PA7, ADC_CHANNEL_7},
    {GPIO_PB0, ADC_CHANNEL_8},
    {GPIO_PF6, ADC_CHANNEL_4},
    {GPIO_PF7, ADC_CHANNEL_5},
    {GPIO_PF8, ADC_CHANNEL_6},
    {GPIO_PF9, ADC_CHANNEL_7},
    {GPIO_PF10, ADC_CHANNEL_8},

    {GPIO_PB1, ADC_CHANNEL_9},

    {GPIO_PC0, ADC_CHANNEL_10},
    {GPIO_PC1, ADC_CHANNEL_11},
    {GPIO_PC2, ADC_CHANNEL_12},
    {GPIO_PC3, ADC_CHANNEL_13},

    {GPIO_PC4, ADC_CHANNEL_14},
    {GPIO_PC5, ADC_CHANNEL_15},
};  

#if ADC1_EN == 1
static adc_drv_t adc_drv1;
#if ADC1_DMA_EN == 1
static DMA_HandleTypeDef adc1_dma;
#endif
#endif

#if ADC2_EN == 1
static adc_drv_t adc_drv2;
#if ADC2_DMA_EN == 1
static DMA_HandleTypeDef adc2_dma;
#endif
#endif

#if ADC3_EN == 1
static adc_drv_t adc_drv3;
#if ADC3_DMA_EN == 1
static DMA_HandleTypeDef adc3_dma;
#endif
#endif

static adc_drv_t* adc_select(adc_unit_t adc)
{
    switch (adc)
    {
    #if ADC1_EN == 1
        case ADC_UNIT_1 : return &adc_drv1; break;
    #endif

    #if ADC2_EN == 1
        case ADC_UNIT_2 : return &adc_drv2; break;
    #endif

    #if ADC3_EN == 1
        case ADC_UNIT_3 : return &adc_drv3; break;
    #endif

        default : return NULL; break;
    }
}

static adc_drv_t* adc_select_by_handle(ADC_HandleTypeDef *handle)
{
#if ADC1_EN == 1
    if (handle == &adc_drv1.handle)
    {
        return &adc_drv1;
    }
#endif

#if ADC2_EN == 1
    if (handle == &adc_drv2.handle)
    {
        return &adc_drv2;
    }
#endif

#if ADC3_EN == 1
    if (handle == &adc_drv3.handle)
    {
        return &adc_drv3;
    }
#endif

    return NULL;
}

static bool is_channel_supported(adc_unit_t adc, adc_ch_t ch)
{
    if (adc == ADC_UNIT_1 || adc == ADC_UNIT_2)
    {
        if ((ch >= ADC3_CH4_PF6) && (ch <= ADC3_CH8_PF10))
        {
            return false;
        }
        return true;
    }
    else if (adc == ADC_UNIT_3)
    {
        if (((ch >= ADC12_CH4_PA4) && (ch <= ADC12_CH8_PB0)) ||   
            (ch == ADC12_CH9_PB1) || (ch >= ADC12_CH14_PC4))      
        {
            return false;
        }
        return true;
    }
    return false;
}

static uint32_t map_sample_time(adc_sample_time_t sample_time)
{
    switch (sample_time) 
    {
        case ADC_SAMPLE_TIME_FASTEST: return ADC_SAMPLETIME_1CYCLE_5;
        case ADC_SAMPLE_TIME_FAST:    return ADC_SAMPLETIME_7CYCLES_5;
        case ADC_SAMPLE_TIME_MEDIUM:  return ADC_SAMPLETIME_28CYCLES_5;
        case ADC_SAMPLE_TIME_SLOW:    return ADC_SAMPLETIME_71CYCLES_5;
        case ADC_SAMPLE_TIME_SLOWEST: return ADC_SAMPLETIME_239CYCLES_5;
        default:                      return ADC_SAMPLETIME_28CYCLES_5;
    }
}

void adc_init(adc_unit_t adc, bool scan_en)
{
    IRQn_Type dma_irq;
	uint32_t dma_irq_prio;
    adc_drv_t *adc_drv = adc_select(adc);

    if (adc_drv == NULL)
    {
        return;
    }
    
    switch (adc)
    {
        case ADC_UNIT_1 :   __HAL_RCC_ADC1_CLK_ENABLE();
                            adc_drv->handle.Instance = ADC1;
                        #if ADC1_EN == 1 && ADC1_DMA_EN == 1
                            adc_drv->hdma = &adc1_dma;
                            adc1_dma.Instance = ADC1_DMA_CH;
                            dma_irq = ADC1_DMA_IRQ;
                            dma_irq_prio = ADC1_DMA_IRQ_PRIORITY;
                        #endif
                            break;
    
        case ADC_UNIT_2 :   __HAL_RCC_ADC2_CLK_ENABLE();
                            adc_drv->handle.Instance = ADC2;
                        #if ADC2_EN == 1 && ADC2_DMA_EN == 1
                            adc_drv->hdma = &adc2_dma;
                            adc2_dma.Instance = ADC2_DMA_CH;
                            dma_irq = ADC2_DMA_IRQ;
                            dma_irq_prio = ADC2_DMA_IRQ_PRIORITY;
	                    #endif
                            break;

        case ADC_UNIT_3 :   __HAL_RCC_ADC3_CLK_ENABLE();
                            adc_drv->handle.Instance = ADC3;
                        #if ADC3_DMA_EN
                            adc_drv->hdma = &adc3_dma;
                            adc3_dma.Instance = ADC3_DMA_CH;
                            dma_irq = ADC3_DMA_IRQ;
                            dma_irq_prio = ADC3_DMA_IRQ_PRIORITY;
                        #endif
                            break;
        
        default : return; break;
    }
    
    adc_drv->sampleone_ch = ADC_CH_MAX;
    adc_drv->handle.Init.ScanConvMode = scan_en ? ADC_SCAN_ENABLE : ADC_SCAN_DISABLE;
    adc_drv->handle.Init.ContinuousConvMode = DISABLE;
    adc_drv->handle.Init.DiscontinuousConvMode = DISABLE;
    adc_drv->handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_drv->handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_drv->handle.Init.NbrOfConversion = 1;
    adc_drv->handle.Init.NbrOfDiscConversion = 0;
    if (HAL_ADC_Init(&adc_drv->handle) != HAL_OK)
    {
        hal_error_handler();
    }

    if (adc_drv->hdma != NULL)
	{
		adc_drv->hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
		adc_drv->hdma->Init.PeriphInc = DMA_PINC_DISABLE;
		adc_drv->hdma->Init.MemInc = DMA_MINC_ENABLE;
		adc_drv->hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		adc_drv->hdma->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		adc_drv->hdma->Init.Mode = DMA_NORMAL;
		adc_drv->hdma->Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(adc_drv->hdma) != HAL_OK)
		{
			hal_error_handler();
		}
		__HAL_LINKDMA(&adc_drv->handle, DMA_Handle, *(adc_drv->hdma));
		HAL_NVIC_SetPriority(dma_irq, dma_irq_prio, 0);
   	 	HAL_NVIC_EnableIRQ(dma_irq);
	}

    HAL_ADCEx_Calibration_Start(&adc_drv->handle);
}

void adc_setup_channel(adc_ch_t ch)
{
    if (ch >= ADC_CH_MAX) 
    {
        return;
    }

    gpio_pin_init(adc_map_table[ch].pin, GPIO_IN_ANALOG, GPIO_AF_ADC);
}

uint16_t adc_sample_one(adc_unit_t adc, adc_ch_t ch, adc_sample_time_t sample_time)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if (adc_drv == NULL)
    {
        return ADC_SAMPLE_INVALID;
    }
    
    if (is_channel_supported(adc, ch) == false)
    {
        return ADC_SAMPLE_INVALID;
    }
    
    if (adc_drv->sampleone_ch != ch)
    {
        adc_drv->sampleone_ch = ch;

        ADC_ChannelConfTypeDef config = {0};

        config.Channel = adc_map_table[ch].channel;
        config.Rank = ADC_REGULAR_RANK_1;
        config.SamplingTime = map_sample_time(sample_time);
        if (HAL_ADC_ConfigChannel(&adc_drv->handle, &config) != HAL_OK)
        {
            hal_error_handler();
        }
        adc_drv->sampling = false;
    }
    
    if (adc_drv->sampling == false)
    {
        if (HAL_ADC_Start(&adc_drv->handle) == HAL_OK)
        {
            adc_drv->sampling = true;
        }
        return ADC_SAMPLE_INVALID;
    }
    else
    {
        if (HAL_IS_BIT_SET(adc_drv->handle.Instance->SR, ADC_FLAG_EOC))
        {
            adc_drv->sampling = false;
            uint16_t value = HAL_ADC_GetValue(&adc_drv->handle);
            __HAL_ADC_CLEAR_FLAG(&adc_drv->handle, ADC_FLAG_EOC);
            HAL_ADC_Stop(&adc_drv->handle);
            return value;
        }
        else
        {
            return ADC_SAMPLE_INVALID;
        }
    }
}

void adc_dma_config(adc_unit_t adc, adc_ch_t *ch, uint8_t ch_num, adc_sample_time_t sample_time)
{
    uint8_t i;
    ADC_ChannelConfTypeDef config = {0};
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->hdma == NULL))
    {
        return;
    }

    adc_drv->handle.Init.ContinuousConvMode = ENABLE;
    SET_BIT(adc_drv->handle.Instance->CR2, ADC_CR2_CONT);

    adc_drv->handle.Init.NbrOfConversion = ch_num;
    CLEAR_BIT(adc_drv->handle.Instance->SQR1, ADC_SQR1_L);
    SET_BIT(adc_drv->handle.Instance->SQR1, ADC_SQR1_L_SHIFT(ch_num));

    for (i = 0; i < ch_num; i++)
    {
        if (is_channel_supported(adc, ch[i]) == false)
        {
            return;
        }
        config.Channel = adc_map_table[ch[i]].channel;
        config.Rank = i + ADC_REGULAR_RANK_1;
        config.SamplingTime = map_sample_time(sample_time);
        if (HAL_ADC_ConfigChannel(&adc_drv->handle, &config) != HAL_OK) 
        {
            return;
        }
    }
}

void adc_dma_start(adc_unit_t adc, uint16_t *buf, uint32_t len, cb_t func)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->hdma == NULL))
    {
        return;
    }

    adc_drv->func = func;
    adc_drv->sampling = true;
    HAL_ADC_Start_DMA(&adc_drv->handle, (uint32_t *)buf, len);
}

void adc_dma_stop(adc_unit_t adc)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->hdma == NULL))
    {
        return;
    }

    HAL_ADC_Stop_DMA(&adc_drv->handle);
    adc_drv->sampling = false;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_drv_t *adc_drv = adc_select_by_handle(hadc);

    if (adc_drv == NULL)
	{
		return;
	}

	if (adc_drv->func)
	{
		adc_drv->func();
	}

    adc_drv->sampling = false;
}

#if ADC1_DMA_EN == 1
void ADC1_DMA_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&adc1_dma);
}
#endif

#if ADC2_DMA_EN == 1
void ADC2_DMA_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&adc2_dma);
}
#endif

#if ADC3_DMA_EN == 1
void ADC3_DMA_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&adc3_dma);
}
#endif