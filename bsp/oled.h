#ifndef OLED_H
#define OLED_H

#define OLED_BLACK  0x00
#define OLED_WHITE  0x01

void oled_init(void);
void oled_set_bright(uint8_t value);
void oled_on(void);
void oled_off(void);
void oled_set_win(uint8_t page, uint8_t col, uint8_t page_len, uint8_t col_len);
void oled_write_ddram(const uint8_t *buf, uint32_t size, cb_t func);
void oled_clear(uint8_t color);

#endif