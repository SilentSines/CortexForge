#ifndef SAI_H
#define SAI_H

typedef enum
{
    SAI_UNIT_A,
    SAI_UNIT_B,
} sai_unit_t;

typedef enum 
{
    SAI_MASTER_TX,
    SAI_MASTER_RX,
    SAI_SLAVE_TX,
    SAI_SLAVE_RX, 
} sai_mode_t;

typedef uint32_t sai_sample_rate_t;

#define SAI_SAMPLE_RATE_8K        8000
#define SAI_SAMPLE_RATE_11_025K   11025
#define SAI_SAMPLE_RATE_16K       16000
#define SAI_SAMPLE_RATE_22_05K    22050
#define SAI_SAMPLE_RATE_32K       32000
#define SAI_SAMPLE_RATE_44_1K     44100
#define SAI_SAMPLE_RATE_48K       48000
#define SAI_SAMPLE_RATE_96K       96000
#define SAI_SAMPLE_RATE_192K      192000

typedef enum
{
    SAI_DATA_WIDTH_8BIT,
    SAI_DATA_WIDTH_10BIT,
    SAI_DATA_WIDTH_16BIT,
    SAI_DATA_WIDTH_20BIT,
    SAI_DATA_WIDTH_24BIT,
    SAI_DATA_WIDTH_32BIT,
} sai_data_width_t;

typedef enum
{
    SAI_DMA_BUF_FIRST,   
    SAI_DMA_BUF_SECOND,
} sai_dma_buf_t;

void sai_bus_init(sai_unit_t sai, sai_mode_t mode);
void sai_set_cfg(sai_unit_t sai, sai_sample_rate_t samplerate, sai_data_width_t width);
void sai_set_dma(sai_unit_t sai, void *buf0, void *buf1, uint32_t len, cb_arg_t func);
void sai_start(sai_unit_t sai);
void sai_stop(sai_unit_t sai);

#endif