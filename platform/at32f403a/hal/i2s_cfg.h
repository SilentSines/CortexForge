#ifndef I2S_CFG_H
#define I2S_CFG_H

#define I2S1_EN 1
#define I2S2_EN 1
#define I2S3_EN 1
#define I2S4_EN 1

/********I2S1 CONFIG********/
#define I2S1_PIN_GROUP_1   1	//WS(PA4) + CK(PA5) + SD(PA7) + MCK(PB0)
#define I2S1_PIN_GROUP_2   0 	//WS(PA15) + CK(PB3) + SD(PB5) + MCK(PB0)
#define I2S1_PIN_GROUP_3   0 	//WS(PA4) + CK(PA5) + SD(PA7) + MCK(PB6)
#define I2S1_PIN_GROUP_4   0 	//WS(PA15) + CK(PB3) + SD(PB5) + MCK(PB6)
/***************************/

/********I2S1 DMA TX SEL********/
#define I2S1_DMA_TX_EN 0
#define I2S1_DMA_TX_IRQ_PRIORITY 10

#define I2S1_DMA_TX     DMA1
#define I2S1_DMA_TX_CH  DMA1_CHANNEL3
#define I2S1_DMA_TX_IRQ DMA1_Channel3_IRQn    
#define I2S1_DMA_TX_FLEX FLEX_CHANNEL3
#define I2S1_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI1_TX
#define I2S1_DMA_TX_IRQHANDLER DMA1_Channel3_IRQHandler
/********************************/

/********I2S1 DMA RX SEL********/
#define I2S1_DMA_RX_EN 0
#define I2S1_DMA_RX_IRQ_PRIORITY 10

#define I2S1_DMA_RX     DMA1
#define I2S1_DMA_RX_CH  DMA1_CHANNEL2
#define I2S1_DMA_RX_IRQ DMA1_Channel2_IRQn 
#define I2S1_DMA_RX_FLEX FLEX_CHANNEL2
#define I2S1_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI1_RX
#define I2S1_DMA_RX_IRQHANDLER DMA1_Channel2_IRQHandler
/********************************/



/********I2S2 CONFIG********/
#define I2S2_PIN_GROUP_1   1	//MCK(PC6)
#define I2S2_PIN_GROUP_2   0 	//MCK(PA3)
#define I2S2_PIN_GROUP_3   0    //MCK(PA6)
/***************************/

/********I2S2 DMA TX SEL********/
#define I2S2_DMA_TX_EN 0
#define I2S2_DMA_TX_IRQ_PRIORITY 10

#define I2S2_DMA_TX     DMA1
#define I2S2_DMA_TX_CH  DMA1_CHANNEL5
#define I2S2_DMA_TX_IRQ DMA1_Channel5_IRQn    
#define I2S2_DMA_TX_FLEX FLEX_CHANNEL5
#define I2S2_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI2_TX 
#define I2S2_DMA_TX_IRQHANDLER DMA1_Channel5_IRQHandler
/********************************/

/********I2S2 DMA RX SEL********/
#define I2S2_DMA_RX_EN 0
#define I2S2_DMA_RX_IRQ_PRIORITY 10

#define I2S2_DMA_RX     DMA1
#define I2S2_DMA_RX_CH  DMA1_CHANNEL4
#define I2S2_DMA_RX_IRQ DMA1_Channel4_IRQn 
#define I2S2_DMA_RX_FLEX FLEX_CHANNEL4
#define I2S2_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI2_RX
#define I2S2_DMA_RX_IRQHANDLER DMA1_Channel4_IRQHandler
/********************************/



/********I2S3 CONFIG********/
#define I2S3_PIN_GROUP_1   0	//WS(PA15) + CK(PB3) + SDEXT(PB4) + SD(PB5) + MCK(PC7)
#define I2S3_PIN_GROUP_2   0 	//WS(PA4) + CK(PC10) + SDEXT(PC11) + SD(PC12) + MCK(PC7)
#define I2S3_PIN_GROUP_3   1 	//WS(PA15) + CK(PB3) + SDEXT(PB4) + SD(PB5) + MCK(PB10)
#define I2S3_PIN_GROUP_4   0 	//WS(PA4) + CK(PC10) + SDEXT(PC11) + SD(PC12) + MCK(PB10)
/***************************/

/********I2S3 DMA TX SEL********/
#define I2S3_DMA_TX_EN 0
#define I2S3_DMA_TX_IRQ_PRIORITY 10

#define I2S3_DMA_TX     DMA2
#define I2S3_DMA_TX_CH  DMA2_CHANNEL2
#define I2S3_DMA_TX_IRQ DMA2_Channel2_IRQn     
#define I2S3_DMA_TX_FLEX FLEX_CHANNEL2
#define I2S3_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI3_TX
#define I2S3_DMA_TX_IRQHANDLER DMA2_Channel2_IRQHandler 
/********************************/

/********I2S3 DMA RX SEL********/
#define I2S3_DMA_RX_EN 1
#define I2S3_DMA_RX_IRQ_PRIORITY 10

#define I2S3_DMA_RX     DMA2
#define I2S3_DMA_RX_CH  DMA2_CHANNEL1
#define I2S3_DMA_RX_IRQ DMA2_Channel1_IRQn 
#define I2S3_DMA_RX_FLEX FLEX_CHANNEL1
#define I2S3_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI3_RX
#define I2S3_DMA_RX_IRQHANDLER DMA2_Channel1_IRQHandler
/********************************/



/********I2S4 CONFIG********/
#define I2S4_PIN_GROUP_1   1	//WS(PE4) + CK(PE2) + SD(PE6) + MCK(PC8)
#define I2S4_PIN_GROUP_2   0 	//WS(PE12) + CK(PE11) + SD(PE14) + MCK(PC8)
#define I2S4_PIN_GROUP_3   0 	//WS(PB6) + CK(PB7) + SD(PB9) + MCK(PC8)
#define I2S4_PIN_GROUP_4   0    //WS(PB6) + CK(PB7) + SD(PB9) + MCK(PA10)
/***************************/

/********I2S4 DMA TX SEL********/
#define I2S4_DMA_TX_EN 0
#define I2S4_DMA_TX_IRQ_PRIORITY 10

#define I2S4_DMA_TX     DMA2
#define I2S4_DMA_TX_CH  DMA2_CHANNEL4
#define I2S4_DMA_TX_IRQ DMA2_Channel4_5_IRQn     
#define I2S4_DMA_TX_FLEX FLEX_CHANNEL4
#define I2S4_DMA_TX_FLEX_REQ DMA_FLEXIBLE_SPI4_TX
#define I2S4_DMA_TX_IRQHANDLER DMA2_Channel4_5_IRQHandler 
/********************************/

/********I2S4 DMA RX SEL********/
#define I2S4_DMA_RX_EN 0
#define I2S4_DMA_RX_IRQ_PRIORITY 10

#define I2S4_DMA_RX     DMA2
#define I2S4_DMA_RX_CH  DMA2_CHANNEL3
#define I2S4_DMA_RX_IRQ DMA2_Channel3_IRQn 
#define I2S4_DMA_RX_FLEX FLEX_CHANNEL3
#define I2S4_DMA_RX_FLEX_REQ DMA_FLEXIBLE_SPI4_RX
#define I2S4_DMA_RX_IRQHANDLER DMA2_Channel3_IRQHandler
/********************************/

typedef struct
{
    i2s_mode_t mode;
    i2s_data_channel_format_type format;
    bool mck_en;
    dma_channel_type *dma_tx;
    dma_channel_type *dma_rx;
    bool dbuf_en;
    cb_arg_t func;
} i2s_drv_t;

#endif