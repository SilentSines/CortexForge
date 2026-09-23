#ifndef TFT_CFG_H
#define TFT_CFG_H

#define TFT_WIDTH_NATIVE  240
#define TFT_HEIGHT_NATIVE 320

#define TFT_RST_PIN GPIO_PB0
#define TFT_RST_H() GPIO_OUT_H(TFT_RST_PIN)
#define TFT_RST_L() GPIO_OUT_L(TFT_RST_PIN)

#define TFT_USE_DMA 1

#define TFT_USE_SPI 1

#if TFT_USE_SPI == 1
    #define TFT_SPI SPI1
    #define TFT_DC_PIN GPIO_PA6
    #define TFT_DC_H() GPIO_OUT_H(TFT_DC_PIN)
    #define TFT_DC_L() GPIO_OUT_L(TFT_DC_PIN)
    #define TFT_CS_PIN GPIO_PA4
    #define TFT_CS_H() GPIO_OUT_H(TFT_CS_PIN)
    #define TFT_CS_L() GPIO_OUT_L(TFT_CS_PIN)	
#endif

#define TFT_BL_USE_GPIO 1
#define TFT_BL_USE_PWM 0

#if TFT_BL_USE_GPIO == 1
    #define TFT_BL_PIN GPIO_PA3
    #define TFT_BL_H() GPIO_OUT_H(TFT_BL_PIN)
    #define TFT_BL_L() GPIO_OUT_L(TFT_BL_PIN)	
#elif TFT_BL_USE_PWM == 1
    #define TFT_BL_PWM_CH TMR9_PWM_CH2_PA3
    #define TFT_BL_PWM_FREQ 50000
    #define TFT_BL_PWM_RES  720
    #define TFT_BL_PWM_POLARITY PWM_POLARITY_HIGH
#endif

typedef struct 
{
    uint16_t w;
    uint16_t h;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} tft_win_t;

typedef struct 
{
    uint8_t cmd;
    uint8_t buf[16];
    uint8_t size;
    uint8_t delay;
} tft_init_t;

typedef enum
{
    TFT_L2R_U2D,  
    TFT_L2R_D2U,  
    TFT_R2L_U2D,  
    TFT_R2L_D2U,  
    TFT_U2D_L2R,  
    TFT_U2D_R2L,  
    TFT_D2U_L2R,  
    TFT_D2U_R2L,  
} tft_dir_t;

#endif
