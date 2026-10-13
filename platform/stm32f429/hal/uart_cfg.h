#ifndef UART_CFG_H
#define UART_CFG_H

#define UART1_EN 0 /* USART1 */
#define UART2_EN 0 /* USART2 */
#define UART3_EN 0 /* USART3 */
#define UART4_EN 0 /* UART4 */
#define UART5_EN 0 /* UART5 */
#define UART6_EN 0 /* USART6 */
#define UART7_EN 0 /* UART7 */
#define UART8_EN 0 /* UART8 */

#define STDIO_COM COM1 // printf fun

/****************UART1 CONFIG****************/
#define UART1_TX_BUF_SIZE	1024
#define UART1_RX_BUF_SIZE	1024

#define UART1_TX_PIN    GPIO_PA9
// #define UART1_TX_PIN    GPIO_PB6

#define UART1_RX_PIN    GPIO_PA10
// #define UART1_RX_PIN    GPIO_PB7
/********************************/

/****************UART2 CONFIG****************/
#define UART2_TX_BUF_SIZE	1024
#define UART2_RX_BUF_SIZE	1024

#define UART2_TX_PIN    GPIO_PA2
// #define UART2_TX_PIN    GPIO_PD5

#define UART2_RX_PIN    GPIO_PA3
// #define UART2_RX_PIN    GPIO_PD6
/********************************/

/****************UART3 CONFIG****************/
#define UART3_TX_BUF_SIZE	1024
#define UART3_RX_BUF_SIZE	1024

#define UART3_TX_PIN    GPIO_PB10
// #define UART3_TX_PIN    GPIO_PC10
// #define UART3_TX_PIN    GPIO_PD8

#define UART3_RX_PIN    GPIO_PB11
// #define UART3_RX_PIN    GPIO_PC11
// #define UART3_RX_PIN    GPIO_PD9
/********************************/

/***************UART4 CONFIG***************/
#define UART4_TX_BUF_SIZE	1024
#define UART4_RX_BUF_SIZE	1024

#define UART4_TX_PIN    GPIO_PA0
// #define UART4_TX_PIN    GPIO_PC10

#define UART4_RX_PIN    GPIO_PA1
// #define UART4_RX_PIN    GPIO_PC11
/********************************/

/****************UART5 CONFIG****************/
#define UART5_TX_BUF_SIZE	1024
#define UART5_RX_BUF_SIZE	1024

#define UART5_TX_PIN    GPIO_PC12

#define UART5_RX_PIN    GPIO_PD2
/********************************/

/****************UART6 CONFIG****************/
#define UART6_TX_BUF_SIZE	1024
#define UART6_RX_BUF_SIZE	1024

#define UART6_TX_PIN    GPIO_PC6
// #define UART6_TX_PIN    GPIO_PG14

#define UART6_RX_PIN    GPIO_PC7
// #define UART6_RX_PIN    GPIO_PG9
/********************************/

/****************UART7 CONFIG****************/
#define UART7_TX_BUF_SIZE	1024
#define UART7_RX_BUF_SIZE	1024

#define UART7_TX_PIN    GPIO_PE8
// #define UART7_TX_PIN    GPIO_PF7

#define UART7_RX_PIN    GPIO_PE7
// #define UART7_RX_PIN    GPIO_PF6
/********************************/

/****************UART8 CONFIG****************/
#define UART8_TX_BUF_SIZE	1024
#define UART8_RX_BUF_SIZE	1024

#define UART8_TX_PIN    GPIO_PE1

#define UART8_RX_PIN    GPIO_PE0
/********************************/

typedef struct
{
	USART_TypeDef *uart;
	uint8_t *tx_ring_buf;					
	uint8_t *rx_ring_buf;
	uint32_t tx_buf_size;
	uint32_t rx_buf_size;					 			 
	__IO uint32_t tx_write;
	__IO uint32_t tx_read;	
 	__IO uint32_t tx_count; 
	__IO uint32_t rx_write; 
	__IO uint32_t rx_read;
	__IO uint32_t rx_count; 
	bool busy;								
} uart_t;

#endif