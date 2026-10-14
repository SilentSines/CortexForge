#include "hal.h"
#include "spi_cfg.h"

static const spi_mode_t spi_mode_table[] =
{
	{SPI_CLOCK_POLARITY_LOW, SPI_CLOCK_PHASE_1EDGE},
	{SPI_CLOCK_POLARITY_LOW, SPI_CLOCK_PHASE_2EDGE},
	{SPI_CLOCK_POLARITY_HIGH, SPI_CLOCK_PHASE_1EDGE},
	{SPI_CLOCK_POLARITY_HIGH, SPI_CLOCK_PHASE_2EDGE},
};

#if SPI1_EN == 1
static spi_drv_t spi_drv1 = {.instance = SPI1};
#endif

#if SPI2_EN == 1
static spi_drv_t spi_drv2 = {.instance = SPI2};
#endif

#if SPI3_EN == 1
static spi_drv_t spi_drv3 = {.instance = SPI3};
#endif

#if SPI4_EN == 1
static spi_drv_t spi_drv4 = {.instance = SPI4};
#endif

static spi_drv_t* spi_select(spi_unit_t spi)
{
    switch (spi)
    {
    #if SPI1_EN == 1
        case SPI_UNIT_1 : return &spi_drv1; break;
    #endif

    #if SPI2_EN == 1
        case SPI_UNIT_2 : return &spi_drv2; break;
    #endif

    #if SPI3_EN == 1
        case SPI_UNIT_3 : return &spi_drv3; break;
    #endif

    #if SPI4_EN == 1
        case SPI_UNIT_4 : return &spi_drv4; break;
    #endif

        default : return NULL; break;
    }
}

void spi_bus_init(spi_unit_t spi, uint8_t mode)
{
	gpio_pin_t sck_pin, miso_pin, mosi_pin;

	dma_type *dma_tx, *dma_rx; 
	IRQn_Type dma_tx_irq, dma_rx_irq;
	uint32_t dma_tx_irq_prio, dma_rx_irq_prio;
	uint8_t flex_ch_tx, flex_ch_rx;
	dma_flexible_request_type flex_req_tx, flex_req_rx;

	spi_init_type spi_cfg = {0};
	dma_init_type dma_cfg = {0};
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	switch (spi)
	{
		case SPI_UNIT_1 :	crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
						#if SPI1_PIN_GROUP_1
							sck_pin = GPIO_PA5;
							miso_pin = GPIO_PA6;
							mosi_pin = GPIO_PA7;
						#elif SPI1_PIN_GROUP_2
							gpio_pin_remap_config(SPI1_GMUX_0001, TRUE);
							sck_pin = GPIO_PB3;
							miso_pin = GPIO_PB4;
							mosi_pin = GPIO_PB5;
						#endif
						#if SPI1_MISO_UNUSED
							miso_pin = GPIO_PIN_UNUSED;
						#endif
						#if SPI1_EN == 1
						#if SPI1_DMA_TX_EN == 1 || SPI1_DMA_RX_EN == 1
							dma_tx = SPI1_DMA_TX;
							spi_drv->dma_tx = SPI1_DMA_TX_CH;
							dma_tx_irq = SPI1_DMA_TX_IRQ;
							dma_tx_irq_prio = SPI1_DMA_TX_IRQ_PRIORITY;
							flex_ch_tx = SPI1_DMA_TX_FLEX;
							flex_req_tx = SPI1_DMA_TX_FLEX_REQ;
						#endif
						#if SPI1_DMA_RX_EN == 1
							dma_rx = SPI1_DMA_RX;
							spi_drv->dma_rx = SPI1_DMA_RX_CH;
							dma_rx_irq = SPI1_DMA_RX_IRQ;
							dma_rx_irq_prio = SPI1_DMA_RX_IRQ_PRIORITY;
							flex_ch_rx = SPI1_DMA_RX_FLEX;
							flex_req_rx = SPI1_DMA_RX_FLEX_REQ;
						#endif
						#endif
							break;

		case SPI_UNIT_2 :	crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
							sck_pin = GPIO_PB13;
							miso_pin = GPIO_PB14;
							mosi_pin = GPIO_PB15;
						#if SPI2_MISO_UNUSED
							miso_pin = GPIO_PIN_UNUSED;
						#endif
						#if SPI2_EN == 1	
						#if SPI2_DMA_TX_EN == 1 || SPI2_DMA_RX_EN == 1
							dma_tx = SPI2_DMA_TX;
							spi_drv->dma_tx = SPI2_DMA_TX_CH;
							dma_tx_irq = SPI2_DMA_TX_IRQ;
							dma_tx_irq_prio = SPI2_DMA_TX_IRQ_PRIORITY;
							flex_ch_tx = SPI2_DMA_TX_FLEX;
							flex_req_tx = SPI2_DMA_TX_FLEX_REQ;
						#endif
						#if SPI2_DMA_RX_EN == 1
							dma_rx = SPI2_DMA_RX;
							spi_drv->dma_rx = SPI2_DMA_RX_CH;
							dma_rx_irq = SPI2_DMA_RX_IRQ;
							dma_rx_irq_prio = SPI2_DMA_RX_IRQ_PRIORITY;
							flex_ch_rx = SPI2_DMA_RX_FLEX;
							flex_req_rx = SPI2_DMA_RX_FLEX_REQ;
						#endif
						#endif
							break;

		case SPI_UNIT_3 :	crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
						#if SPI3_PIN_GROUP_1
							sck_pin = GPIO_PB3;
							miso_pin = GPIO_PB4;
							mosi_pin = GPIO_PB5;
						#elif SPI3_PIN_GROUP_2
							gpio_pin_remap_config(SPI3_GMUX_0001, TRUE);
							sck_pin = GPIO_PC10;
							miso_pin = GPIO_PC11;
							mosi_pin = GPIO_PC12;
						#endif
						#if SPI3_MISO_UNUSED
							miso_pin = GPIO_PIN_UNUSED;
						#endif
						#if SPI3_EN == 1
						#if SPI3_DMA_TX_EN == 1 || SPI3_DMA_RX_EN == 1
							dma_tx = SPI3_DMA_TX;
							spi_drv->dma_tx = SPI3_DMA_TX_CH;
							dma_tx_irq = SPI3_DMA_TX_IRQ;
							dma_tx_irq_prio = SPI3_DMA_TX_IRQ_PRIORITY;
							flex_ch_tx = SPI3_DMA_TX_FLEX;
							flex_req_tx = SPI3_DMA_TX_FLEX_REQ;
						#endif
						#if SPI3_DMA_RX_EN == 1
							dma_rx = SPI3_DMA_RX;
							spi_drv->dma_rx = SPI3_DMA_RX_CH;
							dma_rx_irq = SPI3_DMA_RX_IRQ;
							dma_rx_irq_prio = SPI3_DMA_RX_IRQ_PRIORITY;
							flex_ch_rx = SPI3_DMA_RX_FLEX;
							flex_req_rx = SPI3_DMA_RX_FLEX_REQ;
						#endif
						#endif
							break;

		case SPI_UNIT_4 : 	crm_periph_clock_enable(CRM_SPI4_PERIPH_CLOCK, TRUE);
						#if SPI4_PIN_GROUP_1
							sck_pin = GPIO_PE2;
							miso_pin = GPIO_PE5;
							mosi_pin = GPIO_PE6;
						#elif SPI4_PIN_GROUP_2
							gpio_pin_remap_config(SPI4_GMUX_0001, TRUE);
							sck_pin = GPIO_PE11;
							miso_pin = GPIO_PE13;
							mosi_pin = GPIO_PE14;
						#elif SPI4_PIN_GROUP_3
							gpio_pin_remap_config(SPI4_GMUX_0010, TRUE);
							sck_pin = GPIO_PB7;
							miso_pin = GPIO_PB8;
							mosi_pin = GPIO_PB9;
						#endif
						#if SPI4_MISO_UNUSED
							miso_pin = GPIO_PIN_UNUSED;
						#endif
						#if SPI4_EN == 1
						#if SPI4_DMA_TX_EN == 1 || SPI4_DMA_RX_EN == 1
							dma_tx = SPI4_DMA_TX;
							spi_drv->dma_tx = SPI4_DMA_TX_CH;
							dma_tx_irq = SPI4_DMA_TX_IRQ;
							dma_tx_irq_prio = SPI4_DMA_TX_IRQ_PRIORITY;
							flex_ch_tx = SPI4_DMA_TX_FLEX;
							flex_req_tx = SPI4_DMA_TX_FLEX_REQ;
						#endif
						#if SPI4_DMA_RX_EN == 1
							dma_rx = SPI4_DMA_RX;
							spi_drv->dma_rx = SPI4_DMA_RX_CH;
							dma_rx_irq = SPI4_DMA_RX_IRQ;
							dma_rx_irq_prio = SPI4_DMA_RX_IRQ_PRIORITY;
							flex_ch_rx = SPI4_DMA_RX_FLEX;
							flex_req_rx = SPI4_DMA_RX_FLEX_REQ;
						#endif
						#endif
							break;

		default : return; break;
	}

	gpio_pin_init(sck_pin, GPIO_AF_PP_PULLUP, GPIO_AF_SPI);
	gpio_pin_init(miso_pin, GPIO_AF_PP_PULLUP, GPIO_AF_SPI);
	gpio_pin_init(mosi_pin, GPIO_AF_PP_PULLUP, GPIO_AF_SPI);

	spi_drv->bit_num = SPI_FRAME_8BIT;

	spi_cfg.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
	spi_cfg.master_slave_mode = SPI_MODE_MASTER;
	spi_cfg.mclk_freq_division = SPI_MCLK_DIV_2;
	spi_cfg.first_bit_transmission = SPI_FIRST_BIT_MSB;
	spi_cfg.frame_bit_num = SPI_FRAME_8BIT;
	spi_cfg.clock_polarity = spi_mode_table[mode].cpol;
	spi_cfg.clock_phase = spi_mode_table[mode].cpha;
	spi_cfg.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
	spi_init(spi_drv->instance, &spi_cfg);
	spi_enable(spi_drv->instance, TRUE);

	if (spi_drv->dma_tx != NULL)
	{
		dma_reset(spi_drv->dma_tx);
		dma_cfg.buffer_size = 0;
		dma_cfg.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
		dma_cfg.memory_base_addr = 0;
		dma_cfg.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
		dma_cfg.memory_inc_enable = TRUE;
		dma_cfg.peripheral_base_addr = (uint32_t)&(spi_drv->instance->dt);
		dma_cfg.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
		dma_cfg.peripheral_inc_enable = FALSE;
		dma_cfg.priority = DMA_PRIORITY_MEDIUM;
		dma_cfg.loop_mode_enable = FALSE;
		dma_init(spi_drv->dma_tx, &dma_cfg);
		dma_flexible_config(dma_tx, flex_ch_tx, flex_req_tx);
		nvic_irq_enable(dma_tx_irq, dma_tx_irq_prio, 0);
	}

	if (spi_drv->dma_rx != NULL)
	{
		dma_reset(spi_drv->dma_rx);
		dma_cfg.buffer_size = 0;
		dma_cfg.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
		dma_cfg.memory_base_addr = 0;
		dma_cfg.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
		dma_cfg.memory_inc_enable = TRUE;
		dma_cfg.peripheral_base_addr = (uint32_t)&(spi_drv->instance->dt);
		dma_cfg.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
		dma_cfg.peripheral_inc_enable = FALSE;
		dma_cfg.priority = DMA_PRIORITY_MEDIUM;
		dma_cfg.loop_mode_enable = FALSE;
		dma_init(spi_drv->dma_rx, &dma_cfg);
		dma_flexible_config(dma_rx, flex_ch_rx, flex_req_rx);
		nvic_irq_enable(dma_rx_irq, dma_rx_irq_prio, 0);
	}
}

void spi_set_speed(spi_unit_t spi, spi_freq_div_t div)
{
	spi_mclk_freq_div_type mclk_div = (spi_mclk_freq_div_type)div;
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	spi_drv->instance->ctrl1_bit.spien = FALSE;
	if(mclk_div > SPI_MCLK_DIV_256)
	{
		spi_drv->instance->ctrl2_bit.mdiv_h = 1;
		spi_drv->instance->ctrl1_bit.mdiv_l = mclk_div & 0x7;
	}
	else
	{
		spi_drv->instance->ctrl2_bit.mdiv_h = 0;
		spi_drv->instance->ctrl1_bit.mdiv_l = mclk_div;
	}
	spi_drv->instance->ctrl1_bit.spien = TRUE;
}

void spi_set_data_width(spi_unit_t spi, spi_data_width_t width)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	spi_frame_bit_num_type bit_num = (spi_frame_bit_num_type)(width);
	if (spi_drv->bit_num == bit_num)
	{
		return;
	}
	
    spi_drv->bit_num = bit_num;
	spi_drv->instance->ctrl1_bit.spien = FALSE;
	spi_drv->instance->ctrl1_bit.fbn = bit_num;
	spi_drv->instance->ctrl1_bit.spien = TRUE;

	dma_channel_type *dma_tx = spi_drv->dma_tx;
	if (dma_tx != NULL)
	{
		dma_tx->ctrl_bit.chen = FALSE;
		if (bit_num == SPI_FRAME_8BIT)
		{
			dma_tx->ctrl_bit.pwidth = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
			dma_tx->ctrl_bit.mwidth = DMA_MEMORY_DATA_WIDTH_BYTE;
		}
		else
		{
			dma_tx->ctrl_bit.pwidth = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
			dma_tx->ctrl_bit.mwidth = DMA_MEMORY_DATA_WIDTH_HALFWORD;
		}
	}

    dma_channel_type *dma_rx = spi_drv->dma_rx;
	if (dma_rx != NULL)
	{
		dma_rx->ctrl_bit.chen = FALSE;
		if (bit_num == SPI_FRAME_8BIT)
		{
			dma_rx->ctrl_bit.pwidth = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
			dma_rx->ctrl_bit.mwidth = DMA_MEMORY_DATA_WIDTH_BYTE;
		}
		else
		{
			dma_rx->ctrl_bit.pwidth = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
			dma_rx->ctrl_bit.mwidth = DMA_MEMORY_DATA_WIDTH_HALFWORD;
		}
	}
}

void spi_send_poll(spi_unit_t spi, const void *buf, uint32_t len)
{
	uint32_t i;
	uint16_t retry;
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}
	
	if (spi_drv->bit_num == SPI_FRAME_8BIT)
	{
		const uint8_t *buf8 = (const uint8_t *)buf;

		for (i = 0; i < len; i++)
		{
			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_TDBE_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			spi_i2s_data_transmit(spi_drv->instance, buf8[i]);

			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_RDBF_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			spi_i2s_data_receive(spi_drv->instance);
		}

		retry = 0xFFFF;
		while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_BF_FLAG) != RESET)
		{
			if (--retry == 0)
			{
				return;
			}
		}
	}
	else
	{
		const uint16_t *buf16 = (const uint16_t *)buf;

		for (i = 0; i < len; i++)
		{
			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_TDBE_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			spi_i2s_data_transmit(spi_drv->instance, buf16[i]);

			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_RDBF_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			spi_i2s_data_receive(spi_drv->instance);
		}

		retry = 0xFFFF;
		while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_BF_FLAG) != RESET)
		{
			if (--retry == 0)
			{
				return;
			}
		}
	}
	
}

void spi_recv_poll(spi_unit_t spi, void *buf, uint32_t len)
{
	uint32_t i;
	uint16_t retry;
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	if (spi_drv->bit_num == SPI_FRAME_8BIT)
	{
		uint8_t *buf8 = (uint8_t *)buf;

		for (i = 0; i < len; i++)
		{
			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_TDBE_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			spi_i2s_data_transmit(spi_drv->instance, 0xFF);

			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_RDBF_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			buf8[i] = spi_i2s_data_receive(spi_drv->instance);
		}

		retry = 0xFFFF;
		while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_BF_FLAG) != RESET)
		{
			if (--retry == 0)
			{
				return;
			}
		}
	}
	else
	{
		uint16_t *buf16 = (uint16_t *)buf;
		
		for (i = 0; i < len; i++)
		{
			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_TDBE_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			spi_i2s_data_transmit(spi_drv->instance, 0xFF);

			retry = 0xFFFF;
			while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_RDBF_FLAG) == RESET)
			{
				if (--retry == 0)
				{
					return;
				}
			}
			buf16[i] = spi_i2s_data_receive(spi_drv->instance);
		}

		retry = 0xFFFF;
		while(spi_i2s_flag_get(spi_drv->instance, SPI_I2S_BF_FLAG) != RESET)
		{
			if (--retry == 0)
			{
				return;
			}
		}
	}
}

void spi_send_dma(spi_unit_t spi, const void *buf, uint32_t len, cb_t func)
{
	spi_drv_t *spi_drv = spi_select(spi);
	
	if (spi_drv == NULL)
	{
		return;
	}

	spi_drv->busy = true;
	spi_drv->func = func;
	
	dma_channel_enable(spi_drv->dma_tx, FALSE);
	spi_drv->dma_tx->maddr = (uint32_t)buf;
	spi_drv->dma_tx->dtcnt = len;
	dma_channel_enable(spi_drv->dma_tx, TRUE);
	dma_interrupt_enable(spi_drv->dma_tx, DMA_FDT_INT, TRUE);

	spi_i2s_dma_transmitter_enable(spi_drv->instance, TRUE);
}

void spi_recv_dma(spi_unit_t spi, void *buf, uint32_t len, cb_t func)
{
	spi_drv_t *spi_drv = spi_select(spi);
	
	if (spi_drv == NULL)
	{
		return;
	}

	spi_drv->busy = true;
	spi_drv->func = func;

	dma_channel_enable(spi_drv->dma_rx, FALSE);
	spi_drv->dma_rx->maddr = (uint32_t)buf;
	spi_drv->dma_rx->dtcnt = len;
	dma_channel_enable(spi_drv->dma_rx, TRUE);
	dma_interrupt_enable(spi_drv->dma_rx, DMA_FDT_INT, TRUE);
	spi_i2s_dma_receiver_enable(spi_drv->instance, TRUE);

	dma_channel_enable(spi_drv->dma_tx, FALSE);
	spi_drv->dma_tx->maddr = (uint32_t)buf;
	spi_drv->dma_tx->dtcnt = len;
	dma_channel_enable(spi_drv->dma_tx, TRUE);
	spi_i2s_dma_transmitter_enable(spi_drv->instance, TRUE);
}

bool spi_is_busy(spi_unit_t spi)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return false;
	}
	return spi_drv->busy;
}

static void spi_dma_irq_handle(spi_unit_t spi, bool is_tx)
{
	uint16_t retry = 0xFFFF;
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	dma_channel_type* ch = (is_tx) ? spi_drv->dma_tx : spi_drv->dma_rx;

	if (dma_get_flag(ch, DMA_FDT_FLAG) != RESET)
	{
		dma_channel_enable(spi_drv->dma_tx, FALSE);
		dma_interrupt_enable(spi_drv->dma_tx, DMA_FDT_INT, FALSE);
		dma_clear_flag(spi_drv->dma_tx, DMA_GL_FLAG);
		spi_i2s_dma_transmitter_enable(spi_drv->instance, FALSE);
		
		if (is_tx == false)
		{
			dma_channel_enable(spi_drv->dma_rx, FALSE);
			dma_interrupt_enable(spi_drv->dma_rx, DMA_FDT_INT, FALSE);
			dma_clear_flag(spi_drv->dma_rx, DMA_GL_FLAG);
			spi_i2s_dma_receiver_enable(spi_drv->instance, FALSE);
		}

		while (spi_i2s_flag_get(spi_drv->instance, SPI_I2S_BF_FLAG) != RESET)
		{
			if (--retry == 0)
			{
				break;
			}
		}

		if (spi_drv->func)
		{
			spi_drv->func();
		}
		spi_drv->busy = false;
	}
}

#if SPI1_DMA_TX_EN == 1
void SPI1_DMA_TX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_1, true);
}
#endif

#if SPI1_DMA_RX_EN == 1
void SPI1_DMA_RX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_1, false);
}
#endif

#if SPI2_DMA_TX_EN == 1
void SPI2_DMA_TX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_2, true);
}
#endif

#if SPI2_DMA_RX_EN == 1
void SPI2_DMA_RX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_2, false);	
}	
#endif

#if SPI3_DMA_TX_EN == 1
void SPI3_DMA_TX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_3, true);
}
#endif

#if SPI3_DMA_RX_EN == 1
void SPI3_DMA_RX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_3, false);
}
#endif

#if SPI4_DMA_TX_EN == 1
void SPI4_DMA_TX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_4, true);
}
#endif

#if SPI4_DMA_RX_EN == 1
void SPI4_DMA_RX_IRQHANDLER(void)
{
	spi_dma_irq_handle(SPI_UNIT_4, false);
}
#endif