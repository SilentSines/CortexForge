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
	gpio_pin_t tx_pin, rx_pin;
	uart_drv_t *uart_drv = uart_select(uart);

	if(uart_drv == NULL)
	{
		return;
	}

	switch (uart)
	{
		case UART_UNIT_1 :	crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
							irq = USART1_IRQn;
						#if UART1_PIN_GROUP_1
							tx_pin = GPIO_PA9;
							rx_pin = GPIO_PA10;
						#elif UART1_PIN_GROUP_2
							gpio_pin_remap_config(USART1_GMUX_0001, TRUE);
							tx_pin = GPIO_PB6;
							rx_pin = GPIO_PB7;
						#endif
						#if UART1_EN == 1
							uart_drv->tx_ring_buf = tx_buf1;
							uart_drv->rx_ring_buf = rx_buf1;
						#endif
							break;
	
		case UART_UNIT_2 :	crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
							irq = USART2_IRQn;
						#if UART2_PIN_GROUP_1
							tx_pin = GPIO_PA2;
							rx_pin = GPIO_PA3;
						#elif UART2_PIN_GROUP_2
							gpio_pin_remap_config(USART2_GMUX_0001, TRUE);
							tx_pin = GPIO_PD5;
							rx_pin = GPIO_PD6;
						#endif
						#if UART2_EN == 1
							uart_drv->tx_ring_buf = tx_buf2;
							uart_drv->rx_ring_buf = rx_buf2;
						#endif
							break;	

		case UART_UNIT_3 :	crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);
							irq = USART3_IRQn;
						#if UART3_PIN_GROUP_1
							tx_pin = GPIO_PB10;
							rx_pin = GPIO_PB11;
						#elif UART3_PIN_GROUP_2
							gpio_pin_remap_config(USART3_GMUX_0001, TRUE);
							tx_pin = GPIO_PC10;
							rx_pin = GPIO_PC11;
						#elif UART3_PIN_GROUP_3
							gpio_pin_remap_config(USART3_GMUX_0011, TRUE);
							tx_pin = GPIO_PD8;
							rx_pin = GPIO_PD9;
						#endif
						#if UART3_EN == 1
							uart_drv->tx_ring_buf = tx_buf3;
							uart_drv->rx_ring_buf = rx_buf3;
						#endif
							break;
	
		case UART_UNIT_4 :	crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE);
							irq = UART4_IRQn;
						#if UART4_PIN_GROUP_1
							tx_pin = GPIO_PC10;
							rx_pin = GPIO_PC11;
						#elif UART4_PIN_GROUP_2
							gpio_pin_remap_config(UART4_GMUX_0010, TRUE);
							tx_pin = GPIO_PA0;
							rx_pin = GPIO_PA11;
						#endif
						#if UART4_EN == 1
							uart_drv->tx_ring_buf = tx_buf4;
							uart_drv->rx_ring_buf = rx_buf4;
						#endif	
							break;

		case UART_UNIT_5 :	crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE);
							irq = UART5_IRQn;
						#if UART5_PIN_GROUP_1
							tx_pin = GPIO_PC12;
							rx_pin = GPIO_PD2;
						#elif UART5_PIN_GROUP_2
							gpio_pin_remap_config(UART5_GMUX_0001, TRUE);
							tx_pin = GPIO_PB9;
							rx_pin = GPIO_PB8;
						#endif
						#if UART5_EN == 1
							uart_drv->tx_ring_buf = tx_buf5;
							uart_drv->rx_ring_buf = rx_buf5;
						#endif	
							break;

		case UART_UNIT_6 :	crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, TRUE);
							irq = USART6_IRQn;
						#if UART6_PIN_GROUP_1
							tx_pin = GPIO_PC6;
							rx_pin = GPIO_PC7;
						#elif UART6_PIN_GROUP_2
							gpio_pin_remap_config(USART6_GMUX, TRUE);
							tx_pin = GPIO_PA4;
							rx_pin = GPIO_PA5;
						#endif
						#if UART6_EN == 1
							uart_drv->tx_ring_buf = tx_buf6;
							uart_drv->rx_ring_buf = rx_buf6;
						#endif	
							break;

		case UART_UNIT_7 :	crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, TRUE);
							irq = UART7_IRQn;
						#if UART7_PIN_GROUP_1
							tx_pin = GPIO_PE8;
							rx_pin = GPIO_PE7;
						#elif UART7_PIN_GROUP_2
							gpio_pin_remap_config(UART7_GMUX, TRUE);
							tx_pin = GPIO_PB4;
							rx_pin = GPIO_PB3;
						#endif
						#if UART7_EN == 1
							uart_drv->tx_ring_buf = tx_buf7;
							uart_drv->rx_ring_buf = rx_buf7;
						#endif	
							break;

		case UART_UNIT_8 :	crm_periph_clock_enable(CRM_UART8_PERIPH_CLOCK, TRUE);
							irq = UART8_IRQn;
						#if UART8_PIN_GROUP_1
							tx_pin = GPIO_PE1;
							rx_pin = GPIO_PE0;
						#elif UART8_PIN_GROUP_2
							gpio_pin_remap_config(UART8_GMUX, TRUE);
							tx_pin = GPIO_PC2;
							rx_pin = GPIO_PC3;
						#endif
						#if UART8_EN == 1
							uart_drv->tx_ring_buf = tx_buf8;
							uart_drv->rx_ring_buf = rx_buf8;
						#endif	
							break;
	
		default : return;
	}
	
	gpio_pin_init(tx_pin, GPIO_AF_PP, GPIO_AF_UART);
	gpio_pin_init(rx_pin, GPIO_IN_PULLUP, GPIO_AF_UART);

	usart_init(uart_drv->instance, baud, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_transmitter_enable(uart_drv->instance, TRUE);
	usart_receiver_enable(uart_drv->instance, TRUE);
	usart_parity_selection_config(uart_drv->instance, USART_PARITY_NONE);
	usart_hardware_flow_control_set(uart_drv->instance, USART_HARDWARE_FLOW_NONE);

	usart_flag_clear(uart_drv->instance, USART_TDC_FLAG);
	usart_flag_clear(uart_drv->instance, USART_RDBF_FLAG);
	usart_interrupt_enable(uart_drv->instance, USART_RDBF_INT, TRUE);

	nvic_irq_enable(irq, irqp, 0);

	usart_enable(uart_drv->instance, TRUE);
}

void uart_send(uart_unit_t uart, const uint8_t *buf, uint32_t size)
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
				usart_interrupt_enable(uart_drv->instance, USART_TDBE_INT, TRUE);
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
	usart_interrupt_enable(uart_drv->instance, USART_TDBE_INT, TRUE);
}

void uart_send_byte(uart_unit_t uart, uint8_t data)
{
	uart_send(uart, &data, 1);
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

	if (usart_flag_get(uart_drv->instance, USART_RDBF_FLAG) != RESET)
	{
		data = usart_data_receive(uart_drv->instance);
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

	if ((usart_flag_get(uart_drv->instance, USART_TDBE_FLAG) != RESET) && (usart_interrupt_flag_get(uart_drv->instance, USART_TDBE_FLAG) != RESET))
	{
		if (uart_drv->tx_count == 0)
		{
			usart_interrupt_enable(uart_drv->instance, USART_TDBE_INT, FALSE);
			usart_interrupt_enable(uart_drv->instance, USART_TDC_INT, TRUE);
		}
		else
		{
			usart_interrupt_enable(uart_drv->instance, USART_TDC_INT, FALSE);
			uart_drv->busy = 1;
			usart_data_transmit(uart_drv->instance, uart_drv->tx_ring_buf[uart_drv->tx_read]);
			if (++uart_drv->tx_read >= uart_drv->tx_buf_size)
			{
				uart_drv->tx_read = 0;
			}
			uart_drv->tx_count--;
		}
	}

	if ((usart_flag_get(uart_drv->instance, USART_TDC_FLAG) != RESET) && (usart_interrupt_flag_get(uart_drv->instance, USART_TDC_FLAG) != RESET))
	{
		if (uart_drv->tx_count == 0)
		{
			usart_interrupt_enable(uart_drv->instance, USART_TDC_INT, FALSE);
			uart_drv->busy = 0;
		}
		else
		{
			usart_interrupt_enable(uart_drv->instance, USART_TDBE_INT, TRUE);
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