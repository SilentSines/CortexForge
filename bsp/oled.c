#include "bsp.h"

#include "oled/oled_cfg.h"
#include "oled/ssd1306.h"

static void oled_write_cmd(uint8_t cmd)
{
#if OLED_USE_SPI == 1
    OLED_DC_L();
    spi_send_poll(OLED_SPI, &cmd, 1);
#endif
}

static void oled_init_port(void)
{
#if OLED_USE_SPI == 1
    gpio_pin_init(OLED_DC_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);
    gpio_pin_init(OLED_CS_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);
    OLED_DC_L();
    OLED_CS_L();
#endif
}

static void oled_init_var(void)
{
    uint8_t i;

    for (i = 0; i < sizeof(oled_init_table); i++)
    {
        oled_write_cmd(oled_init_table[i]);
    }
}

static void oled_set_dir(oled_dir_t dir)
{
    OLED_SET_DIR(dir_param[dir][0], dir_param[dir][1]);
}

void oled_init(void)
{
    oled_init_port();

    gpio_pin_init(OLED_RST_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);
    OLED_RST_H();
    delay_ms(100);
    OLED_RST_L();
    delay_ms(100);
    OLED_RST_H();
    delay_ms(100);
    oled_init_var();
    oled_set_dir(OLED_L2R_U2D);

    oled_clear(OLED_BLACK);
    delay_ms(50);

    OLED_ON();
}

void oled_set_bright(uint8_t value)
{
    OLED_SET_BRIGHT(value);
}

void oled_on(void)
{
    OLED_ON();
}

void oled_off(void)
{
    OLED_OFF();
}

void oled_set_win(uint8_t page, uint8_t col, uint8_t page_len, uint8_t col_len)
{
    uint8_t p1, p2, c1, c2;

    p1 = page;
    c1 = col;
    p2 = page + page_len - 1;
    c2 = col + col_len - 1;
    OLED_SET_PAGE(p1, p2);
    OLED_SET_COLUM(c1, c2);
}

void oled_write_ddram(const uint8_t *buf, uint32_t size, cb_t func)
{
#if OLED_USE_SPI == 1
    OLED_DC_H();
#if OLED_USE_DMA
    spi_send_dma(OLED_SPI, buf, size, func);
    if (func == NULL)
    {    
        while (spi_is_busy(OLED_SPI));
    }
#else
    UNUSED(func);
    spi_send_poll(OLED_SPI, buf, size);
#endif

#endif
}

void oled_clear(uint8_t color)
{
    uint32_t i;
    uint32_t size;
    uint32_t rem, block;
    uint8_t data = color ? 0xFF : 0x00;
    uint8_t buf[128];

    oled_set_win(0, 0, OLED_HEIGHT >> 3, OLED_WIDTH);

    size = (OLED_HEIGHT >> 3) * OLED_WIDTH;
    rem = size % 128;
    block = size / 128;
    for (i = 0; i < 128; i++)
    {
        buf[i] = data;
    }
    for (i = 0; i < block; i++)
    {
        oled_write_ddram(buf, 128, NULL);
    }
    if (rem)
    {
        oled_write_ddram(buf, rem, NULL);
    }
}