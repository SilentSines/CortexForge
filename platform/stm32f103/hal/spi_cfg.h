#ifndef SPI_CFG_H
#define SPI_CFG_H

#define SPI1_EN 1
#define SPI2_EN 1
#define SPI3_EN 1 

/********SPI1 CONFIG********/
#define SPI1_PIN_GROUP_1   0	//SCK(PA5) + MISO(PA6) + MOSI(PA7) + CS(PA4) 
#define SPI1_PIN_GROUP_2   1 	//SCK(PB3) + MISO(PB4) + MOSI(PB5)+ CS(PA15)
#define SPI1_MISO_UNUSED   1
/***************************/

/********SPI1 DMA TX SEL********/
#define SPI1_DMA_TX_EN  1
#define SPI1_DMA_TX_IRQ_PRIORITY 10

#define SPI1_DMA_TX_CH          DMA1_Channel3
#define SPI1_DMA_TX_IRQ         DMA1_Channel3_IRQn   
#define SPI1_DMA_TX_IRQHANDLER  DMA1_Channel3_IRQHandler  
/********************************/

/********SPI1 DMA RX SEL********/
#define SPI1_DMA_RX_EN  1
#define SPI1_DMA_RX_IRQ_PRIORITY 10

#define SPI1_DMA_RX_CH          DMA1_Channel2
#define SPI1_DMA_RX_IRQ         DMA1_Channel2_IRQn
#define SPI1_DMA_RX_IRQHANDLER  DMA1_Channel2_IRQHandler
/********************************/



/********SPI2 CONFIG********/
#define SPI2_PIN_GROUP_1   1	//SCK(PB13) + MISO(PB14) + MOSI(PB15) + CS(PB12) 
#define SPI2_MISO_UNUSED   0
/***************************/

/********SPI2 DMA TX SEL********/
#define SPI2_DMA_TX_EN  1
#define SPI2_DMA_TX_IRQ_PRIORITY 10

#define SPI2_DMA_TX_CH          DMA1_Channel5
#define SPI2_DMA_TX_IRQ         DMA1_Channel5_IRQn    
#define SPI2_DMA_TX_IRQHANDLER  DMA1_Channel5_IRQHandler   
/********************************/

/********SPI2 DMA RX SEL********/
#define SPI2_DMA_RX_EN  1
#define SPI2_DMA_RX_IRQ_PRIORITY 10

#define SPI2_DMA_RX_CH          DMA1_Channel4
#define SPI2_DMA_RX_IRQ         DMA1_Channel4_IRQn
#define SPI2_DMA_RX_IRQHANDLER  DMA1_Channel4_IRQHandler
/********************************/



/********SPI3 CONFIG********/
#define SPI3_PIN_GROUP_1   1	//SCK(PB3) + MISO(PB4) + MOSI(PB5) + CS(PA15)
#define SPI3_MISO_UNUSED   0
/***************************/

/********SPI3 DMA TX SEL********/
#define SPI3_DMA_TX_EN  1
#define SPI3_DMA_TX_IRQ_PRIORITY 10

#define SPI3_DMA_TX_CH          DMA2_Channel2
#define SPI3_DMA_TX_IRQ         DMA2_Channel2_IRQn   
#define SPI3_DMA_TX_IRQHANDLER  DMA2_Channel2_IRQHandler     
/********************************/

/********SPI3 DMA RX SEL********/
#define SPI3_DMA_RX_EN  1
#define SPI3_DMA_RX_IRQ_PRIORITY 10

#define SPI3_DMA_RX_CH          DMA2_Channel1
#define SPI3_DMA_RX_IRQ         DMA2_Channel1_IRQn
#define SPI3_DMA_RX_IRQHANDLER  DMA2_Channel1_IRQHandler
/********************************/

typedef struct
{
    uint32_t cpol;
    uint32_t cpha;
} spi_mode_t;

typedef struct
{
    SPI_HandleTypeDef handle;
    DMA_HandleTypeDef *hdma_tx;
    DMA_HandleTypeDef *hdma_rx;
    cb_t func;
} spi_drv_t;

#endif