#ifndef UART_H
#define UART_H

typedef enum
{
    UART_UNIT_1, /* USART1 */
    UART_UNIT_2, /* USART2 */
    UART_UNIT_3, /* USART3 */
    UART_UNIT_4, /* UART4  */
    UART_UNIT_5, /* UART5  */
    UART_UNIT_6, /* USART6 */
    UART_UNIT_7, /* UART7  */
    UART_UNIT_8, /* UART8  */
} uart_unit_t;

void uart_bus_init(uart_unit_t uart, uint32_t baud, uint32_t irqp);
void uart_send(uart_unit_t uart, const uint8_t *buf, uint32_t size);
void uart_send_byte(uart_unit_t uart, uint8_t data);
uint32_t uart_recv(uart_unit_t uart, uint8_t *buf, uint32_t size);
bool uart_recv_byte(uart_unit_t uart, uint8_t *data);
bool uart_peek_rx(uart_unit_t uart, uint8_t *buf, uint32_t size);
void uart_discard_rx(uart_unit_t uart, uint32_t size);
void uart_clr_tx(uart_unit_t uart);
void uart_clr_rx(uart_unit_t uart);
bool uart_tx_empty(uart_unit_t uart);
bool uart_rx_available(uart_unit_t uart);

#endif