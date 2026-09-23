#ifndef DMA_H
#define DMA_H

#define DMA_GL_FLAG     ((uint32_t)0x00000001)
#define DMA_FDT_FLAG    ((uint32_t)0x00000002)
#define DMA_HDT_FLAG    ((uint32_t)0x00000004)
#define DMA_DTERR_FLAG  ((uint32_t)0x00000008)

flag_status dma_get_flag(dma_channel_type *ch, uint32_t dma_flag);
void dma_clear_flag(dma_channel_type *ch, uint32_t dma_flag);

#endif