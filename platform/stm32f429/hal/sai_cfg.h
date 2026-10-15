#ifndef SAI_CFG_H
#define SAI_CFG_H

#define SAIA_EN 1
#define SAIB_EN 1

/********SAIA MCLK PIN SEL********/
#define SAIA_MCLK_PIN   GPIO_PE2
// #define SAIA_MCLK_PIN   GPIO_PIN_UNUSED 
/********************************/

/********SAIA FS PIN SEL********/
#define SAIA_FS_PIN   GPIO_PE4
// #define SAIA_FS_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA SCK PIN SEL********/
#define SAIA_SCK_PIN   GPIO_PE5
// #define SAIA_SCK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA SD PIN SEL********/
// #define SAIA_SD_PIN   GPIO_PD6
#define SAIA_SD_PIN   GPIO_PE6
// #define SAIA_SD_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA DMA SEL********/
#define SAIA_DMA_EN  1
#define SAIA_DMA_IRQ_PRIORITY 2

#define SAIA_DMA_STREAM     DMA2_Stream1
#define SAIA_DMA_CH         DMA_CHANNEL_0
#define SAIA_DMA_IRQ        DMA2_Stream1_IRQn  
#define SAIA_DMA_IRQHANDLER DMA2_Stream1_IRQHandler    

// #define SAIA_DMA_STREAM     DMA2_Stream3
// #define SAIA_DMA_CH         DMA_CHANNEL_0
// #define SAIA_DMA_IRQ        DMA2_Stream3_IRQn    
// #define SAIA_DMA_IRQHANDLER DMA2_Stream3_IRQHandler   
/********************************/ 



/********SAIB MCLK PIN SEL********/
// #define SAIB_MCLK_PIN   GPIO_PF7
#define SAIB_MCLK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB FS PIN SEL********/
// #define SAIB_FS_PIN   GPIO_PF9
#define SAIB_FS_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB SCK PIN SEL********/
// #define SAIB_SCK_PIN   GPIO_PF8
#define SAIB_SCK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB SD PIN SEL********/
#define SAIB_SD_PIN   GPIO_PE3
// #define SAIB_SD_PIN   GPIO_PF6
// #define SAIB_SD_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB DMA SEL********/
#define SAIB_DMA_EN  1
#define SAIB_DMA_IRQ_PRIORITY 2

// #define SAIB_DMA_STREAM     DMA2_Stream4
// #define SAIB_DMA_CH         DMA_CHANNEL_1
// #define SAIB_DMA_IRQ        DMA2_Stream4_IRQn  
// #define SAIB_DMA_IRQHANDLER DMA2_Stream4_IRQHandler  

#define SAIB_DMA_STREAM     DMA2_Stream5
#define SAIB_DMA_CH         DMA_CHANNEL_0
#define SAIB_DMA_IRQ        DMA2_Stream5_IRQn  
#define SAIB_DMA_IRQHANDLER DMA2_Stream5_IRQHandler
/********************************/ 

typedef struct
{
    SAI_HandleTypeDef handle;
    DMA_HandleTypeDef hdma;
    cb_arg_t func;
} sai_drv_t;

typedef struct 
{
    uint32_t freq;        
    uint32_t pll_n;     
    uint32_t pll_q;       
    uint32_t pll_divq;   
    uint32_t mck_div; 
} sai_psc_t;

#endif