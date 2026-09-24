/********SAIA MCLK PIN SEL********/
#define SAIA_MCLK_PORT  GPIOE
#define SAIA_MCLK_PIN   GPIO_PIN_2

// #define SAIA_MCLK_PORT  GPIOE
// #define SAIA_MCLK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA FS PIN SEL********/
#define SAIA_FS_PORT  GPIOE
#define SAIA_FS_PIN   GPIO_PIN_4

// #define SAIA_FS_PORT  GPIOE
// #define SAIA_FS_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA SCK PIN SEL********/
#define SAIA_SCK_PORT  GPIOE
#define SAIA_SCK_PIN   GPIO_PIN_5

// #define SAIA_SCK_PORT  GPIOE
// #define SAIA_SCK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA SD PIN SEL********/
// #define SAIA_SD_PORT  GPIOD
// #define SAIA_SD_PIN   GPIO_PIN_6

#define SAIA_SD_PORT  GPIOE
#define SAIA_SD_PIN   GPIO_PIN_6

// #define SAIA_SD_PORT  GPIOE
// #define SAIA_SD_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIA DMA SEL********/
#define SAIA_DMA_IRQ_PRIORITY 2

#define SAIA_DMA_STREAM DMA2_Stream1
#define SAIA_DMA_CH     DMA_CHANNEL_0
#define SAIA_DMA_IRQ    DMA2_Stream1_IRQn  
#define SAIA_DMA_HANDLER 0     

// #define SAIA_DMA_STREAM DMA2_Stream3
// #define SAIA_DMA_CH     DMA_CHANNEL_0
// #define SAIA_DMA_IRQ    DMA2_Stream3_IRQn    
// #define SAIA_DMA_HANDLER    1   
/********************************/ 



/********SAIB MCLK PIN SEL********/
// #define SAIB_MCLK_PORT  GPIOF
// #define SAIB_MCLK_PIN   GPIO_PIN_7

#define SAIB_MCLK_PORT  GPIOF
#define SAIB_MCLK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB FS PIN SEL********/
// #define SAIB_FS_PORT  GPIOF
// #define SAIB_FS_PIN   GPIO_PIN_9

#define SAIB_FS_PORT  GPIOF
#define SAIB_FS_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB SCK PIN SEL********/
// #define SAIB_SCK_PORT  GPIOF
// #define SAIB_SCK_PIN   GPIO_PIN_8

#define SAIB_SCK_PORT  GPIOF
#define SAIB_SCK_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB SD PIN SEL********/
#define SAIB_SD_PORT  GPIOE
#define SAIB_SD_PIN   GPIO_PIN_3

// #define SAIB_SD_PORT  GPIOF
// #define SAIB_SD_PIN   GPIO_PIN_6

// #define SAIB_SD_PORT  GPIOF
// #define SAIB_SD_PIN   GPIO_PIN_UNUSED
/********************************/

/********SAIB DMA SEL********/
#define SAIB_DMA_IRQ_PRIORITY 2

// #define SAIB_DMA_STREAM DMA2_Stream4
// #define SAIB_DMA_CH     DMA_CHANNEL_1
// #define SAIB_DMA_IRQ    DMA2_Stream4_IRQn  
// #define SAIB_DMA_HANDLER   0   

#define SAIB_DMA_STREAM DMA2_Stream5
#define SAIB_DMA_CH     DMA_CHANNEL_0
#define SAIB_DMA_IRQ    DMA2_Stream5_IRQn  
#define SAIB_DMA_HANDLER    1 
/********************************/ 

typedef struct
{
    SAI_HandleTypeDef handle;
    DMA_HandleTypeDef dma;
    DMA_HandleTypeDef *hdma_tx;
    DMA_HandleTypeDef *hdma_rx;
    cb_arg_t cb;
} sai_drv_t;

typedef struct 
{
    uint32_t freq;        
    uint32_t pll_n;     
    uint32_t pll_q;       
    uint32_t pll_divq;   
    uint32_t mck_div; 
} sai_psc_t;