#ifndef SPI_CFG_H
#define SPI_CFG_H

#define SPI1_EN 1
#define SPI2_EN 0
#define SPI3_EN 0 
#define SPI4_EN 0 

/********SPI1 CONFIG********/
#define SPI1_PIN_GROUP_1   1	//SCK(PA5) + MISO(PA6) + MOSI(PA7) + CS(PA4) 
#define SPI1_PIN_GROUP_2   0 	//SCK(PB3) + MISO(PB4) + MOSI(PB5) + CS(PA15)
#define SPI1_MISO_UNUSED   1
/***************************/

/********SPI1 DMA TX SEL********/
#define SPI1_DMA_TX_EN  1
#define SPI1_DMA_TX_IRQ_PRIORITY 10

#define SPI1_DMA_TX     DMA1
#define SPI1_DMA_TX_CH  DMA1_CHANNEL3
#define SPI1_DMA_TX_IRQ DMA1_Channel3_IRQn    
#define SPI1_DMA_TX_FLEX FLEX_CHANNEL3
#define SPI1_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI1_TX
#define SPI1_DMA_TX_IRQHANDLER DMA1_Channel3_IRQHandler
/********************************/

/********SPI1 DMA RX SEL********/
#define SPI1_DMA_RX_EN  0
#define SPI1_DMA_RX_IRQ_PRIORITY 10

#define SPI1_DMA_RX     DMA1
#define SPI1_DMA_RX_CH  DMA1_CHANNEL2
#define SPI1_DMA_RX_IRQ DMA1_Channel2_IRQn 
#define SPI1_DMA_RX_FLEX FLEX_CHANNEL2
#define SPI1_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI1_RX
#define SPI1_DMA_RX_IRQHANDLER DMA1_Channel2_IRQHandler
/********************************/



/********SPI2 CONFIG********/
#define SPI2_PIN_GROUP_1   1	//SCK(PB13) + MISO(PB14) + MOSI(PB15) + CS(PB12) 
#define SPI2_MISO_UNUSED   0
/***************************/

/********SPI2 DMA TX SEL********/
#define SPI2_DMA_TX_EN  0
#define SPI2_DMA_TX_IRQ_PRIORITY 10

#define SPI2_DMA_TX     DMA1
#define SPI2_DMA_TX_CH  DMA1_CHANNEL5
#define SPI2_DMA_TX_IRQ DMA1_Channel5_IRQn    
#define SPI2_DMA_TX_FLEX FLEX_CHANNEL5
#define SPI2_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI2_TX 
#define SPI2_DMA_TX_IRQHANDLER DMA1_Channel5_IRQHandler
/********************************/

/********SPI2 DMA RX SEL********/
#define SPI2_DMA_RX_EN  0
#define SPI2_DMA_RX_IRQ_PRIORITY 10

#define SPI2_DMA_RX     DMA1
#define SPI2_DMA_RX_CH  DMA1_CHANNEL4
#define SPI2_DMA_RX_IRQ DMA1_Channel4_IRQn 
#define SPI2_DMA_RX_FLEX FLEX_CHANNEL4
#define SPI2_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI2_RX
#define SPI2_DMA_RX_IRQHANDLER DMA1_Channel4_IRQHandler
/********************************/



/********SPI3 CONFIG********/
#define SPI3_PIN_GROUP_1   1	//SCK(PB3) + MISO(PB4) + MOSI(PB5) + CS(PA15)
#define SPI3_PIN_GROUP_2   0 	//SCK(PC10) + MISO(PC11) + MOSI(PC12) + CS(PA4)  
#define SPI3_MISO_UNUSED   0
/***************************/

/********SPI3 DMA TX SEL********/
#define SPI3_DMA_TX_EN  0
#define SPI3_DMA_TX_IRQ_PRIORITY 10

#define SPI3_DMA_TX     DMA2
#define SPI3_DMA_TX_CH  DMA2_CHANNEL2
#define SPI3_DMA_TX_IRQ DMA2_Channel2_IRQn     
#define SPI3_DMA_TX_FLEX FLEX_CHANNEL2
#define SPI3_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI3_TX
#define SPI3_DMA_TX_IRQHANDLER DMA2_Channel2_IRQHandler 
/********************************/

/********SPI3 DMA RX SEL********/
#define SPI3_DMA_RX_EN  0
#define SPI3_DMA_RX_IRQ_PRIORITY 10

#define SPI3_DMA_RX     DMA2
#define SPI3_DMA_RX_CH  DMA2_CHANNEL1
#define SPI3_DMA_RX_IRQ DMA2_Channel1_IRQn 
#define SPI3_DMA_RX_FLEX FLEX_CHANNEL1
#define SPI3_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI3_RX
#define SPI3_DMA_RX_IRQHANDLER DMA2_Channel1_IRQHandler
/********************************/



/********SPI4 CONFIG********/
#define SPI4_PIN_GROUP_1   1	//SCK(PE2) + MISO(PE5) + MOSI(PE6) + CS(PE4)   
#define SPI4_PIN_GROUP_2   0 	//SCK(PE11) + MISO(PE13) + MOSI(PE14) + CS(PE12) 
#define SPI4_PIN_GROUP_3   0 	//SCK(PB7) + MISO(PB8) + MOSI(PB9) + CS(PB6)    
#define SPI4_MISO_UNUSED   0
/***************************/

/********SPI4 DMA TX SEL********/
#define SPI4_DMA_TX_EN  0
#define SPI4_DMA_TX_IRQ_PRIORITY 10

#define SPI4_DMA_TX     DMA2
#define SPI4_DMA_TX_CH  DMA2_CHANNEL4
#define SPI4_DMA_TX_IRQ DMA2_Channel4_5_IRQn     
#define SPI4_DMA_TX_FLEX FLEX_CHANNEL4
#define SPI4_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI4_TX
#define SPI4_DMA_TX_IRQHANDLER DMA2_Channel4_5_IRQHandler 
/********************************/

/********SPI4 DMA RX SEL********/
#define SPI4_DMA_RX_EN  0
#define SPI4_DMA_RX_IRQ_PRIORITY 10

#define SPI4_DMA_RX     DMA2
#define SPI4_DMA_RX_CH  DMA2_CHANNEL3
#define SPI4_DMA_RX_IRQ DMA2_Channel3_IRQn 
#define SPI4_DMA_RX_FLEX FLEX_CHANNEL3
#define SPI4_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI4_RX
#define SPI4_DMA_RX_IRQHANDLER DMA2_Channel3_IRQHandler
/********************************/

typedef struct
{
    spi_clock_polarity_type cpol;
    spi_clock_phase_type cpha;
} spi_mode_t;

typedef struct
{
    spi_frame_bit_num_type bit_num;
    dma_channel_type *dma_tx;
    dma_channel_type *dma_rx;
    cb_t func;
    __IO bool busy;
} spi_drv_t;

#endif