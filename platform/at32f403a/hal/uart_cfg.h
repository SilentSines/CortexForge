#ifndef UART_CFG_H
#define UART_CFG_H

#define UART1_EN 1 /* USART1 */
#define UART2_EN 0 /* USART2 */
#define UART3_EN 0 /* USART3 */
#define UART4_EN 0 /* UART4 */
#define UART5_EN 0 /* UART5 */
#define UART6_EN 0 /* USART6 */
#define UART7_EN 0 /* UART7 */
#define UART8_EN 0 /* UART8 */

// #define STDIO_COM COM2 // printf fun

/****************UART1 CONFIG****************/
#define UART1_TX_BUF_SIZE	1024
#define UART1_RX_BUF_SIZE	1024

#define UART1_PIN_GROUP_1   0	//TX(PA9) + RX(PA10)
#define UART1_PIN_GROUP_2   1 	//TX(PB6) + RX(PB7)
/********************************/

/****************UART2 CONFIG****************/
#define UART2_TX_BUF_SIZE	1024
#define UART2_RX_BUF_SIZE	1024

#define UART2_PIN_GROUP_1  	1 	//TX(PA2) + RX(PA3)
#define UART2_PIN_GROUP_2   0 	//TX(PD5) + RX(PD6)
/********************************/

/****************UART3 CONFIG****************/
#define UART3_TX_BUF_SIZE	1024
#define UART3_RX_BUF_SIZE	1024

#define UART3_PIN_GROUP_1   1	//TX(PB10) + RX(PB11)
#define UART3_PIN_GROUP_2   0 	//TX(PC10) + RX(PC11)
#define UART3_PIN_GROUP_3   0 	//TX(PD8) + RX(PD9)
/********************************/

/****************UART4 CONFIG****************/
#define UART4_TX_BUF_SIZE	1024
#define UART4_RX_BUF_SIZE	1024

#define UART4_PIN_GROUP_1   1	//TX(PC10) + RX(PC11)
#define UART4_PIN_GROUP_2   0 	//TX(PA0) + RX(PA1)
/********************************/

/****************UART5 CONFIG****************/
#define UART5_TX_BUF_SIZE	1024
#define UART5_RX_BUF_SIZE	1024

#define UART5_PIN_GROUP_1   1	//TX(PC12) + RX(PD2)
#define UART5_PIN_GROUP_2   0 	//TX(PB9) + RX(PB8)
/********************************/

/****************UART6 CONFIG****************/
#define UART6_TX_BUF_SIZE	1024
#define UART6_RX_BUF_SIZE	1024

#define UART6_PIN_GROUP_1   1	//TX(PC6) + RX(PC7)
#define UART6_PIN_GROUP_2   0 	//TX(PA4) + RX(PA5)
/********************************/

/****************UART7 CONFIG****************/
#define UART7_TX_BUF_SIZE	1024
#define UART7_RX_BUF_SIZE	1024

#define UART7_PIN_GROUP_1   1	//TX(PE8) + RX(PE7)
#define UART7_PIN_GROUP_2   0 	//TX(PB4) + RX(PB3)
/********************************/

/****************UART8 CONFIG****************/
#define UART8_TX_BUF_SIZE	1024
#define UART8_RX_BUF_SIZE	1024

#define UART8_PIN_GROUP_1   1	//TX(PE1) + RX(PE0)
#define UART8_PIN_GROUP_2   0 	//TX(PC2) + RX(PC3)
/********************************/

typedef struct
{
	usart_type *uart;
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
	__IO bool busy;								
} uart_t;

#endif