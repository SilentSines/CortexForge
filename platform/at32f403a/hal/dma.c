#include "hal.h"

static void dma_ch_get_pos(dma_channel_type *ch, dma_type **dma, uint8_t *pos)
{
	uint32_t base = (uint32_t)ch;
	if (base <= DMA1_CHANNEL7_BASE)
	{
		*dma = DMA1; 
		*pos = (base - DMA1_CHANNEL1_BASE) / 5;
	}
	else
	{
		*dma = DMA2; 
		*pos = (base - DMA2_CHANNEL1_BASE) / 5;
	}
}

flag_status dma_get_flag(dma_channel_type *ch, uint32_t dma_flag)
{
	dma_type *dma; 
	uint8_t pos;
	flag_status status = RESET;

	dma_ch_get_pos(ch, &dma, &pos);
	if ((dma->sts & (dma_flag << pos)) != (uint16_t)RESET)
	{
		status = SET;
	}
	else
	{
		status = RESET;
	}
	return status;
}

void dma_clear_flag(dma_channel_type *ch, uint32_t dma_flag)
{
	dma_type *dma; 
	uint8_t pos;

	dma_ch_get_pos(ch, &dma, &pos);
	dma->clr = (dma_flag << pos);
}