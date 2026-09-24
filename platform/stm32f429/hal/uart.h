#ifndef UART_H
#define UART_H

typedef enum
{
	COM1, /* USART1 */
	COM2, /* USART2 */
	COM3, /* USART3 */
	COM4, /* UART4 */
	COM5, /* UART5 */
	COM6, /* USART6 */
	COM7, /* UART7 */
	COM8, /* UART8 */
} com_port_t;

void uart_init(com_port_t port, uint32_t baud, uint32_t irqp);
void uart_send_buf(com_port_t port, const uint8_t *buf, uint32_t size);
void uart_send_byte(com_port_t port, uint8_t data);
bool uart_recv_byte(com_port_t port, uint8_t *data);
void uart_clr_tx_fifo(com_port_t port);
void uart_clr_rx_fifo(com_port_t port);
bool uart_tx_empty(com_port_t port);

#endif