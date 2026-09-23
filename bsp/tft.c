#include "bsp.h"

#include "tft/tft_cfg.h"
#include "tft/st7789p.h"

static tft_win_t tft_win;

static void tft_write_cmd(uint8_t cmd)
{
#if TFT_USE_SPI == 1
    TFT_DC_L();
    spi_set_data_size(TFT_SPI, SPI_FRAME_8BIT);
    spi_send_poll(TFT_SPI, &cmd, 1);
#endif
}

static void tft_write_param(const uint8_t *param, uint32_t size)
{
#if TFT_USE_SPI == 1
    TFT_DC_H();
    spi_send_poll(TFT_SPI, param, size);
#endif
}

static void tft_init_port(void)
{
#if TFT_USE_SPI == 1
    gpio_pin_init(TFT_DC_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);
    gpio_pin_init(TFT_CS_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);
    TFT_DC_L();
    TFT_CS_L();
#endif
}

static void tft_init_back_light(void)
{
#if TFT_BL_USE_GPIO
    gpio_pin_init(TFT_BL_PIN, GPIO_OUT_PP_LOW, GPIO_DRIVE_STRENGTH_MODERATE);
#elif TFT_BL_USE_PWM
    pwm_init(TFT_BL_PWM_CH, TFT_BL_PWM_RES, TFT_BL_PWM_FREQ, TFT_BL_PWM_POLARITY);
    pwm_start(TFT_BL_PWM_CH);
#endif
}

static void tft_init_var(void)
{
    uint8_t i = 0;

    while (tft_init_table[i].delay != 0xFF)
    {
        tft_write_cmd(tft_init_table[i].cmd);
        if (tft_init_table[i].size)
        {
            tft_write_param(tft_init_table[i].buf, tft_init_table[i].size);
        }
        if (tft_init_table[i].delay)
        {
            delay_ms(tft_init_table[i].delay);
        }
        i++;
    }
}

static void tft_set_dir(tft_dir_t dir)
{
    TFT_SET_MADCTL(dir);
    memcpy(&tft_win, &tft_cfg_table[dir], sizeof(tft_win));
}

void tft_init(void)
{
    tft_init_port();
    tft_init_back_light();
#ifdef TFT_RST_PIN
    gpio_pin_init(TFT_RST_PIN, GPIO_OUT_PP, GPIO_DRIVE_STRENGTH_MODERATE);
    TFT_RST_H();
    delay_ms(10);
    TFT_RST_L();
    delay_ms(20);
    TFT_RST_H();
    delay_ms(150);
#else
    TFT_SOFTWAVE_RESET();
#endif
    tft_init_var();
    tft_set_dir(TFT_U2D_R2L);

    tft_clear(TFT_BLACK);
    delay_ms(50);
}

void tft_set_back_light(uint16_t value)
{
#if TFT_BL_USE_GPIO
    if(value)
    {
        TFT_BL_H();
    }
    else
    {
        TFT_BL_L();
    }
#elif TFT_BL_USE_PWM
    uint32_t duty = (value >= 100) ? TFT_BL_PWM_RES : (value * TFT_BL_PWM_RES / 100);
    pwm_set_duty(TFT_BL_PWM_CH, duty);
#endif
}

void tft_set_win(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    int16_t x1, y1, x2, y2;

    x1 = tft_win.x1 + x;
    y1 = tft_win.y1 + y;
    x2 = x1 + w - 1;
    y2 = y1 + h - 1;
    TFT_SET_COLUM(x1, x2);
    TFT_SET_ROW(y1, y2);
    TFT_WRITE_GRAM();
}

void tft_write_gram(const uint16_t *buf, uint32_t size, cb_t func)
{
#if TFT_USE_SPI == 1
    TFT_DC_H();
    spi_set_data_size(TFT_SPI, SPI_FRAME_16BIT);
#if TFT_USE_DMA
    spi_send_dma(TFT_SPI, buf, size, func);
    if (func == NULL)
    {
        while (spi_is_busy(TFT_SPI));
    }
#else
    UNUSED(func);
    spi_send_poll(TFT_SPI, buf, size);
#endif

#endif
}

void tft_clear(uint16_t color)
{
    uint32_t i;
    uint32_t size;
    uint32_t rem, block;
    uint16_t buf[512] __attribute__((aligned(4))); 

    tft_set_win(0, 0, tft_win.w, tft_win.h);

    size = (uint32_t)tft_win.h * tft_win.w;
    rem = size % 512;
    block = size / 512;
    for (i = 0; i < 512; i++)
    {
        buf[i] = color;
    }
    for (i = 0; i < block; i++)
    {
        tft_write_gram(buf, 512, NULL);
    }
    if (rem)
    {
        tft_write_gram(buf, rem, NULL);
    }
}

bool tft_rect_inside(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    if (x < tft_win.x1 || y < tft_win.y1) 
    {
        return false;
    }
    if (((tft_win.x1 + x + w - 1) > tft_win.x2) || ((tft_win.y1 + y + h - 1) > tft_win.y2)) 
    {
        return false;
    }
    return true;
}