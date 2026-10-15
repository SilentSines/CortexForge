#include "hal.h"
#include "uart_cfg.h"

#if UART1_EN == 1
static uart_drv_t uart_drv1 = {USART1, NULL, NULL, UART1_TX_BUF_SIZE, UART1_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf1[UART1_TX_BUF_SIZE];		
static uint8_t rx_buf1[UART1_RX_BUF_SIZE];
#endif

#if UART2_EN == 1
static uart_drv_t uart_drv2 = {USART2, NULL, NULL, UART2_TX_BUF_SIZE, UART2_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf2[UART2_TX_BUF_SIZE];		
static uint8_t rx_buf2[UART2_RX_BUF_SIZE];
#endif

#if UART3_EN == 1
static uart_drv_t uart_drv3 = {USART3, NULL, NULL, UART3_TX_BUF_SIZE, UART3_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf3[UART3_TX_BUF_SIZE];		
static uint8_t rx_buf3[UART3_RX_BUF_SIZE];
#endif

#if UART4_EN == 1
static uart_drv_t uart_drv4 = {UART4, NULL, NULL, UART4_TX_BUF_SIZE, UART4_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf4[UART4_TX_BUF_SIZE];		
static uint8_t rx_buf4[UART4_RX_BUF_SIZE];
#endif

#if UART5_EN == 1
static uart_drv_t uart_drv5 = {UART5, NULL, NULL, UART5_TX_BUF_SIZE, UART5_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf5[UART5_TX_BUF_SIZE];		
static uint8_t rx_buf5[UART5_RX_BUF_SIZE];
#endif

#if UART6_EN == 1
static uart_drv_t uart_drv6 = {USART6, NULL, NULL, UART6_TX_BUF_SIZE, UART6_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf6[UART6_TX_BUF_SIZE];		
static uint8_t rx_buf6[UART6_RX_BUF_SIZE];
#endif

#if UART7_EN == 1
static uart_drv_t uart_drv7 = {UART7, NULL, NULL, UART7_TX_BUF_SIZE, UART7_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf7[UART7_TX_BUF_SIZE];		
static uint8_t rx_buf7[UART7_RX_BUF_SIZE];
#endif

#if UART8_EN == 1
static uart_drv_t uart_drv8 = {UART8, NULL, NULL, UART8_TX_BUF_SIZE, UART8_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf8[UART8_TX_BUF_SIZE];		
static uint8_t rx_buf8[UART8_RX_BUF_SIZE];
#endif

static uart_drv_t* uart_select(uart_unit_t uart)
{
    switch (uart)
    {
    #if UART1_EN == 1
        case UART_UNIT_1 : return &uart_drv1; break;
    #endif

    #if UART2_EN == 1
        case UART_UNIT_2 : return &uart_drv2; break;
    #endif

    #if UART3_EN == 1
        case UART_UNIT_3 : return &uart_drv3; break;
    #endif

    #if UART4_EN == 1
        case UART_UNIT_4 : return &uart_drv4; break;
    #endif

    #if UART5_EN == 1
        case UART_UNIT_5 : return &uart_drv5; break;
    #endif

    #if UART6_EN == 1
        case UART_UNIT_6 : return &uart_drv6; break;
    #endif

    #if UART7_EN == 1
        case UART_UNIT_7 : return &uart_drv7; break;
    #endif

    #if UART8_EN == 1
        case UART_UNIT_8 : return &uart_drv8; break;
    #endif

        default : return NULL; break;
    }
}

void uart_bus_init(uart_unit_t uart, uint32_t baud, uint32_t irqp)
{
	IRQn_Type irq;
	uint32_t tx_pin, rx_pin;
	uint32_t tx_af, rx_af;
	UART_HandleTypeDef huart = {0};	
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return;
	}

	switch (uart)
	{
		case UART_UNIT_1 :	__HAL_RCC_USART1_CLK_ENABLE();
							irq = USART1_IRQn;
							tx_pin = UART1_TX_PIN;
							tx_af = GPIO_AF7_USART1;
							rx_pin = UART1_RX_PIN;
							rx_af = GPIO_AF7_USART1;
						#if UART1_EN == 1
							uart_drv->tx_ring_buf = tx_buf1;
							uart_drv->rx_ring_buf = rx_buf1;
						#endif
							break;
	
		case UART_UNIT_2 :	__HAL_RCC_USART2_CLK_ENABLE();
							irq = USART2_IRQn;
							tx_pin = UART2_TX_PIN;
							tx_af = GPIO_AF7_USART2;
							rx_pin = UART2_RX_PIN;
							rx_af = GPIO_AF7_USART2;
						#if UART2_EN == 1
							uart_drv->tx_ring_buf = tx_buf2;
							uart_drv->rx_ring_buf = rx_buf2;
						#endif
							break;	

		case UART_UNIT_3 :	__HAL_RCC_USART3_CLK_ENABLE();
							irq = USART3_IRQn;
							tx_pin = UART3_TX_PIN;
							tx_af = GPIO_AF7_USART3;
							rx_pin = UART3_RX_PIN;
							rx_af = GPIO_AF7_USART3;
						#if UART3_EN == 1
							uart_drv->tx_ring_buf = tx_buf3;
							uart_drv->rx_ring_buf = rx_buf3;
						#endif
							break;
	
		case UART_UNIT_4 :	__HAL_RCC_UART4_CLK_ENABLE();
							irq = UART4_IRQn;
							tx_pin = UART4_TX_PIN;
							tx_af = GPIO_AF8_UART4;
							rx_pin = UART4_RX_PIN;
							rx_af = GPIO_AF8_UART4;
						#if UART4_EN == 1
							uart_drv->tx_ring_buf = tx_buf4;
							uart_drv->rx_ring_buf = rx_buf4;
						#endif	
							break;

		case UART_UNIT_5 :	__HAL_RCC_UART5_CLK_ENABLE();
							irq = UART5_IRQn;
							tx_pin = UART5_TX_PIN;
							tx_af = GPIO_AF8_UART5;
							rx_pin = UART5_RX_PIN;
							rx_af = GPIO_AF8_UART5;
						#if UART5_EN == 1
							uart_drv->tx_ring_buf = tx_buf5;
							uart_drv->rx_ring_buf = rx_buf5;
						#endif	
							break;

		case UART_UNIT_6 :	__HAL_RCC_USART6_CLK_ENABLE();
							irq = USART6_IRQn;
							tx_pin = UART6_TX_PIN;
							tx_af = GPIO_AF8_USART6;
							rx_pin = UART6_RX_PIN;
							rx_af = GPIO_AF8_USART6;
						#if UART6_EN == 1
							uart_drv->tx_ring_buf = tx_buf6;
							uart_drv->rx_ring_buf = rx_buf6;
						#endif	
							break;

		case UART_UNIT_7 :	__HAL_RCC_UART7_CLK_ENABLE();
							irq = UART7_IRQn;
							tx_pin = UART7_TX_PIN;
							tx_af = GPIO_AF8_UART7;
							rx_pin = UART7_RX_PIN;
							rx_af = GPIO_AF8_UART7;
						#if UART7_EN == 1
							uart_drv->tx_ring_buf = tx_buf7;
							uart_drv->rx_ring_buf = rx_buf7;
						#endif	
							break;

		case UART_UNIT_8 :	__HAL_RCC_UART8_CLK_ENABLE();
							irq = UART8_IRQn;
							tx_pin = UART8_TX_PIN;
							tx_af = GPIO_AF8_UART8;
							rx_pin = UART8_RX_PIN;
							rx_af = GPIO_AF8_UART8;
						#if UART8_EN == 1
							uart_drv->tx_ring_buf = tx_buf8;
							uart_drv->rx_ring_buf = rx_buf8;
						#endif	
							break;	
	
		default : return; break;
	}
	
	gpio_pin_init(tx_pin, GPIO_AF_PP, tx_af);
	gpio_pin_init(rx_pin, GPIO_AF_PP, rx_af);

	huart.Instance = uart_drv->instance;
	huart.Init.BaudRate = baud;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart) != HAL_OK)
	{
		hal_error_handler();
	}
	CLEAR_BIT(uart_drv->instance->SR, USART_SR_TC);   
    CLEAR_BIT(uart_drv->instance->SR, USART_SR_RXNE);
	SET_BIT(uart_drv->instance->CR1, USART_CR1_RXNEIE);

	HAL_NVIC_SetPriority(irq, irqp, 0);
	HAL_NVIC_EnableIRQ(irq);
}

void uart_send_buf(uart_unit_t uart, const uint8_t *buf, uint32_t size)
{
	uint32_t i;
	__IO uint32_t count;
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return;
	}

	for (i = 0; i < size; i++)
	{
		while (1)
		{
			 
			CRITICAL_SECTION(count = uart_drv->tx_count);
			if (count < uart_drv->tx_buf_size)
			{
				break;
			}
			else if (count == uart_drv->tx_buf_size)
			{
				SET_BIT(uart_drv->instance->CR1, USART_CR1_TXEIE);
			}
		}

		CRITICAL_SECTION(
		uart_drv->tx_ring_buf[uart_drv->tx_write] = buf[i];
		if (++uart_drv->tx_write >= uart_drv->tx_buf_size)
		{
			uart_drv->tx_write = 0;
		}
		uart_drv->tx_count++);
		
	}
	SET_BIT(uart_drv->instance->CR1, USART_CR1_TXEIE);
}

void uart_send_byte(uart_unit_t uart, uint8_t data)
{
	uart_send_buf(uart, &data, 1);
}

uint32_t uart_recv(uart_unit_t uart, uint8_t *buf, uint32_t size)
{
    uint32_t i, count, recv_len;
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return 0;
	}
	
    CRITICAL_SECTION(count = uart_drv->rx_count);
    recv_len = (count < size) ? count : size;
    for (i = 0; i < recv_len; i++) 
    {
        buf[i] = uart_drv->rx_ring_buf[uart_drv->rx_read];
        CRITICAL_SECTION(
        if (++uart_drv->rx_read >= uart_drv->rx_buf_size) 
        {
            uart_drv->rx_read = 0;
        }
        uart_drv->rx_count--);
    }
    return recv_len;
}

bool uart_recv_byte(uart_unit_t uart, uint8_t *data)
{
	uint32_t len = uart_recv(uart, data, 1);
	return (len == 1) ? true : false;
}

bool uart_peek_rx(uart_unit_t uart, uint8_t *buf, uint32_t size)
{
	uint32_t i, read, count;
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return false;
	}
	CRITICAL_SECTION(count = uart_drv->rx_count);
	if (count < size) 
	{
		return false;
	}

	read = uart_drv->rx_read;
	for (i = 0; i < size; i++)
	{
		buf[i] = uart_drv->rx_ring_buf[read];
		if (++read >= uart_drv->rx_buf_size)
		{
			read = 0;
		}
	}
	return true;
}

void uart_discard_rx(uart_unit_t uart, uint32_t size)
{
	uint32_t count;
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return;
	}
	CRITICAL_SECTION(count = uart_drv->rx_count);
	if (count == 0) 
	{
		return;
	}
	if (count > size)
	{
		count = size;
	}
	CRITICAL_SECTION(
	uart_drv->rx_read += count;
	if (uart_drv->rx_read >= uart_drv->rx_buf_size)
	{
		uart_drv->rx_read -= uart_drv->rx_buf_size;
	}
	uart_drv->rx_count -= count);
}

void uart_clr_tx(uart_unit_t uart)
{
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return;
	}
	CRITICAL_SECTION(
	uart_drv->tx_write = 0;
	uart_drv->tx_read = 0;
	uart_drv->tx_count = 0);
}

void uart_clr_rx(uart_unit_t uart)
{
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return;
	}
	CRITICAL_SECTION(
	uart_drv->rx_write = 0;
	uart_drv->rx_read = 0;
	uart_drv->rx_count = 0);
}

bool uart_tx_empty(uart_unit_t uart)
{
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return false;
	}
	return uart_drv->busy;
}

bool uart_rx_available(uart_unit_t uart)
{
	uint32_t count;
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return false;
	}

	CRITICAL_SECTION(count = uart_drv->rx_count);
	return (count > 0) ? true : false;
}

static void uart_irq(uart_drv_t *uart_drv)
{
	uint8_t data;

	if (uart_drv == NULL)
	{
		return;
	}

	uint32_t sr = READ_REG(uart_drv->instance->SR);
	uint32_t cr1 = READ_REG(uart_drv->instance->CR1);

	if ((sr & USART_SR_RXNE) != RESET)
	{
		data = READ_REG(uart_drv->instance->DR);
		uart_drv->rx_ring_buf[uart_drv->rx_write] = data;
		if (++uart_drv->rx_write >= uart_drv->rx_buf_size)
		{
			uart_drv->rx_write = 0;
		}
		if (uart_drv->rx_count < uart_drv->rx_buf_size)
		{
			uart_drv->rx_count++;
		}
	}

	if (((sr & USART_SR_TXE) != RESET) && ((cr1 & USART_CR1_TXEIE) != RESET))
	{
		if (uart_drv->tx_count == 0)
		{
			CLEAR_BIT(uart_drv->instance->CR1, USART_CR1_TXEIE);
			SET_BIT(uart_drv->instance->CR1, USART_CR1_TCIE);
		}
		else
		{
			CLEAR_BIT(uart_drv->instance->CR1, USART_CR1_TCIE);
			uart_drv->busy = 1;
			uart_drv->instance->DR = uart_drv->tx_ring_buf[uart_drv->tx_read];
			if (++uart_drv->tx_read >= uart_drv->tx_buf_size)
			{
				uart_drv->tx_read = 0;
			}
			uart_drv->tx_count--;
		}
	}

	if (((sr & USART_SR_TC) != RESET) && ((cr1 & USART_CR1_TCIE) != RESET))
	{
		if (uart_drv->tx_count == 0)
		{
			CLEAR_BIT(uart_drv->instance->CR1, USART_CR1_TCIE);
			uart_drv->busy = 0;
		}
		else
		{
			SET_BIT(uart_drv->instance->CR1, USART_CR1_TXEIE);
		}
	}
}

#if UART1_EN == 1
void USART1_IRQHandler(void)
{
	uart_irq(&uart_drv1);
}
#endif

#if UART2_EN == 1
void USART2_IRQHandler(void)
{
	uart_irq(&uart_drv2);
}
#endif

#if UART3_EN == 1
void USART3_IRQHandler(void)
{
	uart_irq(&uart_drv3);
}
#endif

#if UART4_EN == 1
void UART4_IRQHandler(void)
{
	uart_irq(&uart_drv4);
}
#endif

#if UART5_EN == 1
void UART5_IRQHandler(void)
{
	uart_irq(&uart_drv5);
}
#endif

#if UART6_EN == 1
void USART6_IRQHandler(void)
{
	uart_irq(&uart_drv6);
}
#endif

#if UART7_EN == 1
void UART7_IRQHandler(void)
{
	uart_irq(&uart_drv7);
}
#endif

#if UART8_EN == 1
void UART8_IRQHandler(void)
{
	uart_irq(&uart_drv8);
}
#endif

#ifdef STDIO_COM
#ifdef ARM
int fputc(int ch, FILE *f)
{
	UNUSED(f);
	uart_send_byte(STDIO_COM, ch);
	return ch;
}

int fgetc(FILE *f)
{
	uint8_t data;

	UNUSED(f);
	while (uart_recv_byte(STDIO_COM, &data) == false);
	return data;
}
#elif defined(GCC)
int _write( int file, char *ptr, int len)
{
    UNUSED(file); 
    uart_send(STDIO_COM, ptr, len);
	return len;
}

int _read(int file, char *ptr, int len)
{
    UNUSED(file); 
	int total = len;
	while (total > 0)
	{
		int recv_len = uart_recv(STDIO_COM, ptr, total);
		total -= recv_len;
		ptr += recv_len;
	}
	return len;
}
#endif
#endif