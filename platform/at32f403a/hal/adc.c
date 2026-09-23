#include "hal.h"
#include "adc_cfg.h"

static const adc_pin_t adc_pin_table[] =   
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
static adc_drv_t adc_drv1;
#endif

#if ADC2_EN == 1
static adc_drv_t adc_drv2;
#endif

#if ADC3_EN == 1
static adc_drv_t adc_drv3;
#endif

static adc_drv_t* adc_select(adc_type *adc)
{
    adc_drv_t *adc_drv = NULL;

    if (adc == ADC1)
    {
    #if ADC1_EN == 1
		adc_drv = &adc_drv1;
	#endif
    }
    else if (adc == ADC2)
    {
    #if ADC2_EN == 1
		adc_drv = &adc_drv2;
	#endif
    }
    else if (adc == ADC3)
    {
    #if ADC3_EN == 1
		adc_drv = &adc_drv3;
	#endif
    }
    return adc_drv;
}

static bool is_channel_supported(adc_type *adc, adc_ch_t ch)
{
    if (adc == ADC3)
    {
        if (((ch >= ADC12_CH4_PA4) && (ch <= ADC12_CH9_PB1)) ||  (ch >= ADC12_CH14_PC4))      
        {
            return false;
        }
        return true;
    }
    return true;
}

void adc_init(adc_type *adc, bool scan_en)
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

    if (adc == ADC1)
    {
        crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);

    #if ADC1_EN == 1 && ADC1_DMA_EN == 1
        dma = ADC1_DMA;
		adc_drv->dma = ADC1_DMA_CH;
		dma_irq = ADC1_DMA_IRQ;
		dma_irq_prio = ADC1_DMA_IRQ_PRIORITY;
        flex_ch = ADC1_DMA_FLEX;
        flex_req = ADC1_DMA_FLEX_REQ;
	#endif
    }
    else if (adc == ADC2)
    {
        crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);

    #if ADC2_EN == 1 && ADC2_DMA_EN == 1
        dma = ADC2_DMA;
		adc_drv->dma = ADC2_DMA_CH;
		dma_irq = ADC2_DMA_IRQ;
		dma_irq_prio = ADC2_DMA_IRQ_PRIORITY;
        flex_ch = ADC2_DMA_FLEX;
        flex_req = ADC2_DMA_FLEX_REQ;
	#endif
    }
    else if (adc == ADC3)
    {
        crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);

    #if ADC3_EN == 1 && ADC3_DMA_EN == 1
        dma = ADC3_DMA;
		adc_drv->dma = ADC3_DMA_CH;
		dma_irq = ADC3_DMA_IRQ;
		dma_irq_prio = ADC3_DMA_IRQ_PRIORITY;
        flex_ch = ADC3_DMA_FLEX;
        flex_req = ADC3_DMA_FLEX_REQ;
	#endif
    }

    adc_reset(adc);
    crm_adc_clock_div_set(CRM_ADC_DIV_4);
    adc_cfg.sequence_mode = scan_en ? TRUE : FALSE;
    adc_cfg.repeat_mode = FALSE;
    adc_cfg.data_align = ADC_RIGHT_ALIGNMENT;
    adc_cfg.ordinary_channel_length = 1;
    adc_base_config(adc, &adc_cfg);
    adc_combine_mode_select(ADC_INDEPENDENT_MODE);
    adc_ordinary_part_mode_enable(adc, FALSE);
    adc_ordinary_conversion_trigger_set(adc, (adc == ADC3) ? ADC3_ORDINARY_TRIG_SOFTWARE : ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
    adc_enable(adc, TRUE);

    if (adc_drv->dma != NULL)
    {
        dma_reset(adc_drv->dma);
		dma_cfg.buffer_size = 0;
		dma_cfg.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
		dma_cfg.memory_base_addr = 0;
		dma_cfg.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
		dma_cfg.memory_inc_enable = TRUE;
		dma_cfg.peripheral_base_addr = (uint32_t)&(adc->odt);
		dma_cfg.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
		dma_cfg.peripheral_inc_enable = FALSE;
		dma_cfg.priority = DMA_PRIORITY_MEDIUM;
		dma_cfg.loop_mode_enable = FALSE;
		dma_init(adc_drv->dma, &dma_cfg);
		dma_flexible_config(dma, flex_ch, flex_req);
		nvic_irq_enable(dma_irq, dma_irq_prio, 0);
    }
    
    adc_calibration_init(adc);
    while (adc_calibration_init_status_get(adc));
    adc_calibration_start(adc);
    while (adc_calibration_status_get(adc));
}

void adc_setup_channel(adc_ch_t ch)
{
    gpio_pin_init(adc_pin_table[ch].pin, GPIO_IN_ANALOG, GPIO_DRIVE_STRENGTH_MODERATE);
}

uint16_t adc_sample_one(adc_type *adc, adc_ch_t ch, adc_sampletime_select_type sample_time)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if (adc_drv == NULL)
    {
        return 0xFFFF;
    }

    if (is_channel_supported(adc, ch) == false)
    {
        return 0xFFFF;
    }
    
    if (adc_drv->sampleone_ch != ch)
    {
        adc_drv->sampleone_ch = ch;
        adc_ordinary_channel_set(adc, adc_pin_table[ch].channel, 1, sample_time);
        adc_drv->sampling = false;
    }

    if (adc_drv->sampling == false)
    {
        adc_ordinary_software_trigger_enable(adc, true);
        adc_drv->sampling = true;
        return 0xFFFF;
    }
    else
    {
        if (adc_flag_get(adc, ADC_CCE_FLAG) == SET)
        {
            adc_drv->sampling = false;
            return adc_ordinary_conversion_data_get(adc);
        }
        else
        {
            return 0xFFFF;
        }
    }
}

void adc_dma_config(adc_type *adc, adc_ch_t *ch, uint8_t ch_num, adc_sampletime_select_type sample_time)
{
    uint8_t i;
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->dma == NULL))
    {
        return;
    }

    adc->ctrl2_bit.rpen = TRUE;
    adc->osq1_bit.oclen = ch_num - 1;

    for (i = 0; i < ch_num; i++)
    {
        if (is_channel_supported(adc, ch[i]) == false)
        {
            return;
        }

        adc_ordinary_channel_set(adc, adc_pin_table[ch[i]].channel, i, sample_time);
    }
}

void adc_dma_start(adc_type *adc, uint16_t *buf, uint32_t len, cb_t func)
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

    adc_enable(adc, TRUE);
    adc_dma_mode_enable(adc, TRUE);
    adc_ordinary_software_trigger_enable(adc, TRUE);
}

void adc_dma_stop(adc_type *adc)
{
    adc_drv_t *adc_drv = adc_select(adc);

    if ((adc_drv == NULL) || (adc_drv->dma == NULL))
    {
        return;
    }

    adc_enable(adc, FALSE);
    adc_dma_mode_enable(adc, FALSE);
    adc_drv->sampling = false;
}

static void adc_dma_irq_handle(adc_type *adc)
{
    adc_drv_t *adc_drv = adc_select(adc);

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
	adc_dma_irq_handle(ADC1);
}
#endif

#if ADC2_DMA_EN == 1
void ADC2_DMA_IRQHANDLER(void)
{
	adc_dma_irq_handle(ADC2);
}
#endif

#if ADC3_DMA_EN == 1
void ADC3_DMA_IRQHANDLER(void)
{
	adc_dma_irq_handle(ADC3);
}
#endif