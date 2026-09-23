#ifndef ST7789P_H
#define ST7789P_H

#define TFT_SOFTWAVE_RESET() {tft_write_cmd(0x01); delay_ms(150);}
#define TFT_SET_MADCTL(x) {tft_write_cmd(0x36); tft_write_param((uint8_t *)&dir_param[x], 1);}
#define TFT_SET_COLUM(x1, x2) {tft_write_cmd(0x2A); tft_write_param((uint8_t[]){x1 >> 8, x1 & 0xFF, x2 >> 8, x2 & 0xFF}, 4);}
#define TFT_SET_ROW(y1, y2) {tft_write_cmd(0x2B); tft_write_param((uint8_t[]){y1 >> 8, y1 & 0xFF, y2 >> 8, y2 & 0xFF}, 4);}
#define TFT_SET_CURSOR(x, y) {tft_write_cmd(0x2A); tft_write_param((uint8_t[]){x >> 8, x & 0xFF}, 2); tft_write_cmd(0x2B); tft_write_param((uint8_t[]){y >> 8, y & 0xFF}, 2);}
#define TFT_WRITE_GRAM() tft_write_cmd(0x2C)

#define LITTLE_ENDIAN_EN 0

static const tft_init_t tft_init_table[] =
{
    /* Sleep Out */
    {0x11, {0}, 0, 120},
    /* Interface Pixel Format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1, 0},
    /* RAM control */
#if LITTLE_ENDIAN_EN
    {0xB0, {0x00, 0xFC}, 2, 0},
#else
    {0xB0, {0x00, 0xF4}, 2, 0},
#endif
    /* Porch Setting */
    {0xB2, {0x0C, 0x0C, 0x00, 0x33, 0x33}, 5, 0},
    /* Gate Control, Vgh=13.65V, Vgl=-10.43V */
    {0xB7, {0x56}, 1, 0},
    /* VCOM Setting, VCOM=1.175V */
    {0xBB, {0x1D}, 1, 0},
     /* LCM Control, XOR: BGR, MX, MH */
    {0xC0, {0x2C}, 1, 0},
    /* VDV and VRH Command Enable, enable=1 */
    {0xC2, {0x01}, 1, 0},
    /* VRH Set, Vap=4.4+... */
    {0xC3, {0x0F}, 1, 0},
    /* Frame Rate Control, 60Hz, inversion=0 */
    {0xC6, {0x0F}, 1, 0},
    /* Power Control 1, AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V */
    {0xD0, {0xA7}, 1, 0},
    /* Power Control 1, AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V */
    {0xD0, {0xA4, 0xA1}, 2, 0},
    /* Gate Output(Sleep In Mode), GND */
    {0xD6, {0xA1}, 1, 0},
    /* Positive Voltage Gamma Control */
    {0xE0, {0xF0, 0x02, 0x07, 0x05, 0x06, 0x14, 0x2F, 0x54, 0x46, 0x38, 0x13, 0x11, 0x2E, 0x35}, 14, 0},
    /* Negative Voltage Gamma Control */
    {0xE1, {0xF0, 0x08, 0x0C, 0x0C, 0x09, 0x05, 0x2F, 0x43, 0x46, 0x36, 0x10, 0x12, 0x2C, 0x32}, 14, 0},
    /* Display Inversion On */
    {0x21, {0}, 0, 0},
    /* Display On */
    {0x29, {0}, 0, 0},
    /* Memory Write */
    {0x2C, {0}, 0, 0},
    
    {0, {0}, 0, 0xFF}
};

static const uint8_t dir_param[] = {0x00, 0x80, 0x40, 0xC0, 0x20, 0x60, 0xA0, 0xE0};

static const tft_win_t tft_cfg_table[] = 
{
#if (TFT_WIDTH_NATIVE == 240 && TFT_HEIGHT_NATIVE == 320)
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 0, 0 + TFT_WIDTH_NATIVE - 1, 0 + TFT_HEIGHT_NATIVE - 1},
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 0, 0 + TFT_WIDTH_NATIVE - 1, 0 + TFT_HEIGHT_NATIVE - 1},
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 0, 0 + TFT_WIDTH_NATIVE - 1, 0 + TFT_HEIGHT_NATIVE - 1},
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 0, 0 + TFT_WIDTH_NATIVE - 1, 0 + TFT_HEIGHT_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 0, 0, 0 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 0, 0, 0 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 0, 0, 0 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 0, 0, 0 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1}
#endif
};

#endif