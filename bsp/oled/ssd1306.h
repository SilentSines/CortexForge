#ifndef SSD1306_H
#define SSD1306_H

#define OLED_SET_PAGE(p1, p2)   {oled_write_cmd(0x22); oled_write_cmd(p1); oled_write_cmd(p2);}
#define OLED_SET_COLUM(c1, c2)  {oled_write_cmd(0x21); oled_write_cmd(c1); oled_write_cmd(c2);}
#define OLED_SET_DIR(v1 ,v2)    {oled_write_cmd(v1); oled_write_cmd(v2);}
#define OLED_SET_BRIGHT(v)      {oled_write_cmd(0x81); oled_write_cmd(v);}
#define OLED_ON()               {oled_write_cmd(0x8D); oled_write_cmd(0x14); oled_write_cmd(0xAF);}
#define OLED_OFF()              {oled_write_cmd(0x8D); oled_write_cmd(0x10); oled_write_cmd(0xAE);}

#define HOR_MODE     0x00
#define VER_MODE     0x01
#define PAGE_MODE    0x02

static const uint8_t oled_init_table[] =
{
    0xAE,           // Display OFF
    0x00, 0x10,     // Set column address
    0x40,           // Set Display Start Line address
    0x81, 0x00,     // Set Contrast
    0xA6,           // Normal Display (not inverted)
    0xA8, 0x3F,     // Set Multiplex Ratio = 63 (64 rows)
    0xD3, 0x00,     // Set Display Offset = 0
    0xD5, 0x80,     // Set Clock Divide Ratio / Oscillator Frequency
    0xD9, 0xF1,     // Set Pre-charge Period
    0xDA, 0x12,     // Set COM Pins Hardware Configuration
    0xDB, 0x30,     // Set VCOMH Deselect Level
    0x20, HOR_MODE, // Set Memory Addressing Mode (Horizontal)
    0xA4,
};

static const uint8_t dir_param[][2] =
{
    {0xA1, 0xC8},
    {0xA1, 0xC0},
    {0xA0, 0xC8},
    {0xA0, 0xC0},
};

#endif