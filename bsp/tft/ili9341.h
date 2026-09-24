#ifndef ILI9341_H
#define ILI9341_H

#define TFT_SOFTWAVE_RESET() {tft_write_cmd(0x01); delay_ms(150);}
#define TFT_SET_MADCTL(x) {tft_write_cmd(0x36); tft_write_param((uint8_t *)&dir_param[x], 1);}
#define TFT_SET_COLUM(x1, x2) {tft_write_cmd(0x2A); tft_write_param((uint8_t[]){x1 >> 8, x1 & 0xFF, x2 >> 8, x2 & 0xFF}, 4);}
#define TFT_SET_ROW(y1, y2) {tft_write_cmd(0x2B); tft_write_param((uint8_t[]){y1 >> 8, y1 & 0xFF, y2 >> 8, y2 & 0xFF}, 4);}
#define TFT_SET_CURSOR(x, y) {tft_write_cmd(0x2A); tft_write_param((uint8_t[]){x >> 8, x & 0xFF}, 2); tft_write_cmd(0x2B); tft_write_param((uint8_t[]){y >> 8, y & 0xFF}, 2);}
#define TFT_WRITE_GRAM() tft_write_cmd(0x2C)

static const tft_init_t tft_init_table[] =
{
    {0xCF, {0x00, 0xC1, 0X30}, 3, 0},
    {0xED, {0x64, 0x03, 0X12, 0X81}, 4, 0},
    {0xE8, {0x85, 0x10, 0x7A}, 3, 0},
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5, 0},
    {0xF7, {0x20}, 1, 0},
    {0xEA, {0x00, 0x00}, 2, 0},
    //Power control  VRH[5:0] 
    {0xC0, {0x1B}, 1, 0},
    //Power control  SAP[2:0];BT[3:0] 
    {0xC1, {0x01}, 1, 0},
    //VCM control 
    {0xC5, {0x30, 0x30}, 2, 0},
    //VCM control2 
    {0xC7, {0XB7}, 1, 0},
    {0x3A, {0x55}, 1, 0},
    {0xB1, {0x00, 0x1A}, 2, 0},
    // Display Function Control 
    {0xB6, {0x0A, 0xA2}, 2, 0},
    // 3Gamma Function Disable 
    {0xF2, {0x00}, 1, 0},
    //Gamma curve selected 
    {0x26, {0x01}, 1, 0},
    //Set Gamma 
    {0xE0, {0x0F, 0x2A, 0x28, 0x08, 0x0E, 0x08, 0x54, 0XA9, 0x43, 0x0A, 0x0F, 0x00, 0x00, 0x00, 0x00}, 15, 0},
    //Set Gamma 
    {0XE1, {0x00, 0x15, 0x17, 0x07, 0x11, 0x06, 0x2B, 0x56, 0x3C, 0x05, 0x10, 0x0F, 0x3F, 0x3F, 0x0F}, 15, 0},
    //Exit Sleep
    {0x11, {0}, 0, 120},
    //display on	
    {0x29, {0}, 0, 0},
    {0, {0}, 0, 0xFF},

};

static const uint8_t dir_param[] = {0x08, 0x88, 0x48, 0xC8, 0x28, 0x68, 0xA8, 0xE8};

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
