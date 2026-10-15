#include "hal.h"
#include "sai_cfg.h"

typedef struct
{
  __IO uint32_t ISR;  
  __IO uint32_t Reserved;
  __IO uint32_t IFCR; 
} sai_dma_base_t;

//MCKDIV != 0: Fs = SAI_CK_x / [512 * MCKDIV]
//MCKDIV == 0: Fs = SAI_CK_x / 256
//SAI_CK_x = (HSE / pllm) * PLLN / PLLQ / (PLLDIVQ + 1)
// HSE 25000000
// pllm 15
const sai_psc_t sai_psc_table[] = 
{
    {8000,   236, 4, 1, 12},   // 8 Khz
    {11025,  237, 5, 6, 2},   // 11.025 Khz
    {16000,  236, 4, 1, 6},    // 16 Khz
    {22050,  237, 5, 6, 1},   // 22.05 Khz
    {44100,  237, 5, 6, 0},   // 44.1 Khz
    {48000,  236, 4, 1, 2},    // 48 Khz
    {96000,  236, 4, 1, 1},    // 96 Khz
    {192000, 236, 4, 1, 0},    // 192 Khz
};

#if SAIA_EN == 1
static sai_drv_t saia_drv;
#endif

#if SAIB_EN == 1
static sai_drv_t saib_drv;
#endif

static sai_drv_t* sai_select(sai_unit_t sai)
{
    switch (sai)
    {
    #if SAIA_EN == 1
        case SAI_UNIT_A : return &saia_drv; break;
    #endif

    #if SAIB_EN == 1
        case SAI_UNIT_B : return &saib_drv; break;
    #endif

        default : return NULL; break;
    }
}

void sai_bus_init(sai_unit_t sai, sai_mode_t mode)
{
    IRQn_Type irq;
    uint32_t irqp;
    uint32_t audio_mode;
    sai_drv_t *sai_drv = sai_select(sai);

	if (sai_drv == NULL)
	{
		return;
	}
    
    switch (sai)
    {
        case SAI_UNIT_A :   gpio_pin_init(SAIA_MCLK_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);
                            gpio_pin_init(SAIA_FS_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);
                            gpio_pin_init(SAIA_SCK_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);
                            gpio_pin_init(SAIA_SD_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);

                            irq = SAIA_DMA_IRQ;
                            irqp = SAIA_DMA_IRQ_PRIORITY;
                            break;

        case SAI_UNIT_B :   gpio_pin_init(SAIB_MCLK_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);
                            gpio_pin_init(SAIB_FS_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);
                            gpio_pin_init(SAIB_SCK_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);
                            gpio_pin_init(SAIB_SD_PIN, GPIO_AF_PP, GPIO_AF6_SAI1);

                            irq = SAIB_DMA_IRQ;
                            irqp = SAIB_DMA_IRQ_PRIORITY;
                            break;

        default : return; break;
    }

    __HAL_RCC_SAI1_CLK_ENABLE(); 

    switch (mode)
    {
        case SAI_MASTER_TX : audio_mode = SAI_MODEMASTER_TX; break;
        case SAI_MASTER_RX : audio_mode = SAI_MODEMASTER_RX; break;
        case SAI_SLAVE_TX :  audio_mode = SAI_MODESLAVE_TX; break;
        case SAI_SLAVE_RX :  audio_mode = SAI_MODESLAVE_RX; break;
        default :            audio_mode = SAI_MODEMASTER_TX; break;
    }
                          
    sai_drv->handle.Instance = (sai == SAI_UNIT_A) ? SAI1_Block_A : SAI1_Block_B;                    
    sai_drv->handle.Init.AudioMode = audio_mode;                      
    sai_drv->handle.Init.Synchro = ((audio_mode == SAI_MODEMASTER_TX) || (audio_mode == SAI_MODEMASTER_RX)) ? SAI_ASYNCHRONOUS : SAI_SYNCHRONOUS;            
    sai_drv->handle.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;  
    sai_drv->handle.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;  
    sai_drv->handle.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF; 
    sai_drv->handle.Init.ClockSource = SAI_CLKSOURCE_PLLI2S;    
    sai_drv->handle.Init.MonoStereoMode = SAI_STEREOMODE;    
    sai_drv->handle.Init.Protocol = SAI_FREE_PROTOCOL;        
    sai_drv->handle.Init.DataSize = SAI_DATASIZE_8;                    
    sai_drv->handle.Init.FirstBit = SAI_FIRSTBIT_MSB;     
    sai_drv->handle.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;                 
    sai_drv->handle.FrameInit.FrameLength = 64;                            
    sai_drv->handle.FrameInit.ActiveFrameLength = 32;                       
    sai_drv->handle.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
    sai_drv->handle.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;               
    sai_drv->handle.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;            
    sai_drv->handle.SlotInit.FirstBitOffset = 0;                               
    sai_drv->handle.SlotInit.SlotSize = SAI_SLOTSIZE_32B;                    
    sai_drv->handle.SlotInit.SlotNumber = 2;                                 
    sai_drv->handle.SlotInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;
    if (HAL_SAI_Init(&sai_drv->handle) != HAL_OK)
	{
		hal_error_handler();
	}

    if ((audio_mode == SAI_MODEMASTER_TX) || (audio_mode == SAI_MODESLAVE_TX))
    {
        __HAL_LINKDMA(&sai_drv->handle, hdmatx, sai_drv->hdma);

        sai_drv->hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    }
    else if ((audio_mode == SAI_MODEMASTER_RX) || (audio_mode == SAI_MODESLAVE_RX))
    {
        __HAL_LINKDMA(&sai_drv->handle, hdmarx, sai_drv->hdma);

        sai_drv->hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    }
    sai_drv->hdma.Instance = (sai == SAI_UNIT_A) ? SAIA_DMA_STREAM : SAIB_DMA_STREAM;
    sai_drv->hdma.Init.Channel = (sai == SAI_UNIT_A) ? SAIA_DMA_CH : SAIB_DMA_CH;
    sai_drv->hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    sai_drv->hdma.Init.MemInc = DMA_MINC_ENABLE;
    sai_drv->hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    sai_drv->hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    sai_drv->hdma.Init.Mode = DMA_CIRCULAR;
    sai_drv->hdma.Init.Priority = DMA_PRIORITY_HIGH;
    sai_drv->hdma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    sai_drv->hdma.Init.MemBurst = DMA_MBURST_SINGLE;
    sai_drv->hdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&sai_drv->hdma) != HAL_OK)
	{
		hal_error_handler();
	}

    HAL_NVIC_SetPriority(irq, irqp, 0);
    HAL_NVIC_EnableIRQ(irq);
}

void sai_set_cfg(sai_unit_t sai, sai_sample_rate_t samplerate, sai_data_width_t width)
{
    uint8_t i = 0;
    uint32_t data_size;
    RCC_PeriphCLKInitTypeDef rcc_init = {0};
    sai_drv_t *sai_drv = sai_select(sai);

	if (sai_drv == NULL)
	{
		return;
	}

    for (i = 0; i < (sizeof(sai_psc_table) / sizeof(sai_psc_table[0])); i++) 
    {
        if (samplerate == sai_psc_table[i].freq)
        {    
            break;
        }
    }
    if (i == (sizeof(sai_psc_table) / sizeof(sai_psc_table[0])))
    {    
        return;    
    }                                             
    rcc_init.PeriphClockSelection = RCC_PERIPHCLK_SAI_PLLI2S;
    rcc_init.PLLI2S.PLLI2SN = sai_psc_table[i].pll_n;    
    rcc_init.PLLI2S.PLLI2SQ = sai_psc_table[i].pll_q;
    rcc_init.PLLI2SDivQ = sai_psc_table[i].pll_divq + 1; 
    if (HAL_RCCEx_PeriphCLKConfig(&rcc_init) != HAL_OK)
	{
		hal_error_handler();
	}

    if (sai == SAI_UNIT_A)
    {
        __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(RCC_SAIACLKSOURCE_PLLI2S); 
    }
    else
    {
        __HAL_RCC_SAI_BLOCKBCLKSOURCE_CONFIG(RCC_SAIBCLKSOURCE_PLLI2S); 
    }

    switch (width)
    {
        case SAI_DATA_WIDTH_8BIT :  data_size = SAI_DATASIZE_8; break;
        case SAI_DATA_WIDTH_10BIT : data_size = SAI_DATASIZE_10; break;
        case SAI_DATA_WIDTH_16BIT : data_size = SAI_DATASIZE_16; break;
        case SAI_DATA_WIDTH_20BIT : data_size = SAI_DATASIZE_20; break;
        case SAI_DATA_WIDTH_24BIT : data_size = SAI_DATASIZE_24; break;
        case SAI_DATA_WIDTH_32BIT : data_size = SAI_DATASIZE_32; break;
        default :                   data_size = SAI_DATASIZE_16; break;
    }

    sai_drv->handle.Init.AudioFrequency = samplerate; 
    sai_drv->handle.Init.DataSize = data_size; 
    if (HAL_SAI_Init(&sai_drv->handle) != HAL_OK)
	{
		hal_error_handler();
	}    
    
    switch(data_size)
    {
        case SAI_DATASIZE_8 :   sai_drv->hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
                                sai_drv->hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
                                break;

        case SAI_DATASIZE_10 :
        case SAI_DATASIZE_16 :  sai_drv->hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
                                sai_drv->hdma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
                                break;

        case SAI_DATASIZE_20 :
        case SAI_DATASIZE_24 :
        case SAI_DATASIZE_32 :  sai_drv->hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
                                sai_drv->hdma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
                                break;
            
    }
    if (HAL_DMA_Init(&sai_drv->hdma) != HAL_OK)
	{
		hal_error_handler();
	}
} 

void sai_set_dma(sai_unit_t sai, void* buf0, void *buf1, uint32_t len, cb_arg_t func)
{ 
    sai_drv_t *sai_drv = sai_select(sai);

	if (sai_drv == NULL)
	{
		return;
	}

    uint32_t dr_addr = (uint32_t)&sai_drv->handle.Instance->DR;

    __HAL_DMA_DISABLE(&sai_drv->hdma); 

    if (buf1)
    {
        if (sai_drv->handle.hdmatx)
        {
            if (HAL_DMAEx_MultiBufferStart(sai_drv->handle.hdmatx, (uint32_t)buf0, dr_addr, (uint32_t)buf1, len) != HAL_OK)
            {
                hal_error_handler();
            }
        }
        else if (sai_drv->handle.hdmarx)
        {
            if (HAL_DMAEx_MultiBufferStart(sai_drv->handle.hdmarx, dr_addr, (uint32_t)buf0, (uint32_t)buf1, len) != HAL_OK)
            {
                hal_error_handler();
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        if (sai_drv->handle.hdmatx)
        {
            sai_drv->handle.hdmatx->Instance->M0AR = (uint32_t)buf0;
            sai_drv->handle.hdmatx->Instance->NDTR = len;
            sai_drv->handle.hdmatx->Instance->PAR = dr_addr;
        }
        else if (sai_drv->handle.hdmarx)
        {
            sai_drv->handle.hdmarx->Instance->M0AR = (uint32_t)buf0;
            sai_drv->handle.hdmarx->Instance->NDTR = len;
            sai_drv->handle.hdmarx->Instance->PAR = dr_addr;
        }
        else
        {
            return;
        }
    }
    
    __HAL_DMA_CLEAR_FLAG (&sai_drv->hdma, __HAL_DMA_GET_TC_FLAG_INDEX(&sai_drv->hdma));
    __HAL_DMA_CLEAR_FLAG (&sai_drv->hdma, __HAL_DMA_GET_HT_FLAG_INDEX(&sai_drv->hdma));
    __HAL_DMA_CLEAR_FLAG (&sai_drv->hdma, __HAL_DMA_GET_TE_FLAG_INDEX(&sai_drv->hdma));
    __HAL_DMA_CLEAR_FLAG (&sai_drv->hdma, __HAL_DMA_GET_DME_FLAG_INDEX(&sai_drv->hdma));
    __HAL_DMA_CLEAR_FLAG (&sai_drv->hdma, __HAL_DMA_GET_FE_FLAG_INDEX(&sai_drv->hdma));
    __HAL_DMA_ENABLE_IT(&sai_drv->hdma, DMA_IT_TC);

    sai_drv->func = func;
}

void sai_start(sai_unit_t sai)
{
    sai_drv_t *sai_drv = sai_select(sai);

	if (sai_drv == NULL)
	{
		return;
	}

    __HAL_DMA_ENABLE(&sai_drv->hdma);
    __HAL_SAI_ENABLE(&sai_drv->handle);
    SET_BIT(sai_drv->handle.Instance->CR1, SAI_xCR1_DMAEN);
}

void sai_stop(sai_unit_t sai)
{
    sai_drv_t *sai_drv = sai_select(sai);

	if (sai_drv == NULL)
	{
		return;
	}

    __HAL_DMA_DISABLE(&sai_drv->hdma);
    HAL_SAI_DMAStop(&sai_drv->handle);
}

static void sai_dma_irq_handle(sai_drv_t *sai_drv)
{
	if (sai_drv == NULL)
	{
		return;
	}

    DMA_HandleTypeDef *hdma = &sai_drv->hdma;
    sai_dma_base_t *base = (sai_dma_base_t *)hdma->StreamBaseAddress;
    cb_arg_t func = sai_drv->func;

    if ((base->ISR & (DMA_FLAG_TCIF0_4 << hdma->StreamIndex)) != RESET)
    {
        if (__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET)
        {
            base->IFCR = DMA_FLAG_TCIF0_4 << hdma->StreamIndex;
            if (hdma->Instance->CR & DMA_SxCR_CT)
            {
                if (func)
                {
                    func((void *)SAI_DMA_BUF_FIRST);
                }
            }
            else
            {
                if (func)
                {
                    func((void *)SAI_DMA_BUF_SECOND);
                }
            }
        }
    }
}

#if SAIA_EN == 1 && SAIA_DMA_EN == 1
void SAIA_DMA_IRQHANDLER(void)
{   	
    sai_dma_irq_handle(&saia_drv);						 
} 
#endif 

#if SAIB_EN == 1 && SAIB_DMA_EN == 1
void SAIB_DMA_IRQHANDLER(void)
{   
    sai_dma_irq_handle(&saib_drv);		
}  
#endif