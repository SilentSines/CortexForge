#ifndef OLED_CFG_H
#define OLED_CFG_H

#define OLED_WIDTH   128
#define OLED_HEIGHT  64

#define OLED_RST_PIN   GPIO_PB0
#define OLED_RST_H()   GPIO_OUT_H(OLED_RST_PIN)
#define OLED_RST_L()   GPIO_OUT_L(OLED_RST_PIN)

#define OLED_USE_DMA  1

#define OLED_USE_SPI  1

#if OLED_USE_SPI == 1
    #define OLED_SPI    SPI1
    #define OLED_DC_PIN  GPIO_PA6
    #define OLED_DC_H()  GPIO_OUT_H(OLED_DC_PIN)
    #define OLED_DC_L()  GPIO_OUT_L(OLED_DC_PIN)
    #define OLED_CS_PIN  GPIO_PA4
    #define OLED_CS_H()  GPIO_OUT_H(OLED_CS_PIN)
    #define OLED_CS_L()  GPIO_OUT_L(OLED_CS_PIN)
#endif

typedef enum
{
    OLED_L2R_U2D,
    OLED_L2R_D2U,
    OLED_R2L_U2D,
    OLED_R2L_D2U
} oled_dir_t;

#endif