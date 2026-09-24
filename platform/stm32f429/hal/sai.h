#ifndef SAI_H
#define SAI_H

typedef enum
{
    SAI_A,
    SAI_B,
} sai_t;

void sai_init(sai_t sai, uint32_t mode, uint32_t cpol);
void sai_config(sai_t sai, uint32_t samplerate, uint32_t bits);
void sai_dma_config(sai_t sai, void* buf0, void *buf1, uint32_t size, cb_arg_t func);
void sai_start(sai_t sai);
void sai_stop(sai_t sai);

#endif