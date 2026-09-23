#include "hal.h"
#include "i2s_cfg.h"

#if I2S1_EN == 1
static i2s_drv_t i2s_drv1;
#endif

#if I2S2_EN == 1
static i2s_drv_t i2s_drv2;
#endif

#if I2S3_EN == 1
static i2s_drv_t i2s_drv3;
#endif

#if I2S4_EN == 1
static i2s_drv_t i2s_drv4;
#endif

static i2s_drv_t* i2s_select(i2s_port_t i2s)
{
	i2s_drv_t *i2s_drv = NULL;

	if (i2s == I2S1)
	{
	#if I2S1_EN == 1
		i2s_drv = &i2s_drv1;
	#endif
	}
	else if (i2s == I2S2)
	{
	#if I2S2_EN == 1
		i2s_drv = &i2s_drv2;
	#endif
	}
	else if (i2s == I2S3)
	{
	#if I2S3_EN == 1
		i2s_drv = &i2s_drv3;
	#endif
	}
	else if (i2s == I2S4)
	{
	#if I2S4_EN == 1
		i2s_drv = &i2s_drv4;
	#endif
	}
	return i2s_drv;
}

static void spi_drv_select(i2s_port_t i2s, spi_type **spi, spi_type **i2s_ext)
{
	switch (i2s)
	{
		case I2S1 : *spi = SPI1; *i2s_ext = NULL; break;
		case I2S2 : *spi = SPI2; *i2s_ext = I2S2EXT; break;
		case I2S3 : *spi = SPI3; *i2s_ext = I2S3EXT; break;
		case I2S4 : *spi = SPI4; *i2s_ext = NULL; break;
	}
}

void i2s_bus_init(i2s_port_t i2s, i2s_mode_t mode, bool mck_en)
{
	crm_periph_clock_type clk;
	spi_type *spi, *i2s_ext;
	gpio_pin_t ws_pin, ck_pin, sdext_pin, sd_pin, mck_pin;

	dma_type *dma_tx, *dma_rx; 
	IRQn_Type dma_tx_irq, dma_rx_irq;
	uint32_t dma_tx_irq_prio, dma_rx_irq_prio;
	uint8_t flex_ch_tx, flex_ch_rx;
	dma_flexible_request_type flex_req_tx, flex_req_rx;

	dma_init_type dma_cfg = {0};
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}

	if (i2s == I2S1)
	{
		clk = CRM_SPI1_PERIPH_CLOCK;
	#if I2S1_PIN_GROUP_1
		ws_pin = GPIO_PA4;
		ck_pin = GPIO_PA5;
		sdext_pin = GPIO_PA7;
		sd_pin = GPIO_PA7;
		mck_pin = GPIO_PB0;
	#elif I2S1_PIN_GROUP_2
		gpio_pin_remap_config(SPI1_GMUX_0001, TRUE);
		ws_pin = GPIO_PA15;
		ck_pin = GPIO_PB3;
		sdext_pin = GPIO_PB5;
		sd_pin = GPIO_PB5;
		mck_pin = GPIO_PB0;
	#elif I2S1_PIN_GROUP_3
		gpio_pin_remap_config(SPI1_GMUX_0010, TRUE);
		ws_pin = GPIO_PA4;
		ck_pin = GPIO_PA5;
		sdext_pin = GPIO_PA7;
		sd_pin = GPIO_PA7;
		mck_pin = GPIO_PB6;
	#elif I2S1_PIN_GROUP_4
		gpio_pin_remap_config(SPI1_GMUX_0011, TRUE);
		ws_pin = GPIO_PA15;
		ck_pin = GPIO_PB3;
		sdext_pin = GPIO_PB5;
		sd_pin = GPIO_PB5;
		mck_pin = GPIO_PB6;
	#endif
		dma_tx = I2S1_DMA_TX;
		i2s_drv->dma_tx = I2S1_DMA_TX_CH;
		dma_tx_irq = I2S1_DMA_TX_IRQ;
		dma_tx_irq_prio = I2S1_DMA_TX_IRQ_PRIORITY;
		flex_ch_tx = I2S1_DMA_TX_FLEX;
		flex_req_tx = I2S1_DMA_TX_FLEX_REQ;

		dma_rx = I2S1_DMA_RX;
		i2s_drv->dma_rx = I2S1_DMA_RX_CH;
		dma_rx_irq = I2S1_DMA_RX_IRQ;
		dma_rx_irq_prio = I2S1_DMA_RX_IRQ_PRIORITY;
		flex_ch_rx = I2S1_DMA_RX_FLEX;
		flex_req_rx = I2S1_DMA_RX_FLEX_REQ;	
	}
	else if (i2s == I2S2)
	{
		clk = CRM_SPI2_PERIPH_CLOCK;

		ws_pin = GPIO_PB12;
		ck_pin = GPIO_PB13;
		sdext_pin = GPIO_PB14;
		sd_pin = GPIO_PB15;
	#if I2S2_PIN_GROUP_1
		mck_pin = GPIO_PC6;
	#elif I2S2_PIN_GROUP_2
		gpio_pin_remap_config(SPI2_GMUX_0001, TRUE);
		mck_pin = GPIO_PA3;
	#elif I2S2_PIN_GROUP_3
		gpio_pin_remap_config(SPI2_GMUX_0010, TRUE);
		mck_pin = GPIO_PA6;
	#endif
		dma_tx = I2S2_DMA_TX;
		i2s_drv->dma_tx = I2S2_DMA_TX_CH;
		dma_tx_irq = I2S2_DMA_TX_IRQ;
		dma_tx_irq_prio = I2S2_DMA_TX_IRQ_PRIORITY;
		flex_ch_tx = I2S2_DMA_TX_FLEX;
		flex_req_tx = I2S2_DMA_TX_FLEX_REQ;

		dma_rx = I2S2_DMA_RX;
		i2s_drv->dma_rx = I2S2_DMA_RX_CH;
		dma_rx_irq = I2S2_DMA_RX_IRQ;
		dma_rx_irq_prio = I2S2_DMA_RX_IRQ_PRIORITY;
		flex_ch_rx = I2S2_DMA_RX_FLEX;
		flex_req_rx = I2S2_DMA_RX_FLEX_REQ;
	}
	else if (i2s == I2S3)
	{
		clk = CRM_SPI3_PERIPH_CLOCK;
	#if I2S3_PIN_GROUP_1
		ws_pin = GPIO_PA15;
		ck_pin = GPIO_PB3;
		sdext_pin = GPIO_PB4;
		sd_pin = GPIO_PB5;
		mck_pin = GPIO_PC7;
	#elif I2S3_PIN_GROUP_2
		gpio_pin_remap_config(SPI3_GMUX_0001, TRUE);
		ws_pin = GPIO_PA4;
		ck_pin = GPIO_PC10;
		sdext_pin = GPIO_PC11;
		sd_pin = GPIO_PC12;
		mck_pin = GPIO_PC7;
	#elif I2S3_PIN_GROUP_3
		gpio_pin_remap_config(SPI3_GMUX_0010, TRUE);
		ws_pin = GPIO_PA15;
		ck_pin = GPIO_PB3;
		sdext_pin = GPIO_PB4;
		sd_pin = GPIO_PB5;
		mck_pin = GPIO_PB10;
	#elif I2S3_PIN_GROUP_4
		gpio_pin_remap_config(SPI3_GMUX_0011, TRUE);
		ws_pin = GPIO_PA4;
		ck_pin = GPIO_PC10;
		sdext_pin = GPIO_PC11;
		sd_pin = GPIO_PC12;
		mck_pin = GPIO_PB10;
	#endif
		dma_tx = I2S3_DMA_TX;
		i2s_drv->dma_tx = I2S3_DMA_TX_CH;
		dma_tx_irq = I2S3_DMA_TX_IRQ;
		dma_tx_irq_prio = I2S3_DMA_TX_IRQ_PRIORITY;
		flex_ch_tx = I2S3_DMA_TX_FLEX;
		flex_req_tx = I2S3_DMA_TX_FLEX_REQ;

		dma_rx = I2S3_DMA_RX;
		i2s_drv->dma_rx = I2S3_DMA_RX_CH;
		dma_rx_irq = I2S3_DMA_RX_IRQ;
		dma_rx_irq_prio = I2S3_DMA_RX_IRQ_PRIORITY;
		flex_ch_rx = I2S3_DMA_RX_FLEX;
		flex_req_rx = I2S3_DMA_RX_FLEX_REQ;
	}
	else if (i2s == I2S4)
	{
		clk = CRM_SPI4_PERIPH_CLOCK;
	#if I2S4_PIN_GROUP_1
		ws_pin = GPIO_PE4;
		ck_pin = GPIO_PE2;
		sdext_pin = GPIO_PE6;
		sd_pin = GPIO_PE6;
		mck_pin = GPIO_PC8;
	#elif I2S4_PIN_GROUP_2
		gpio_pin_remap_config(SPI4_GMUX_0001, TRUE);
		ws_pin = GPIO_PE12;
		ck_pin = GPIO_PE11;
		sdext_pin = GPIO_PE14;
		sd_pin = GPIO_PE14;
		mck_pin = GPIO_PC8;
	#elif I2S4_PIN_GROUP_3
		gpio_pin_remap_config(SPI4_GMUX_0010, TRUE);
		ws_pin = GPIO_PB6;
		ck_pin = GPIO_PB7;
		sdext_pin = GPIO_PB9;
		sd_pin = GPIO_PB9;
		mck_pin = GPIO_PC8;
	#elif I2S4_PIN_GROUP_4
		gpio_pin_remap_config(SPI4_GMUX_0011, TRUE);
		ws_pin = GPIO_PB6;
		ck_pin = GPIO_PB7;
		sdext_pin = GPIO_PB9;
		sd_pin = GPIO_PB9;
		mck_pin = GPIO_PA10;
	#endif
		dma_tx = I2S4_DMA_TX;
		i2s_drv->dma_tx = I2S4_DMA_TX_CH;
		dma_tx_irq = I2S4_DMA_TX_IRQ;
		dma_tx_irq_prio = I2S4_DMA_TX_IRQ_PRIORITY;
		flex_ch_tx = I2S4_DMA_TX_FLEX;
		flex_req_tx = I2S4_DMA_TX_FLEX_REQ;

		dma_rx = I2S4_DMA_RX;
		i2s_drv->dma_rx = I2S4_DMA_RX_CH;
		dma_rx_irq = I2S4_DMA_RX_IRQ;
		dma_rx_irq_prio = I2S4_DMA_RX_IRQ_PRIORITY;
		flex_ch_rx = I2S4_DMA_RX_FLEX;
		flex_req_rx = I2S4_DMA_RX_FLEX_REQ;
	}
	
	gpio_pin_init(ws_pin, GPIO_AF_PP_PULLUP, GPIO_DRIVE_STRENGTH_STRONGER);
	gpio_pin_init(ck_pin, GPIO_AF_PP_PULLUP, GPIO_DRIVE_STRENGTH_STRONGER);
	gpio_pin_init(sd_pin, GPIO_AF_PP_PULLUP, GPIO_DRIVE_STRENGTH_STRONGER);
	if (mode == I2S_MASTER_FULL_DUPLEX || mode == I2S_SLAVE_FULL_DUPLEX)
	{
		gpio_pin_init(sdext_pin, GPIO_AF_PP_PULLUP, GPIO_DRIVE_STRENGTH_STRONGER);
	}
	if (mck_en)
	{
		gpio_pin_init(mck_pin, GPIO_AF_PP_PULLUP, GPIO_DRIVE_STRENGTH_STRONGER);
	}

	crm_periph_clock_enable(clk, TRUE);
	spi_drv_select(i2s, &spi, &i2s_ext);

	i2s_drv->mode = mode;
	i2s_drv->mck_en = mck_en;
	i2s_drv->format = I2S_DATA_16BIT_CHANNEL_16BIT;
	i2s_set_cfg(i2s, I2S_AUDIO_FREQUENCY_8K, I2S_DATA_16BIT_CHANNEL_16BIT);
	
	if (mode != I2S_MASTER_HALF_RX && mode != I2S_SLAVE_HALF_RX)//TX DMA
	{
		dma_reset(i2s_drv->dma_tx);
		dma_cfg.buffer_size = 0;
		dma_cfg.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
		dma_cfg.memory_base_addr = 0;
		dma_cfg.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
		dma_cfg.memory_inc_enable = TRUE;
		if (mode == I2S_SLAVE_FULL_DUPLEX)
		{
			dma_cfg.peripheral_base_addr = (uint32_t)&(i2s_ext->dt);
		}
		else
		{
			dma_cfg.peripheral_base_addr = (uint32_t)&(spi->dt);
		}
		dma_cfg.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
		dma_cfg.peripheral_inc_enable = FALSE;
		dma_cfg.priority = DMA_PRIORITY_MEDIUM;
		dma_cfg.loop_mode_enable = TRUE;
		dma_init(i2s_drv->dma_tx, &dma_cfg);
		dma_flexible_config(dma_tx, flex_ch_tx, flex_req_tx);
		nvic_irq_enable(dma_tx_irq, dma_tx_irq_prio, 0);
	}
	
	if (mode != I2S_MASTER_HALF_TX && mode != I2S_SLAVE_HALF_TX)//RX DMA
	{
		dma_reset(i2s_drv->dma_rx);
		dma_cfg.buffer_size = 0;
		dma_cfg.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
		dma_cfg.memory_base_addr = 0;
		dma_cfg.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
		dma_cfg.memory_inc_enable = TRUE;
		if (mode == I2S_MASTER_FULL_DUPLEX)
		{
			dma_cfg.peripheral_base_addr = (uint32_t)&(i2s_ext->dt);
		}
		else
		{
			dma_cfg.peripheral_base_addr = (uint32_t)&(spi->dt);
		}
		dma_cfg.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
		dma_cfg.peripheral_inc_enable = FALSE;
		dma_cfg.priority = DMA_PRIORITY_MEDIUM;
		dma_cfg.loop_mode_enable = TRUE;
		dma_init(i2s_drv->dma_rx, &dma_cfg);
		dma_flexible_config(dma_rx, flex_ch_rx, flex_req_rx);
		nvic_irq_enable(dma_rx_irq, dma_rx_irq_prio, 0);
	}
}

void i2s_set_cfg(i2s_port_t i2s, i2s_audio_sampling_freq_type samplerate, i2s_data_channel_format_type format)
{
	spi_type *spi, *i2s_ext;
	i2s_mode_t mode;
	bool mck_en;
	i2s_init_type i2s_cfg = {0};
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}
	
	spi_drv_select(i2s, &spi, &i2s_ext);

	i2s_drv->format = format;
	mode = i2s_drv->mode;
	mck_en = i2s_drv->mck_en;

	i2s_enable(spi, FALSE);
	i2s_enable(i2s_ext, FALSE);

	switch (mode)
	{
		case I2S_MASTER_FULL_DUPLEX : 	i2s_cfg.operation_mode = I2S_MODE_MASTER_TX; break;
		case I2S_MASTER_HALF_TX	: 		i2s_cfg.operation_mode = I2S_MODE_MASTER_TX; break;
		case I2S_MASTER_HALF_RX :		i2s_cfg.operation_mode = I2S_MODE_MASTER_RX; break;
		case I2S_SLAVE_FULL_DUPLEX : 	i2s_cfg.operation_mode = I2S_MODE_SLAVE_RX; break;
		case I2S_SLAVE_HALF_TX	: 		i2s_cfg.operation_mode = I2S_MODE_SLAVE_TX; break;
		case I2S_SLAVE_HALF_RX :		i2s_cfg.operation_mode = I2S_MODE_SLAVE_RX; break;
	}
	i2s_cfg.audio_protocol = I2S_AUDIO_PROTOCOL_PHILLIPS;
	i2s_cfg.data_channel_format = format;
	i2s_cfg.mclk_output_enable = (mck_en) ? TRUE : FALSE;
	i2s_cfg.audio_sampling_freq = samplerate;
	i2s_cfg.clock_polarity = I2S_CLOCK_POLARITY_LOW;
	i2s_init(spi, &i2s_cfg);
  
	if (mode == I2S_MASTER_FULL_DUPLEX)
	{
		i2s_cfg.operation_mode = I2S_MODE_SLAVE_RX;
		i2s_init(i2s_ext, &i2s_cfg);
	}
	else if (mode == I2S_SLAVE_FULL_DUPLEX)
	{
		i2s_cfg.operation_mode = I2S_MODE_SLAVE_TX;
		i2s_init(i2s_ext, &i2s_cfg);
	}
}

void i2s_set_tx_dma(i2s_port_t i2s, uint16_t *buf, uint32_t len, bool dbuf_en, cb_arg_t func)
{
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}

	i2s_drv->func = func;
	i2s_drv->dbuf_en = dbuf_en;
	dma_channel_enable(i2s_drv->dma_tx, FALSE);
	i2s_drv->dma_tx->maddr = (uint32_t)buf;
	i2s_drv->dma_tx->dtcnt = (i2s_drv->format <= I2S_DATA_16BIT_CHANNEL_32BIT) ? len : (len << 1);
	if (dbuf_en)
	{
		dma_interrupt_enable(i2s_drv->dma_tx, DMA_FDT_INT | DMA_HDT_INT, TRUE);
	}
	else
	{
		dma_interrupt_enable(i2s_drv->dma_tx, DMA_HDT_INT, FALSE);
		dma_interrupt_enable(i2s_drv->dma_tx, DMA_FDT_INT, TRUE);
	}
}

void i2s_set_rx_dma(i2s_port_t i2s, uint16_t *buf, uint32_t len, bool dbuf_en, cb_arg_t func)
{
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}

	i2s_drv->func = func;
	i2s_drv->dbuf_en = dbuf_en;
	dma_channel_enable(i2s_drv->dma_rx, FALSE);
	i2s_drv->dma_rx->maddr = (uint32_t)buf;
	i2s_drv->dma_rx->dtcnt = (i2s_drv->format <= I2S_DATA_16BIT_CHANNEL_32BIT) ? len : (len << 1);
	if (dbuf_en)
	{
		dma_interrupt_enable(i2s_drv->dma_rx, DMA_FDT_INT | DMA_HDT_INT, TRUE);
	}
	else
	{
		dma_interrupt_enable(i2s_drv->dma_rx, DMA_HDT_INT, FALSE);
		dma_interrupt_enable(i2s_drv->dma_rx, DMA_FDT_INT, TRUE);
	}
}

void i2s_tx_start(i2s_port_t i2s)
{
	spi_type *spi, *i2s_ext;
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}
	
	spi_drv_select(i2s, &spi, &i2s_ext);

	if (i2s_drv->mode == I2S_SLAVE_FULL_DUPLEX && (i2s == I2S2 || i2s == I2S3))
	{
		spi_i2s_dma_transmitter_enable(i2s_ext, TRUE);
		dma_channel_enable(i2s_drv->dma_tx, TRUE);
		i2s_enable(i2s_ext, TRUE);
	}
	else
	{	
		spi_i2s_dma_transmitter_enable(spi, TRUE);
		dma_channel_enable(i2s_drv->dma_tx, TRUE);
		i2s_enable(spi, TRUE);
	}
}

void i2s_tx_stop(i2s_port_t i2s)
{
	spi_type *spi, *i2s_ext;
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}
	
	spi_drv_select(i2s, &spi, &i2s_ext);

	if (i2s_drv->mode == I2S_SLAVE_FULL_DUPLEX && (i2s == I2S2 || i2s == I2S3))
	{
		spi_i2s_dma_transmitter_enable(i2s_ext, FALSE);
		dma_channel_enable(i2s_drv->dma_tx, FALSE);
		i2s_enable(i2s_ext, FALSE);
	}
	else
	{	
		spi_i2s_dma_transmitter_enable(spi, FALSE);
		dma_channel_enable(i2s_drv->dma_tx, FALSE);
		i2s_enable(spi, FALSE);
	}
}

void i2s_rx_start(i2s_port_t i2s)
{
    spi_type *spi, *i2s_ext;
    i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}
    
    spi_drv_select(i2s, &spi, &i2s_ext);
    
    if (i2s_drv->mode == I2S_MASTER_FULL_DUPLEX && (i2s == I2S2 || i2s == I2S3))
    {
        spi_i2s_dma_receiver_enable(i2s_ext, TRUE);
        dma_channel_enable(i2s_drv->dma_rx, TRUE);
        i2s_enable(i2s_ext, TRUE);
    }
    else
    {
        spi_i2s_dma_receiver_enable(spi, TRUE);
        dma_channel_enable(i2s_drv->dma_rx, TRUE);
        i2s_enable(spi, TRUE);
    }
}

void i2s_rx_stop(i2s_port_t i2s)
{
    spi_type *spi, *i2s_ext;
    i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}
    
    spi_drv_select(i2s, &spi, &i2s_ext);
    
    if (i2s_drv->mode == I2S_MASTER_FULL_DUPLEX && (i2s == I2S2 || i2s == I2S3))
    {
        spi_i2s_dma_receiver_enable(i2s_ext, FALSE);
        dma_channel_enable(i2s_drv->dma_rx, FALSE);
        i2s_enable(i2s_ext, FALSE);
    }
    else
    {
        spi_i2s_dma_receiver_enable(spi, FALSE);
        dma_channel_enable(i2s_drv->dma_rx, FALSE);
        i2s_enable(spi, FALSE);
    }
}

static void i2s_dma_irq_handle(i2s_port_t i2s, bool is_tx)
{
	dma_channel_type *ch;
	cb_arg_t func;
	bool dbuf;
	i2s_drv_t *i2s_drv = i2s_select(i2s);

	if (i2s_drv == NULL)
	{
		return;
	}

	ch = (is_tx) ? i2s_drv->dma_tx : i2s_drv->dma_rx;
	func = i2s_drv->func;
	dbuf = i2s_drv->dbuf_en;

	if ((dbuf) && (dma_get_flag(ch, DMA_HDT_FLAG) != RESET))
	{
		dma_clear_flag(ch, DMA_HDT_FLAG);
		if (func)
		{
			func((void *)DMA_HDT_FLAG);
		}
	}

	if (dma_get_flag(ch, DMA_FDT_FLAG) != RESET)
	{
		dma_clear_flag(ch, DMA_GL_FLAG);
		if (func)
		{
			func((void *)DMA_FDT_FLAG);
		}
	}
}

#if I2S1_DMA_TX_EN
void I2S1_DMA_TX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S1, true);
}
#endif

#if I2S1_DMA_RX_EN
void I2S1_DMA_RX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S1, false);
}
#endif

#if I2S2_DMA_TX_EN
void I2S2_DMA_TX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S2, true);
}
#endif

#if I2S2_DMA_RX_EN
void I2S2_DMA_RX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S2, false);
}	
#endif

#if I2S3_DMA_TX_EN
void I2S3_DMA_TX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S3, true);
}
#endif

#if I2S3_DMA_RX_EN
void I2S3_DMA_RX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S3, false);
}
#endif

#if I2S4_DMA_TX_EN
void I2S4_DMA_TX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S4, true);
}
#endif

#if I2S4_DMA_RX_EN
void I2S4_DMA_RX_IRQHANDLER(void)
{
	i2s_dma_irq_handle(I2S4, false);
}
#endif