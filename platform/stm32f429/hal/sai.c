#include "bsp.h" 
#include "sai_cfg.h"

#define INIT_SAI_PIN(x, y)                \
    {                                     \
        pin = y;                          \
        if (pin != GPIO_PIN_UNUSED)       \
        {                                 \
            gpio_init.Pin = y;            \
            HAL_GPIO_Init(x, &gpio_init); \
        }                                 \
    }

static sai_drv_t sai_drv[2];

void sai_init(sai_t sai, uint32_t mode, uint32_t cpol)
{
    uint32_t pin;
    GPIO_InitTypeDef gpio_init = {0};
    sai_drv_t *drv = &sai_drv[sai];
    IRQn_Type irq = (sai == SAI_A) ? SAIA_DMA_IRQ : SAIB_DMA_IRQ;
    uint32_t irqp = (sai == SAI_A) ? SAIA_DMA_IRQ_PRIORITY : SAIB_DMA_IRQ_PRIORITY;
    
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = GPIO_AF6_SAI1;
    if (sai == SAI_A)
    {
        INIT_SAI_PIN(SAIA_MCLK_PORT, SAIA_MCLK_PIN);
        INIT_SAI_PIN(SAIA_FS_PORT, SAIA_FS_PIN);
        INIT_SAI_PIN(SAIA_SCK_PORT, SAIA_SCK_PIN);
        INIT_SAI_PIN(SAIA_SD_PORT, SAIA_SD_PIN);
    }
    else if (sai == SAI_B)
    {
        INIT_SAI_PIN(SAIB_MCLK_PORT, SAIB_MCLK_PIN);
        INIT_SAI_PIN(SAIB_FS_PORT, SAIB_FS_PIN);
        INIT_SAI_PIN(SAIB_SCK_PORT, SAIB_SCK_PIN);
        INIT_SAI_PIN(SAIB_SD_PORT, SAIB_SD_PIN);
    }
    else
    {
        return;
    }
    
    __HAL_RCC_SAI1_CLK_ENABLE(); 
                          
    drv->handle.Instance = (sai == SAI_A) ? SAI1_Block_A : SAI1_Block_B;                    
    drv->handle.Init.AudioMode = mode;                      
    drv->handle.Init.Synchro = ((mode == SAI_MODEMASTER_TX) || (mode == SAI_MODEMASTER_RX)) 
                                ? SAI_ASYNCHRONOUS 
                                : SAI_SYNCHRONOUS;            
    drv->handle.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;  
    drv->handle.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;  
    drv->handle.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF; 
    drv->handle.Init.ClockSource = SAI_CLKSOURCE_PLLI2S;    
    drv->handle.Init.MonoStereoMode = SAI_STEREOMODE;    
    drv->handle.Init.Protocol = SAI_FREE_PROTOCOL;        
    drv->handle.Init.DataSize = SAI_DATASIZE_8;                    
    drv->handle.Init.FirstBit = SAI_FIRSTBIT_MSB;     
    drv->handle.Init.ClockStrobing = cpol;                 
    drv->handle.FrameInit.FrameLength = 64;                            
    drv->handle.FrameInit.ActiveFrameLength = 32;                       
    drv->handle.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
    drv->handle.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;               
    drv->handle.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;            
    drv->handle.SlotInit.FirstBitOffset = 0;                               
    drv->handle.SlotInit.SlotSize = SAI_SLOTSIZE_32B;                    
    drv->handle.SlotInit.SlotNumber = 2;                                 
    drv->handle.SlotInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;
    if (HAL_SAI_Init(&drv->handle) != HAL_OK)
	{
		hal_error_handler();
	}

    if ((mode == SAI_MODEMASTER_TX) || (mode == SAI_MODESLAVE_TX))
    {
        drv->hdma_tx = &drv->dma;
        drv->hdma_rx = NULL;
        __HAL_LINKDMA(&drv->handle, hdmatx, drv->dma);

        drv->dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    }
    else if ((mode == SAI_MODEMASTER_RX) || (mode == SAI_MODESLAVE_RX))
    {
        drv->hdma_rx = &drv->dma;
        drv->hdma_tx = NULL;
        __HAL_LINKDMA(&drv->handle, hdmarx, drv->dma);

        drv->dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    }
    drv->dma.Instance = (sai == SAI_A) ? SAIA_DMA_STREAM : SAIB_DMA_STREAM;
    drv->dma.Init.Channel = (sai == SAI_A) ? SAIA_DMA_CH : SAIB_DMA_CH;
    drv->dma.Init.PeriphInc = DMA_PINC_DISABLE;
    drv->dma.Init.MemInc = DMA_MINC_ENABLE;
    drv->dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    drv->dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    drv->dma.Init.Mode = DMA_CIRCULAR;
    drv->dma.Init.Priority = DMA_PRIORITY_HIGH;
    drv->dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    drv->dma.Init.MemBurst = DMA_MBURST_SINGLE;
    drv->dma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&drv->dma) != HAL_OK)
	{
		hal_error_handler();
	}

    HAL_NVIC_SetPriority(irq, irqp, 0);
    HAL_NVIC_EnableIRQ(irq);
}

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

void sai_config(sai_t sai, uint32_t samplerate, uint32_t bits)
{
    uint8_t i = 0;
    sai_drv_t *drv = &sai_drv[sai];
    RCC_PeriphCLKInitTypeDef rcc_init = {0};

    for (i = 0; i < ARRAY_SIZE(sai_psc_table); i++) 
    {
        if (samplerate == sai_psc_table[i].freq)
        {    
            break;
        }
    }
    if (i == ARRAY_SIZE(sai_psc_table))
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

    if (sai == SAI_A)
    {
        __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(RCC_SAIACLKSOURCE_PLLI2S); 
    }
    else
    {
        __HAL_RCC_SAI_BLOCKBCLKSOURCE_CONFIG(RCC_SAIBCLKSOURCE_PLLI2S); 
    }

    drv->handle.Init.AudioFrequency = samplerate; 
    drv->handle.Init.DataSize = bits; 
    if (HAL_SAI_Init(&drv->handle) != HAL_OK)
	{
		hal_error_handler();
	}    
    
    switch(bits)
    {
        case SAI_DATASIZE_8 :   drv->dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
                                drv->dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
                                break;

        case SAI_DATASIZE_10 :
        case SAI_DATASIZE_16 :  drv->dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
                                drv->dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
                                break;

        case SAI_DATASIZE_20 :
        case SAI_DATASIZE_24 :
        case SAI_DATASIZE_32 :  drv->dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
                                drv->dma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
                                break;
            
    }
    if (HAL_DMA_Init(&drv->dma) != HAL_OK)
	{
		hal_error_handler();
	}
} 

void sai_dma_config(sai_t sai, void* buf0, void *buf1, uint32_t size, cb_arg_t func)
{ 
    sai_drv_t *drv = &sai_drv[sai];

    if (buf1)
    {
        if (drv->hdma_tx)
        {
            if (HAL_DMAEx_MultiBufferStart(drv->hdma_tx, (uint32_t)buf0, (uint32_t)&drv->handle.Instance->DR, (uint32_t)buf1, size) != HAL_OK)
            {
                hal_error_handler();
            }
        }
        else if (drv->hdma_rx)
        {
            if (HAL_DMAEx_MultiBufferStart(drv->hdma_rx, (uint32_t)&drv->handle.Instance->DR, (uint32_t)buf0, (uint32_t)buf1, size) != HAL_OK)
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
        if (drv->hdma_tx)
        {
            drv->hdma_tx->Instance->M0AR = (uint32_t)buf0;
            drv->hdma_tx->Instance->NDTR = size;
            drv->hdma_tx->Instance->PAR = (uint32_t)&drv->handle.Instance->DR;
        }
        else if (drv->hdma_rx)
        {
            drv->hdma_tx->Instance->M0AR = (uint32_t)&drv->handle.Instance->DR;
            drv->hdma_tx->Instance->NDTR = size;
            drv->hdma_tx->Instance->PAR =  (uint32_t)buf0;
        }
        else
        {
            return;
        }
    }
    
    __HAL_DMA_DISABLE(&drv->dma); 
    __HAL_DMA_CLEAR_FLAG (&drv->dma, __HAL_DMA_GET_TC_FLAG_INDEX(&drv->dma));
    __HAL_DMA_CLEAR_FLAG (&drv->dma, __HAL_DMA_GET_HT_FLAG_INDEX(&drv->dma));
    __HAL_DMA_CLEAR_FLAG (&drv->dma, __HAL_DMA_GET_TE_FLAG_INDEX(&drv->dma));
    __HAL_DMA_CLEAR_FLAG (&drv->dma, __HAL_DMA_GET_DME_FLAG_INDEX(&drv->dma));
    __HAL_DMA_CLEAR_FLAG (&drv->dma, __HAL_DMA_GET_FE_FLAG_INDEX(&drv->dma));
    __HAL_DMA_ENABLE_IT(&drv->dma, DMA_IT_TC);

    drv->cb = func;
}

void sai_start(sai_t sai)
{
    sai_drv_t *drv = &sai_drv[sai];

    __HAL_DMA_ENABLE(&drv->dma);
    __HAL_SAI_ENABLE(&drv->handle);
    SET_BIT(drv->handle.Instance->CR1, SAI_xCR1_DMAEN);
}

void sai_stop(sai_t sai)
{
    sai_drv_t *drv = &sai_drv[sai];

    __HAL_DMA_DISABLE(&drv->dma);
    HAL_SAI_DMAStop(&drv->handle);
}

#if SAIA_DMA_HANDLER == 0
void DMA2_Stream1_IRQHandler(void)
{   
    if ((DMA2->LISR & DMA_FLAG_TCIF1_5) != RESET)
    {
        DMA2->LIFCR = DMA_FLAG_TCIF1_5;
        sai_drv[0].cb((void *)(DMA2_Stream1->CR & (1 << 19)));
    }							 
}  
#else
void DMA2_Stream3_IRQHandler(void)
{
    if ((DMA2->LISR & DMA_FLAG_TCIF3_7) != RESET)
    {
        DMA2->LIFCR = DMA_FLAG_TCIF3_7;
        sai_drv[0].cb((void *)(DMA2_Stream3->CR & (1 << 19)));
    }
}  
#endif

#if SAIB_DMA_HANDLER == 0
void DMA2_Stream4_IRQHandler(void)
{   
    if ((DMA2->HISR & DMA_FLAG_TCIF0_4) != RESET)
    {
        DMA2->HIFCR = DMA_FLAG_TCIF0_4;
        sai_drv[1].cb((void *)(DMA2_Stream4->CR & (1 << 19)));
    }
}  
#else
void DMA2_Stream5_IRQHandler(void)
{   
    if ((DMA2->HISR & DMA_FLAG_TCIF1_5) != RESET)
    {
        DMA2->HIFCR = DMA_FLAG_TCIF1_5;
        sai_drv[1].cb((void *)(DMA2_Stream5->CR & (1 << 19)));
    }
}  
#endif