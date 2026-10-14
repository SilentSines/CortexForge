#ifndef I2S_H
#define I2S_H

typedef enum
{
    I2S_UNIT_1,
    I2S_UNIT_2,
    I2S_UNIT_3,
    I2S_UNIT_4,
} i2s_unit_t;

typedef enum
{
    I2S_MASTER_FULL_DUPLEX,
    I2S_MASTER_HALF_TX,
    I2S_MASTER_HALF_RX,
    I2S_SLAVE_FULL_DUPLEX,
    I2S_SLAVE_HALF_TX,
    I2S_SLAVE_HALF_RX,
} i2s_mode_t;

typedef uint32_t i2s_sample_rate_t;

#define I2S_SAMPLE_RATE_8K        8000
#define I2S_SAMPLE_RATE_11_025K   11025
#define I2S_SAMPLE_RATE_16K       16000
#define I2S_SAMPLE_RATE_22_05K    22050
#define I2S_SAMPLE_RATE_32K       32000
#define I2S_SAMPLE_RATE_44_1K     44100
#define I2S_SAMPLE_RATE_48K       48000
#define I2S_SAMPLE_RATE_96K       96000
#define I2S_SAMPLE_RATE_192K      192000

typedef enum
{
    I2S_FMT_DATA16_CH16,
    I2S_FMT_DATA16_CH32,
    I2S_FMT_DATA24_CH32,
    I2S_FMT_DATA32_CH32,
} i2s_format_t;

typedef enum
{
    I2S_DMA_BUF_FIRST,
    I2S_DMA_BUF_SECOND,
} i2s_dma_buf_t;

void i2s_bus_init(i2s_unit_t i2s, i2s_mode_t mode, bool mck_en);
void i2s_set_cfg(i2s_unit_t i2s, i2s_sample_rate_t samplerate, i2s_format_t format);
void i2s_set_tx_dma(i2s_unit_t i2s, void *buf, uint32_t len, cb_arg_t func);
void i2s_set_rx_dma(i2s_unit_t i2s, void *buf, uint32_t len, cb_arg_t func);
void i2s_tx_start(i2s_unit_t i2s);
void i2s_tx_stop(i2s_unit_t i2s);
void i2s_rx_start(i2s_unit_t i2s);
void i2s_rx_stop(i2s_unit_t i2s);

#endif