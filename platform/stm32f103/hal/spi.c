#include "bsp.h"
#include "spi_cfg.h"

static const spi_mode_t spi_mode_table[] =
{
	{SPI_POLARITY_LOW, SPI_PHASE_1EDGE},
	{SPI_POLARITY_LOW, SPI_PHASE_2EDGE},
	{SPI_POLARITY_HIGH, SPI_PHASE_1EDGE},
	{SPI_POLARITY_HIGH, SPI_PHASE_2EDGE},
};

#if SPI1_EN == 1
static spi_drv_t spi_drv1;
#if SPI1_DMA_TX_EN
static DMA_HandleTypeDef spi1_dma_tx;
#endif
#if SPI1_DMA_RX_EN
static DMA_HandleTypeDef spi1_dma_rx;
#endif
#endif

#if SPI2_EN == 1
static spi_drv_t spi_drv2;
#if SPI2_DMA_TX_EN == 1
static DMA_HandleTypeDef spi2_dma_tx;
#endif
#if SPI2_DMA_RX_EN == 1
static DMA_HandleTypeDef spi2_dma_rx;
#endif
#endif

#if SPI3_EN == 1
static spi_drv_t spi_drv3;
#if SPI3_DMA_TX_EN == 1
static DMA_HandleTypeDef spi3_dma_tx;
#endif
#if SPI3_DMA_RX_EN == 1
static DMA_HandleTypeDef spi3_dma_rx;
#endif
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

        default : return NULL; break;
    }
}

static spi_drv_t* spi_select_by_handle(SPI_HandleTypeDef *handle)
{
#if SPI1_EN == 1
    if (handle == &spi_drv1.handle)
    {
        return &spi_drv1;
    }
#endif

#if SPI2_EN == 1
    if (handle == &spi_drv2.handle)
    {
        return &spi_drv2;
    }
#endif

#if SPI3_EN == 1
    if (handle == &spi_drv3.handle)
    {
        return &spi_drv3;
    }
#endif

    return NULL;
}

void spi_bus_init(spi_unit_t spi, uint8_t mode)
{
	gpio_pin_t sck_pin, miso_pin, mosi_pin;

	IRQn_Type dma_tx_irq, dma_rx_irq;
	uint32_t dma_tx_irq_prio, dma_rx_irq_prio;
	uint32_t baud_rate_pre;

	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	switch (spi)
	{
		case SPI_UNIT_1 :	__HAL_RCC_SPI1_CLK_ENABLE();
							spi_drv->handle.Instance = SPI1;
							baud_rate_pre = SPI_BAUDRATEPRESCALER_4;
						#if SPI1_PIN_GROUP_1
							sck_pin = GPIO_PA5;
							miso_pin = GPIO_PA6;
							mosi_pin = GPIO_PA7;
						#elif SPI1_PIN_GROUP_2
							__HAL_AFIO_REMAP_SPI1_ENABLE();
							sck_pin = GPIO_PB3;
							miso_pin = GPIO_PB4;
							mosi_pin = GPIO_PB5;
						#endif
						#if SPI1_MISO_UNUSED
							miso_pin = GPIO_PIN_UNUSED;
						#endif
						#if SPI1_EN == 1
						#if SPI1_DMA_TX_EN == 1 || SPI1_DMA_RX_EN == 1
							spi_drv->hdma_tx = &spi1_dma_tx;
							spi1_dma_tx.Instance = SPI1_DMA_TX_CH;
							dma_tx_irq = SPI1_DMA_TX_IRQ;
							dma_tx_irq_prio = SPI1_DMA_TX_IRQ_PRIORITY;
						#endif
						#if SPI1_DMA_RX_EN == 1
							spi_drv->hdma_rx = &spi1_dma_rx;
							spi1_dma_rx.Instance = SPI1_DMA_RX_CH;
							dma_rx_irq = SPI1_DMA_RX_IRQ;
							dma_rx_irq_prio = SPI1_DMA_RX_IRQ_PRIORITY;
						#endif
						#endif
							break;
	
	case SPI_UNIT_2 :	__HAL_RCC_SPI2_CLK_ENABLE();
						spi_drv->handle.Instance = SPI2;
						baud_rate_pre = SPI_BAUDRATEPRESCALER_2;
						sck_pin = GPIO_PB13;
						miso_pin = GPIO_PB14;
						mosi_pin = GPIO_PB15;
					#if SPI2_MISO_UNUSED
						miso_pin = GPIO_PIN_UNUSED;
					#endif
					#if SPI2_EN == 1
					#if SPI2_DMA_TX_EN == 1 || SPI2_DMA_RX_EN == 1
						spi_drv->hdma_tx = &spi2_dma_tx;
						spi2_dma_tx.Instance = SPI2_DMA_TX_CH;
						dma_tx_irq = SPI2_DMA_TX_IRQ;
					#endif
					#if SPI2_DMA_RX_EN == 1
						spi_drv->hdma_rx = &spi2_dma_rx;
						spi2_dma_rx.Instance = SPI2_DMA_RX_CH;
						dma_rx_irq = SPI2_DMA_RX_IRQ;
						dma_rx_irq_prio = SPI2_DMA_RX_IRQ_PRIORITY;
					#endif
					#endif
						break;

	case SPI_UNIT_3 :	__HAL_RCC_SPI3_CLK_ENABLE();
						spi_drv->handle.Instance = SPI3;
						baud_rate_pre = SPI_BAUDRATEPRESCALER_2;
						sck_pin = GPIO_PB3;
						miso_pin = GPIO_PB4;
						mosi_pin = GPIO_PB5;
					#if SPI3_MISO_UNUSED
						miso_pin = GPIO_PIN_UNUSED;
					#endif	
					#if SPI3_EN == 1
					#if SPI3_DMA_TX_EN == 1 || SPI3_DMA_RX_EN == 1
						spi_drv->hdma_tx = &spi3_dma_tx;
						spi3_dma_tx.Instance = SPI3_DMA_TX_CH;
						dma_tx_irq = SPI3_DMA_TX_IRQ;
						dma_tx_irq_prio = SPI3_DMA_TX_IRQ_PRIORITY;
					#endif
					#if SPI3_DMA_RX_EN == 1
						spi_drv->hdma_rx = &spi3_dma_rx;
						spi3_dma_rx.Instance = SPI3_DMA_RX_CH;
						dma_rx_irq = SPI3_DMA_RX_IRQ;
						dma_rx_irq_prio = SPI3_DMA_RX_IRQ_PRIORITY;
					#endif
					#endif
						break;

		default : return; break;
	}

	gpio_pin_init(sck_pin, GPIO_AF_PP , GPIO_AF_SPI);
	gpio_pin_init(mosi_pin, GPIO_AF_PP, GPIO_AF_SPI);
	gpio_pin_init(miso_pin, GPIO_IN_FLOATING, GPIO_AF_SPI);

	spi_drv->handle.Init.Mode = SPI_MODE_MASTER;
	spi_drv->handle.Init.Direction = SPI_DIRECTION_2LINES;
	spi_drv->handle.Init.DataSize = SPI_DATASIZE_8BIT;
	spi_drv->handle.Init.CLKPolarity = spi_mode_table[mode].cpol;
	spi_drv->handle.Init.CLKPhase = spi_mode_table[mode].cpha;
	spi_drv->handle.Init.NSS = SPI_NSS_SOFT;
	spi_drv->handle.Init.BaudRatePrescaler = baud_rate_pre;
	spi_drv->handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi_drv->handle.Init.TIMode = SPI_TIMODE_DISABLE;
	spi_drv->handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi_drv->handle.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&spi_drv->handle) != HAL_OK)
	{
		hal_error_handler();
	}

	if (spi_drv->hdma_tx != NULL)
	{
		spi_drv->hdma_tx->Init.Direction = DMA_MEMORY_TO_PERIPH;
		spi_drv->hdma_tx->Init.PeriphInc = DMA_PINC_DISABLE;
		spi_drv->hdma_tx->Init.MemInc = DMA_MINC_ENABLE;
		spi_drv->hdma_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		spi_drv->hdma_tx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		spi_drv->hdma_tx->Init.Mode = DMA_NORMAL;
		spi_drv->hdma_tx->Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(spi_drv->hdma_tx) != HAL_OK)
		{
			hal_error_handler();
		}
		__HAL_LINKDMA(&spi_drv->handle, hdmatx, *(spi_drv->hdma_tx));
		HAL_NVIC_SetPriority(dma_tx_irq, dma_tx_irq_prio, 0);
   	 	HAL_NVIC_EnableIRQ(dma_tx_irq);
	}
	
	if (spi_drv->hdma_rx != NULL)
	{
		spi_drv->hdma_rx->Init.Direction = DMA_PERIPH_TO_MEMORY;
		spi_drv->hdma_rx->Init.PeriphInc = DMA_PINC_DISABLE;
		spi_drv->hdma_rx->Init.MemInc = DMA_MINC_ENABLE;
		spi_drv->hdma_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		spi_drv->hdma_rx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		spi_drv->hdma_rx->Init.Mode = DMA_NORMAL;
		spi_drv->hdma_rx->Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(spi_drv->hdma_rx) != HAL_OK)
		{
			hal_error_handler();
		}
		__HAL_LINKDMA(&spi_drv->handle, hdmarx, *(spi_drv->hdma_rx));
		HAL_NVIC_SetPriority(dma_rx_irq, dma_rx_irq_prio, 0);
   	 	HAL_NVIC_EnableIRQ(dma_rx_irq);
	}
}

void spi_set_speed(spi_unit_t spi, spi_freq_div_t div)
{
	uint32_t baud_rate_pre;
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	switch (div)
    {
        case SPI_DIV_2 :   	baud_rate_pre = SPI_BAUDRATEPRESCALER_2;   break;
        case SPI_DIV_4 :   	baud_rate_pre = SPI_BAUDRATEPRESCALER_4;   break;
        case SPI_DIV_8 :   	baud_rate_pre = SPI_BAUDRATEPRESCALER_8;   break;
        case SPI_DIV_16 :  	baud_rate_pre = SPI_BAUDRATEPRESCALER_16;  break;
        case SPI_DIV_32 :  	baud_rate_pre = SPI_BAUDRATEPRESCALER_32;  break;
        case SPI_DIV_64 :  	baud_rate_pre = SPI_BAUDRATEPRESCALER_64;  break;
        case SPI_DIV_128 : 	baud_rate_pre = SPI_BAUDRATEPRESCALER_128; break;
        case SPI_DIV_256 :	baud_rate_pre = SPI_BAUDRATEPRESCALER_256; break;
        default : return; break;
    }

	if (spi_drv->handle.Init.BaudRatePrescaler == baud_rate_pre)
	{
		return;
	}

	spi_drv->handle.Init.BaudRatePrescaler = baud_rate_pre;
	CLEAR_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_SPE);
	CLEAR_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_BR_Msk);
	SET_BIT(spi_drv->handle.Instance->CR1, baud_rate_pre);
	SET_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_SPE);
}

void spi_set_data_width(spi_unit_t spi, spi_data_width_t width)
{
	uint32_t data_size;
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	switch (width)
	{
		case SPI_DATA_WIDTH_8BIT : data_size = SPI_DATASIZE_8BIT; break;
		case SPI_DATA_WIDTH_16BIT : data_size = SPI_DATASIZE_16BIT; break;
		default : return; break;
	}

	if (spi_drv->handle.Init.DataSize == data_size)
	{
		return;
	}

	spi_drv->handle.Init.DataSize = data_size;
	CLEAR_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_SPE);
	if (data_size == SPI_DATASIZE_8BIT)
	{
		CLEAR_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_DFF);
	}
	else
	{
		SET_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_DFF);
	}
	SET_BIT(spi_drv->handle.Instance->CR1, SPI_CR1_SPE);

	DMA_HandleTypeDef *dma_tx = spi_drv->hdma_tx;
	if (dma_tx != NULL)
	{
		CLEAR_BIT(dma_tx->Instance->CCR, DMA_CCR_EN);
		CLEAR_BIT(dma_tx->Instance->CCR, DMA_CCR_PSIZE_Msk);
		if (data_size == SPI_DATASIZE_16BIT)
		{
			SET_BIT(dma_tx->Instance->CCR, DMA_CCR_PSIZE_0);
		}
		CLEAR_BIT(dma_tx->Instance->CCR, DMA_CCR_MSIZE_Msk);
		if (data_size == SPI_DATASIZE_16BIT)
		{
			SET_BIT(dma_tx->Instance->CCR, DMA_CCR_MSIZE_0);
		}
	}

	DMA_HandleTypeDef *dma_rx = spi_drv->hdma_rx;
	if (dma_rx != NULL)
	{
		CLEAR_BIT(dma_rx->Instance->CCR, DMA_CCR_EN);
		CLEAR_BIT(dma_rx->Instance->CCR, DMA_CCR_PSIZE_Msk);
		if (data_size == SPI_DATASIZE_16BIT)
		{
			SET_BIT(dma_rx->Instance->CCR, DMA_CCR_PSIZE_0);
		}
		CLEAR_BIT(dma_rx->Instance->CCR, DMA_CCR_MSIZE_Msk);
		if (data_size == SPI_DATASIZE_16BIT)
		{
			SET_BIT(dma_rx->Instance->CCR, DMA_CCR_MSIZE_0);
		}
	}
}

void spi_send_poll(spi_unit_t spi, const void *buf, uint32_t size)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	HAL_SPI_Transmit(&spi_drv->handle, buf, size, 10000);
}

void spi_recv_poll(spi_unit_t spi, void *buf, uint32_t size)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	HAL_SPI_Receive(&spi_drv->handle, buf, size, 10000);
}

void spi_send_dma(spi_unit_t spi, const void *buf, uint32_t size, cb_t func)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	spi_drv->func = func;
	HAL_SPI_Transmit_DMA(&spi_drv->handle, buf, size);
}

void spi_recv_dma(spi_unit_t spi, void *buf, uint32_t size, cb_t func)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return;
	}

	spi_drv->func = func;
	HAL_SPI_Receive_DMA(&spi_drv->handle, buf, size);
}

bool spi_is_busy(spi_unit_t spi)
{
	spi_drv_t *spi_drv = spi_select(spi);

	if (spi_drv == NULL)
	{
		return false;
	}
	return (spi_drv->handle.State == HAL_SPI_STATE_READY) ? false : true;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	spi_drv_t *spi_drv = spi_select_by_handle(hspi);

	if (spi_drv == NULL)
	{
		return;
	}
	if (spi_drv->func)
	{
		spi_drv->func();
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	spi_drv_t *spi_drv = spi_select_by_handle(hspi);

	if (spi_drv == NULL)
	{
		return;
	}
	if (spi_drv->func)
	{
		spi_drv->func();
	}
}

#if SPI1_EN == 1 && SPI1_DMA_TX_EN == 1
void SPI1_DMA_TX_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&spi1_dma_tx);
}
#endif

#if SPI1_EN == 1 && SPI1_DMA_RX_EN == 1
void SPI1_DMA_RX_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&spi1_dma_rx);
}
#endif

#if SPI2_EN == 1 && SPI2_DMA_TX_EN == 1
void SPI2_DMA_TX_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&spi2_dma_tx);
}
#endif

#if SPI2_EN == 1 && SPI2_DMA_RX_EN == 1
void SPI2_DMA_RX_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&spi2_dma_rx);
}
#endif

#if SPI3_EN == 1 && SPI3_DMA_TX_EN == 1
void SPI3_DMA_TX_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&spi3_dma_tx);
}
#endif

#if SPI3_EN == 1 && SPI3_DMA_RX_EN == 1
void SPI3_DMA_RX_IRQHANDLER(void)
{
	HAL_DMA_IRQHandler(&spi3_dma_rx);
}
#endif