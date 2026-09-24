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
/********UART1 TX PIN SEL********/
#define UART1_TX_PORT   GPIOA
#define UART1_TX_PIN    GPIO_PIN_9

// #define UART1_TX_PORT   GPIOB
// #define UART1_TX_PIN    GPIO_PIN_6
/********UART1 RX PIN SEL********/
#define UART1_RX_PORT   GPIOA
#define UART1_RX_PIN    GPIO_PIN_10

// #define UART1_RX_PORT   GPIOB
// #define UART1_RX_PIN    GPIO_PIN_7
/********************************/

/****************UART2 CONFIG****************/
#define UART2_TX_BUF_SIZE	1024
#define UART2_RX_BUF_SIZE	1024
/********UART2 TX PIN SEL********/
#define UART2_TX_PORT   GPIOA
#define UART2_TX_PIN    GPIO_PIN_2

// #define UART2_TX_PORT   GPIOD
// #define UART2_TX_PIN    GPIO_PIN_5
/********UART2 RX PIN SEL********/
#define UART2_RX_PORT   GPIOA
#define UART2_RX_PIN    GPIO_PIN_3

// #define UART2_RX_PORT   GPIOD
// #define UART2_RX_PIN    GPIO_PIN_6
/********************************/

/****************UART3 CONFIG****************/
#define UART3_TX_BUF_SIZE	1024
#define UART3_RX_BUF_SIZE	1024
/********UART3 TX PIN SEL********/
#define UART3_TX_PORT   GPIOB
#define UART3_TX_PIN    GPIO_PIN_10

// #define UART3_TX_PORT   GPIOC
// #define UART3_TX_PIN    GPIO_PIN_10

// #define UART3_TX_PORT   GPIOD
// #define UART3_TX_PIN    GPIO_PIN_8
/********UART3 RX PIN SEL********/
#define UART3_RX_PORT   GPIOB
#define UART3_RX_PIN    GPIO_PIN_11

// #define UART3_RX_PORT   GPIOC
// #define UART3_RX_PIN    GPIO_PIN_11

// #define UART3_RX_PORT   GPIOD
// #define UART3_RX_PIN    GPIO_PIN_9
/********************************/

/***************UART4 CONFIG***************/
#define UART4_TX_BUF_SIZE	1024
#define UART4_RX_BUF_SIZE	1024
/********UART4 TX PIN SEL********/
#define UART4_TX_PORT   GPIOA
#define UART4_TX_PIN    GPIO_PIN_0

// #define UART4_TX_PORT   GPIOC
// #define UART4_TX_PIN    GPIO_PIN_10
/********UART4 RX PIN SEL********/
#define UART4_RX_PORT   GPIOA
#define UART4_RX_PIN    GPIO_PIN_1

// #define UART4_RX_PORT   GPIOC
// #define UART4_RX_PIN    GPIO_PIN_11
/********************************/

/****************UART5 CONFIG****************/
#define UART5_TX_BUF_SIZE	1024
#define UART5_RX_BUF_SIZE	1024
/********UART5 TX PIN SEL********/
#define UART5_TX_PORT   GPIOC
#define UART5_TX_PIN    GPIO_PIN_12
/********UART5 RX PIN SEL********/
#define UART5_RX_PORT   GPIOD
#define UART5_RX_PIN    GPIO_PIN_2
/********************************/

/****************UART6 CONFIG****************/
#define UART6_TX_BUF_SIZE	1024
#define UART6_RX_BUF_SIZE	1024
/********UART6 TX PIN SEL********/
#define UART6_TX_PORT   GPIOC
#define UART6_TX_PIN    GPIO_PIN_6

// #define UART6_TX_PORT   GPIOG
// #define UART6_TX_PIN    GPIO_PIN_14
/********UART6 RX PIN SEL********/
#define UART6_RX_PORT   GPIOC
#define UART6_RX_PIN    GPIO_PIN_7

// #define UART6_RX_PORT   GPIOG
// #define UART6_RX_PIN    GPIO_PIN_9
/********************************/

/****************UART7 CONFIG****************/
#define UART7_TX_BUF_SIZE	1024
#define UART7_RX_BUF_SIZE	1024
/********UART7 TX PIN SEL********/
#define UART7_TX_PORT   GPIOE
#define UART7_TX_PIN    GPIO_PIN_8

// #define UART7_TX_PORT   GPIOF
// #define UART7_TX_PIN    GPIO_PIN_7
/********UART7 RX PIN SEL********/
#define UART7_RX_PORT   GPIOE
#define UART7_RX_PIN    GPIO_PIN_7

// #define UART7_RX_PORT   GPIOF
// #define UART7_RX_PIN    GPIO_PIN_6
/********************************/

/****************UART8 CONFIG****************/
#define UART8_TX_BUF_SIZE	1024
#define UART8_RX_BUF_SIZE	1024
/********UART8 TX PIN SEL********/
#define UART8_TX_PORT   GPIOE
#define UART8_TX_PIN    GPIO_PIN_1
/********UART8 RX PIN SEL********/
#define UART8_RX_PORT   GPIOE
#define UART8_RX_PIN    GPIO_PIN_0
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