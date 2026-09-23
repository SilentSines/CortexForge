#include "hal.h"
#include "uart_cfg.h"

#if UART1_EN == 1
static uart_t uart1 = {USART1, NULL, NULL, UART1_TX_BUF_SIZE, UART1_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf1[UART1_TX_BUF_SIZE];		
static uint8_t rx_buf1[UART1_RX_BUF_SIZE];
#endif

#if UART2_EN == 1
static uart_t uart2 = {USART2, NULL, NULL, UART2_TX_BUF_SIZE, UART2_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf2[UART2_TX_BUF_SIZE];		
static uint8_t rx_buf2[UART2_RX_BUF_SIZE];
#endif

#if UART3_EN == 1
static uart_t uart3 = {USART3, NULL, NULL, UART3_TX_BUF_SIZE, UART3_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf3[UART3_TX_BUF_SIZE];		
static uint8_t rx_buf3[UART3_RX_BUF_SIZE];
#endif

#if UART4_EN == 1
static uart_t uart4 = {UART4, NULL, NULL, UART4_TX_BUF_SIZE, UART4_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf4[UART4_TX_BUF_SIZE];		
static uint8_t rx_buf4[UART4_RX_BUF_SIZE];
#endif

#if UART5_EN == 1
static uart_t uart5 = {UART5, NULL, NULL, UART5_TX_BUF_SIZE, UART5_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf5[UART5_TX_BUF_SIZE];		
static uint8_t rx_buf5[UART5_RX_BUF_SIZE];
#endif

#if UART6_EN == 1
static uart_t uart6 = {USART6, NULL, NULL, UART6_TX_BUF_SIZE, UART6_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf6[UART6_TX_BUF_SIZE];		
static uint8_t rx_buf6[UART6_RX_BUF_SIZE];
#endif

#if UART7_EN == 1
static uart_t uart7 = {UART7, NULL, NULL, UART7_TX_BUF_SIZE, UART7_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf7[UART7_TX_BUF_SIZE];		
static uint8_t rx_buf7[UART7_RX_BUF_SIZE];
#endif

#if UART8_EN == 1
static uart_t uart8 = {UART8, NULL, NULL, UART8_TX_BUF_SIZE, UART8_RX_BUF_SIZE, 0, 0, 0, 0, 0, 0, false};
static uint8_t tx_buf8[UART8_TX_BUF_SIZE];		
static uint8_t rx_buf8[UART8_RX_BUF_SIZE];
#endif

static uart_t* uart_select(com_port_t port)
{
	uart_t *com = NULL;

	switch (port)
	{
	#if UART1_EN == 1
        case COM1 : com = &uart1; break;
    #endif

    #if UART2_EN == 1
        case COM2 : com = &uart2; break;
    #endif

    #if UART3_EN == 1
        case COM3 : com = &uart3; break;
    #endif

	#if UART4_EN == 1
        case COM4 : com = &uart4; break;
    #endif

	#if UART5_EN == 1
        case COM5 : com = &uart5; break;
    #endif

	#if UART6_EN == 1
        case COM6 : com = &uart6; break;
    #endif

	#if UART7_EN == 1
        case COM7 : com = &uart7; break;
    #endif

	#if UART8_EN == 1
        case COM8 : com = &uart8; break;
    #endif
		
		default : 	com = NULL; break;
	}
	return com;
}

void uart_init(com_port_t port, uint32_t baud, uint32_t irqp)
{
	IRQn_Type irq;
	crm_periph_clock_type clk;
	gpio_pin_t tx_pin, rx_pin;
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return;
	}
	switch (port)
	{
		case COM1 :	clk = CRM_USART1_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf1;
					com->rx_ring_buf = rx_buf1;
				#endif
					break;
	
		case COM2 :	clk = CRM_USART2_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf2;
					com->rx_ring_buf = rx_buf2;
				#endif
					break;	

		case COM3 :	clk = CRM_USART3_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf3;
					com->rx_ring_buf = rx_buf3;
				#endif
					break;
	
		case COM4 :	clk = CRM_UART4_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf4;
					com->rx_ring_buf = rx_buf4;
				#endif	
					break;

		case COM5 :	clk = CRM_UART5_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf5;
					com->rx_ring_buf = rx_buf5;
				#endif	
					break;

		case COM6 :	clk = CRM_USART6_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf6;
					com->rx_ring_buf = rx_buf6;
				#endif	
					break;

		case COM7 :	clk = CRM_UART7_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf7;
					com->rx_ring_buf = rx_buf7;
				#endif	
					break;

		case COM8 :	clk = CRM_UART8_PERIPH_CLOCK;
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
					com->tx_ring_buf = tx_buf8;
					com->rx_ring_buf = rx_buf8;
				#endif	
					break;
	
		default : return;
	}
	
	gpio_pin_init(tx_pin, GPIO_AF_PP, GPIO_AF_UART);
	gpio_pin_init(rx_pin, GPIO_IN_PULLUP, GPIO_AF_UART);

	crm_periph_clock_enable(clk, TRUE);

	usart_init(com->uart, baud, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_transmitter_enable(com->uart, TRUE);
	usart_receiver_enable(com->uart, TRUE);
	usart_parity_selection_config(com->uart, USART_PARITY_NONE);
	usart_hardware_flow_control_set(com->uart, USART_HARDWARE_FLOW_NONE);

	usart_flag_clear(com->uart, USART_TDC_FLAG);
	usart_flag_clear(com->uart, USART_RDBF_FLAG);
	usart_interrupt_enable(com->uart, USART_RDBF_INT, TRUE);

	nvic_irq_enable(irq, irqp, 0);

	usart_enable(com->uart, TRUE);
}

void uart_send(com_port_t port, const uint8_t *buf, uint32_t size)
{
	uint32_t i;
	__IO uint32_t count;
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return;
	}

	for (i = 0; i < size; i++)
	{
		while (1)
		{
			 
			CRITICAL_SECTION(count = com->tx_count);
			if (count < com->tx_buf_size)
			{
				break;
			}
			else if (count == com->tx_buf_size)
			{
				usart_interrupt_enable(com->uart, USART_TDBE_INT, TRUE);
			}
		}

		CRITICAL_SECTION(
		com->tx_ring_buf[com->tx_write] = buf[i];
		if (++com->tx_write >= com->tx_buf_size)
		{
			com->tx_write = 0;
		}
		com->tx_count++);
		
	}
	usart_interrupt_enable(com->uart, USART_TDBE_INT, TRUE);
}

void uart_send_byte(com_port_t port, uint8_t data)
{
	uart_send(port, &data, 1);
}

uint32_t uart_recv(com_port_t port, uint8_t *buf, uint32_t size)
{
    uint32_t i, count, recv_len;
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return 0;
	}
    CRITICAL_SECTION(count = com->rx_count);
    recv_len = (count < size) ? count : size;
    for (i = 0; i < recv_len; i++) 
    {
        buf[i] = com->rx_ring_buf[com->rx_read];
        CRITICAL_SECTION(
        if (++com->rx_read >= com->rx_buf_size) 
        {
            com->rx_read = 0;
        }
        com->rx_count--);
    }
    return recv_len;
}

bool uart_recv_byte(com_port_t port, uint8_t *data)
{
	uint32_t len = uart_recv(port, data, 1);
	return (len == 1) ? true : false;
}

bool uart_peek_rx(com_port_t port, uint8_t *buf, uint32_t size)
{
	uint32_t i, read, count;
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return false;
	}
	CRITICAL_SECTION(count = com->rx_count);
	if (count < size) 
	{
		return false;
	}

	read = com->rx_read;
	for (i = 0; i < size; i++)
	{
		buf[i] = com->rx_ring_buf[read];
		if (++read >= com->rx_buf_size)
		{
			read = 0;
		}
	}
	return true;
	
}

void uart_discard_rx(com_port_t port, uint32_t size)
{
	uint32_t count;
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return;
	}
	CRITICAL_SECTION(count = com->rx_count);
	if (count == 0) 
	{
		return;
	}
	if (count > size)
	{
		count = size;
	}
	CRITICAL_SECTION(
	com->rx_read += count;
	if (com->rx_read >= com->rx_buf_size)
	{
		com->rx_read -= com->rx_buf_size;
	}
	com->rx_count -= count);
}

void uart_clr_tx(com_port_t port)
{
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return;
	}
	CRITICAL_SECTION(
	com->tx_write = 0;
	com->tx_read = 0;
	com->tx_count = 0);
}

void uart_clr_rx(com_port_t port)
{
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return;
	}
	CRITICAL_SECTION(
	com->rx_write = 0;
	com->rx_read = 0;
	com->rx_count = 0);
}

bool uart_tx_empty(com_port_t port)
{
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return false;
	}
	return com->busy;
}

bool uart_rx_available(com_port_t port)
{
	uint32_t count;
	uart_t *com = uart_select(port);

	if(com == NULL)
	{
		return false;
	}

	CRITICAL_SECTION(count = com->rx_count);
	return (count > 0) ? true : false;
}

static void uart_irq(uart_t *com)
{
	uint8_t data;

	if (usart_flag_get(com->uart, USART_RDBF_FLAG) != RESET)
	{
		data = usart_data_receive(com->uart);
		com->rx_ring_buf[com->rx_write] = data;
		if (++com->rx_write >= com->rx_buf_size)
		{
			com->rx_write = 0;
		}
		if (com->rx_count < com->rx_buf_size)
		{
			com->rx_count++;
		}
	}

	if ((usart_flag_get(com->uart, USART_TDBE_FLAG) != RESET) && (usart_interrupt_flag_get(com->uart, USART_TDBE_FLAG) != RESET))
	{
		if (com->tx_count == 0)
		{
			usart_interrupt_enable(com->uart, USART_TDBE_INT, FALSE);
			usart_interrupt_enable(com->uart, USART_TDC_INT, TRUE);
		}
		else
		{
			usart_interrupt_enable(com->uart, USART_TDC_INT, FALSE);
			com->busy = 1;
			usart_data_transmit(com->uart, com->tx_ring_buf[com->tx_read]);
			if (++com->tx_read >= com->tx_buf_size)
			{
				com->tx_read = 0;
			}
			com->tx_count--;
		}
	}

	if ((usart_flag_get(com->uart, USART_TDC_FLAG) != RESET) && (usart_interrupt_flag_get(com->uart, USART_TDC_FLAG) != RESET))
	{
		if (com->tx_count == 0)
		{
			usart_interrupt_enable(com->uart, USART_TDC_INT, FALSE);
			com->busy = 0;
		}
		else
		{
			usart_interrupt_enable(com->uart, USART_TDBE_INT, TRUE);
		}
	}
}

#if UART1_EN == 1
void USART1_IRQHandler(void)
{
	uart_irq(&uart1);
}
#endif

#if UART2_EN == 1
void USART2_IRQHandler(void)
{
	uart_irq(&uart2);
}
#endif

#if UART3_EN == 1
void USART3_IRQHandler(void)
{
	uart_irq(&uart3);
}
#endif

#if UART4_EN == 1
void UART4_IRQHandler(void)
{
	uart_irq(&uart4);
}
#endif

#if UART5_EN == 1
void UART5_IRQHandler(void)
{
	uart_irq(&uart5);
}
#endif

#if UART6_EN == 1
void USART6_IRQHandler(void)
{
	uart_irq(&uart6);
}
#endif

#if UART7_EN == 1
void UART7_IRQHandler(void)
{
	uart_irq(&uart7);
}
#endif

#if UART8_EN == 1
void UART8_IRQHandler(void)
{
	uart_irq(&uart8);
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