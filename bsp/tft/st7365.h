#ifndef ST7365_H
#define ST7365_H

#define TFT_SOFTWAVE_RESET() {tft_write_cmd(0x01); delay_ms(150);}
#define TFT_SET_MADCTL(x) {tft_write_cmd(0x36); tft_write_param((uint8_t *)&dir_param[x], 1);}
#define TFT_SET_COLUM(x1, x2) {tft_write_cmd(0x2A); tft_write_param((uint8_t[]){x1 >> 8, x1 & 0xFF, x2 >> 8, x2 & 0xFF}, 4);}
#define TFT_SET_ROW(y1, y2) {tft_write_cmd(0x2B); tft_write_param((uint8_t[]){y1 >> 8, y1 & 0xFF, y2 >> 8, y2 & 0xFF}, 4);}
#define TFT_SET_CURSOR(x, y) {tft_write_cmd(0x2A); tft_write_param((uint8_t[]){x >> 8, x & 0xFF}, 2); tft_write_cmd(0x2B); tft_write_param((uint8_t[]){y >> 8, y & 0xFF}, 2);}
#define TFT_WRITE_GRAM() tft_write_cmd(0x2C)

static const tft_init_t tft_init_table[] =
{
    {0x3A, {0x55}, 1, 0},
    {0xF0, {0xC3}, 1, 0},
    {0xF0, {0x96}, 1, 0},
    {0xB4, {0x01}, 1, 0},
    {0xB7, {0xC6}, 1, 0},
    {0xC0, {0x80, 0x45}, 2, 0},
    {0xC1, {0x13}, 1, 0},
    {0xC2, {0xA7}, 1, 0},
    {0xC5, {0x0A}, 1, 0},
    {0xE8, {0x40, 0x8A, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33}, 8, 0},
    {0xE0, {0xD0, 0x08, 0x0F, 0x06, 0x06, 0x33, 0x30, 0x33, 0x47, 0x17, 0x13, 0x13, 0x2B, 0x31}, 14, 0},
    {0xE1, {0xD0, 0x0A, 0x11, 0x0B, 0x09, 0x07, 0x2F, 0x33, 0x47, 0x38, 0x15, 0x16, 0x2C, 0x32}, 14, 0},
    {0xF0, {0x3C}, 1, 0},
    {0xF0, {0x69}, 1, 0},
    {0x3A, {0x05}, 1, 0},
    {0x21, {0}, 0, 0},
    {0x11, {0}, 0, 480},
    {0x29, {0}, 0, 80},
    {0, {0}, 0, 0xFF},
};

static const uint8_t dir_param[] = {0x48, 0xC8, 0x08, 0x88, 0x68, 0x28, 0xE8, 0xA8};

static const tft_win_t tft_cfg_table[] = 
{
#if (TFT_WIDTH_NATIVE == 320 && TFT_HEIGHT_NATIVE == 320)
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 0, 0 + TFT_WIDTH_NATIVE - 1, 0 + TFT_HEIGHT_NATIVE - 1},
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 80, 0 + TFT_WIDTH_NATIVE - 1, 80 + TFT_HEIGHT_NATIVE - 1},
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 0, 0 + TFT_WIDTH_NATIVE - 1, 0 + TFT_HEIGHT_NATIVE - 1},
    {TFT_WIDTH_NATIVE, TFT_HEIGHT_NATIVE, 0, 80, 0 + TFT_WIDTH_NATIVE - 1, 80 + TFT_HEIGHT_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 0, 0, 0 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 0, 0, 0 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 80, 0, 80 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1},
    {TFT_HEIGHT_NATIVE, TFT_WIDTH_NATIVE, 80, 0, 80 + TFT_HEIGHT_NATIVE - 1, 0 + TFT_WIDTH_NATIVE - 1}
#endif
};

#endif
