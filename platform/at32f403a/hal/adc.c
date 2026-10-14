#include "hal.h"
#include "adc_cfg.h"

static const adc_map_t adc_map_table[] =   
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
    {GPIO_PB1, ADC_CHANNEL_9},
    {GPIO_PC0, ADC_CHANNEL_10},
    {GPIO_PC1, ADC_CHANNEL_11},
    {GPIO_PC2, ADC_CHANNEL_12},
    {GPIO_PC3, ADC_CHANNEL_13},
    {GPIO_PC4, ADC_CHANNEL_14},
    {GPIO_PC5, ADC_CHANNEL_15},
};  

#if ADC1_EN == 1
static adc_drv_t adc_drv1 = {.instance = ADC1};
#endif

#if ADC2_EN == 1
static adc_drv_t adc_drv2 = {.instance = ADC2};
#endif

#if ADC3_EN == 1
static adc_drv_t adc_drv3 = {.instance = ADC3};
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

static bool is_channel_supported(adc_unit_t adc, adc_ch_t ch)
{
    if (adc == ADC_UNIT_3)
    {
        if (((ch >= ADC12_CH4_PA4) && (ch <= ADC12_CH9_PB1)) ||  (ch >= ADC12_CH14_PC4))      
        {
            return false;
        }
        return true;
    }
    return true;
}

static adc_sampletime_select_type map_sample_time(adc_sample_time_t sample_time)
{
    switch (sample_time) 
    {
        case ADC_SAMPLE_TIME_FASTEST: return ADC_SAMPLETIME_1_5;
        case ADC_SAMPLE_TIME_FAST:    return ADC_SAMPLETIME_7_5;
        case ADC_SAMPLE_TIME_MEDIUM:  return ADC_SAMPLETIME_28_5;
        case ADC_SAMPLE_TIME_SLOW:    return ADC_SAMPLETIME_71_5;
        case ADC_SAMPLE_TIME_SLOWEST: return ADC_SAMPLETIME_239_5;
        default:                      return ADC_SAMPLETIME_28_5;
    }
}

void adc_init(adc_unit_t adc, bool scan_en)
{
    dma_type *dma;
    IRQn_Type dma_irq;
	uint32_t dma_irq_prio;
    uint8_t flex_ch;
    dma_flexible_request_type flex_req;

    adc_base_config_type adc_cfg = {0};
    dma_init_type dma_cfg = {0};
    adc_drv_t *adc_drv = adc_select(adc);

    if (adc_drv == NULL)
    {
        return;
    }

    switch (adc)
    {
        case ADC_UNIT_1 :   crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
                        #if ADC1_EN == 1 && ADC1_DMA_EN == 1
                            dma = ADC1_DMA;
                            adc_drv->dma = ADC1_DMA_CH;
                            dma_irq = ADC1_DMA_IRQ;
                            dma_irq_prio = ADC1_DMA_IRQ_PRIORITY;
                            flex_ch = ADC1_DMA_FLEX;
                            flex_req = ADC1_DMA_FLEX_REQ;
                        #endif
                            break;

        case ADC_UNIT_2 :   crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);
                        #if ADC2_EN == 1 && ADC2_DMA_EN == 1
                            dma = ADC2_DMA;
                            adc_drv->dma = ADC2_DMA_CH;
                            dma_irq = ADC2_DMA_IRQ;
                            dma_irq_prio = ADC2_DMA_IRQ_PRIORITY;
                            flex_ch = ADC2_DMA_FLEX;
                            flex_req = ADC2_DMA_FLEX_REQ;
                        #endif
                            break;

        case ADC_UNIT_3 :   crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);
                        #if ADC3_EN == 1 && ADC3_DMA_EN == 1
                            dma = ADC3_DMA;
                            adc_drv->dma = ADC3_DMA_CH;
                            dma_irq = ADC3_DMA_IRQ;
                            dma_irq_prio = ADC3_DMA_IRQ_PRIORITY;
                            flex_ch = ADC3_DMA_FLEX;
                            flex_req = ADC3_DMA_FLEX_REQ;
                        #endif
                            break;

        default : return;
    }

    adc_reset(adc_drv->instance);
    crm_adc_clock_div_set(CRM_ADC_DIV_4);
    adc_cfg.sequence_mode = scan_en ? TRUE : FALSE;
    adc_cfg.repeat_mode = FALSE;
    adc_cfg.data_align = ADC_RIGHT_ALIGNMENT;
    adc_cfg.ordinary_channel_length = 1;
    adc_base_config(adc_drv->instance, &adc_cfg);
    adc_combine_mode_select(ADC_INDEPENDENT_MODE);
    adc_ordinary_part_mode_enable(adc_drv->instance, FALSE);
    adc_ordinary_conversion_trigger_set(adc_drv->instance, (adc == ADC_UNIT_3) ? ADC3_ORDINARY_TRIG_SOFTWARE : ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
    adc_enable(adc_drv->instance, TRUE);

    if (adc_drv->dma != NULL)
    {
        dma_reset(adc_drv->dma);
		dma_cfg.buffer_size = 0;
		dma_cfg.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
		dma_cfg.memory_base_addr = 0;
		dma_cfg.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
		dma_cfg.memory_inc_enable = TRUE;
		dma_cfg.peripheral_base_addr = (uint32_t)&(adc_drv->instance->odt);
		dma_cfg.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
		dma_cfg.peripheral_inc_enable = FALSE;
		dma_cfg.priority = DMA_PRIORITY_MEDIUM;
		dma_cfg.loop_mode_enable = FALSE;
		dma_init(adc_drv->dma, &dma_cfg);
		dma_flexible_config(dma, flex_ch, flex_req);
		nvic_irq_enable(dma_irq, dma_irq_prio, 0);
    }
    
    adc_calibration_init(adc_drv->instance);
    while (adc_calibration_init_status_get(adc_drv->instance));
    adc_calibration_start(adc_drv->instance);
    while (adc_calibration_status_get(adc_drv->instance));
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
        adc_ordinary_channel_set(adc_drv->instance, adc_map_table[ch].channel, 1, map_sample_time(sample_time));
        adc_drv->sampling = false;
    }

    if (adc_drv->sampling == false)
    {
        adc_ordinary_software_trigger_enable(adc_drv->instance, true);
        adc_drv->sampling = true;
        return ADC_SAMPLE_INVALID;
    }
    else
    {
        if (adc_flag_get(adc_drv->instance, ADC_CCE_FLAG) == SET)
        {
            uint16_t data = adc_ordinary_conversion_data_get(adc_drv->instance);
            adc_flag_clear(adc_drv->instance, ADC_CCE_FLAG);
            adc_drv->sampling = false;
            return data;
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
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->dma == NULL))
    {
        return;
    }

    adc_drv->instance->ctrl2_bit.rpen = TRUE;
    adc_drv->instance->osq1_bit.oclen = ch_num - 1;

    for (i = 0; i < ch_num; i++)
    {
        if (is_channel_supported(adc, ch[i]) == false)
        {
            return;
        }

        adc_ordinary_channel_set(adc_drv->instance, adc_map_table[ch[i]].channel, i, map_sample_time(sample_time));
    }
}

void adc_dma_start(adc_unit_t adc, uint16_t *buf, uint32_t len, cb_t func)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->dma == NULL))
    {
        return;
    }

    adc_drv->func = func;
    adc_drv->sampling = true;

    dma_channel_enable(adc_drv->dma, FALSE);
	adc_drv->dma->maddr = (uint32_t)buf;
	adc_drv->dma->dtcnt = len;
	dma_channel_enable(adc_drv->dma, TRUE);
	dma_interrupt_enable(adc_drv->dma, DMA_FDT_INT, TRUE);

    adc_enable(adc_drv->instance, FALSE);
    adc_dma_mode_enable(adc_drv->instance, TRUE);
    adc_enable(adc_drv->instance, TRUE);
    adc_ordinary_software_trigger_enable(adc_drv->instance, TRUE);
}

void adc_dma_stop(adc_unit_t adc)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->dma == NULL))
    {
        return;
    }

    adc_enable(adc_drv->instance, FALSE);
    adc_dma_mode_enable(adc_drv->instance, FALSE);

    dma_interrupt_enable(adc_drv->dma, DMA_FDT_INT, FALSE);
    dma_channel_enable(adc_drv->dma, FALSE);

    adc_drv->sampling = false;
}

static void adc_dma_irq_handle(adc_unit_t adc)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if (adc_drv == NULL)
	{
		return;
	}
    
    if (dma_get_flag(adc_drv->dma, DMA_FDT_FLAG) != RESET)
    {
        dma_clear_flag(adc_drv->dma, DMA_GL_FLAG);

        if (adc_drv->func)
        {
            adc_drv->func();
        }

        adc_drv->sampling = false;
    }
}

#if ADC1_DMA_EN == 1
void ADC1_DMA_IRQHANDLER(void)
{
	adc_dma_irq_handle(ADC_UNIT_1);
}
#endif

#if ADC2_DMA_EN == 1
void ADC2_DMA_IRQHANDLER(void)
{
	adc_dma_irq_handle(ADC_UNIT_2);
}
#endif

#if ADC3_DMA_EN == 1
void ADC3_DMA_IRQHANDLER(void)
{
	adc_dma_irq_handle(ADC_UNIT_3);
}
#endif