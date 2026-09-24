#ifndef GPIO_H
#define GPIO_H

typedef enum
{
#ifdef GPIOA
    GPIO_PA0 = 0,
    GPIO_PA1,
    GPIO_PA2,
    GPIO_PA3,
    GPIO_PA4,
    GPIO_PA5,
    GPIO_PA6,
    GPIO_PA7,
    GPIO_PA8,
    GPIO_PA9,
    GPIO_PA10,
    GPIO_PA11,
    GPIO_PA12,
    GPIO_PA13,
    GPIO_PA14,
    GPIO_PA15,
#endif 
    
#ifdef GPIOB
    GPIO_PB0,
    GPIO_PB1,
    GPIO_PB2,
    GPIO_PB3,
    GPIO_PB4,
    GPIO_PB5,
    GPIO_PB6,
    GPIO_PB7,
    GPIO_PB8,
    GPIO_PB9,
    GPIO_PB10,
    GPIO_PB11,
    GPIO_PB12,
    GPIO_PB13,
    GPIO_PB14,
    GPIO_PB15,
#endif

#ifdef GPIOC
    GPIO_PC0,
    GPIO_PC1,
    GPIO_PC2,
    GPIO_PC3,
    GPIO_PC4,
    GPIO_PC5,
    GPIO_PC6,
    GPIO_PC7,
    GPIO_PC8,
    GPIO_PC9,
    GPIO_PC10,
    GPIO_PC11,
    GPIO_PC12,
    GPIO_PC13,
    GPIO_PC14,
    GPIO_PC15,
#endif
    
#ifdef GPIOD
    GPIO_PD0,
    GPIO_PD1,
    GPIO_PD2,
    GPIO_PD3,
    GPIO_PD4,
    GPIO_PD5,
    GPIO_PD6,
    GPIO_PD7,
    GPIO_PD8,
    GPIO_PD9,
    GPIO_PD10,
    GPIO_PD11,
    GPIO_PD12,
    GPIO_PD13,
    GPIO_PD14,
    GPIO_PD15,
#endif
    
#ifdef GPIOE
    GPIO_PE0,
    GPIO_PE1,
    GPIO_PE2,
    GPIO_PE3,
    GPIO_PE4,
    GPIO_PE5,
    GPIO_PE6,
    GPIO_PE7,
    GPIO_PE8,
    GPIO_PE9,
    GPIO_PE10,
    GPIO_PE11,
    GPIO_PE12,
    GPIO_PE13,
    GPIO_PE14,
    GPIO_PE15,
#endif
    
#ifdef GPIOF
    GPIO_PF0,
    GPIO_PF1,
    GPIO_PF2,
    GPIO_PF3,
    GPIO_PF4,
    GPIO_PF5,
    GPIO_PF6,
    GPIO_PF7,
    GPIO_PF8,
    GPIO_PF9,
    GPIO_PF10,
    GPIO_PF11,
    GPIO_PF12,
    GPIO_PF13,
    GPIO_PF14,
    GPIO_PF15,
#endif
    
#ifdef GPIOG
    GPIO_PG0,
    GPIO_PG1,
    GPIO_PG2,
    GPIO_PG3,
    GPIO_PG4,
    GPIO_PG5,
    GPIO_PG6,
    GPIO_PG7,
    GPIO_PG8,
    GPIO_PG9,
    GPIO_PG10,
    GPIO_PG11,
    GPIO_PG12,
    GPIO_PG13,
    GPIO_PG14,
    GPIO_PG15,
#endif

#ifdef GPIOH
    GPIO_PH0,
    GPIO_PH1,
    GPIO_PH2,
    GPIO_PH3,
    GPIO_PH4,
    GPIO_PH5,
    GPIO_PH6,
    GPIO_PH7,
    GPIO_PH8,
    GPIO_PH9,
    GPIO_PH10,
    GPIO_PH11,
    GPIO_PH12,
    GPIO_PH13,
    GPIO_PH14,
    GPIO_PH15,
#endif
    
#ifdef GPIOI
    GPIO_PI0,
    GPIO_PI1,
    GPIO_PI2,
    GPIO_PI3,
    GPIO_PI4,
    GPIO_PI5,
    GPIO_PI6,
    GPIO_PI7,
    GPIO_PI8,
    GPIO_PI9,
    GPIO_PI10,
    GPIO_PI11,
    GPIO_PI12,
    GPIO_PI13,
    GPIO_PI14,
    GPIO_PI15,
#endif
    
    GPIO_MAX, 
    GPIO_PIN_UNUSED = GPIO_MAX,
} gpio_pin_t;

typedef enum
{
    GPIO_IN_FLOATING,          
    GPIO_IN_ANALOG,           
    GPIO_IN_PULLUP,            
    GPIO_IN_PULLDOWN,          
    GPIO_OUT_PP,              
    GPIO_OUT_PP_HIGH,         
    GPIO_OUT_PP_LOW,           
    GPIO_OUT_OD,               
    GPIO_OUT_OD_HIGH,         
    GPIO_OUT_OD_LOW,       
    GPIO_AF_PP,    
    GPIO_AF_PP_PULLUP,            
    GPIO_AF_PP_PULLDOWN,                
    GPIO_AF_OD,  
    GPIO_AF_OD_PULLUP,            
    GPIO_AF_OD_PULLDOWN,                                 
} gpio_mode_t;

typedef enum
{
    GPIO_AF_NONE,
    GPIO_AF_UART,
    GPIO_AF_SPI,
    GPIO_AF_I2C,
    GPIO_AF_I2S,
    GPIO_AF_TMR,
    GPIO_AF_ADC,
    GPIO_AF_DAC,
    GPIO_AF_CAN,
    GPIO_AF_USB,
    GPIO_AF_SDIO,
    GPIO_AF_ETH,
    GPIO_AF_SAI,
    GPIO_AF_LCD,    /* RGB LCD */
    GPIO_AF_EBI,    /* 外部总线：FSMC/FMC/XMC */
    GPIO_AF_QSPI,
    GPIO_AF_MAX,
} gpio_af_t;

typedef enum
{
    EXTI_RISING,
    EXTI_FALLING,  
    EXTI_BOTH,
} exti_edge_t;

#define GPIO_HIGH  1
#define GPIO_LOW   0

#define GPIO_OUT_H(pin)  ((GPIO_TypeDef *)gpio_port_table[(pin) >> 4])->BSRR = gpio_pin_table[(pin) & 0xF]
#define GPIO_OUT_L(pin)  ((GPIO_TypeDef *)gpio_port_table[(pin) >> 4])->BSRR = (uint32_t)(gpio_pin_table[(pin) & 0xF]) << 16
#define GPIO_TOGGLE(pin) ((GPIO_TypeDef *)gpio_port_table[(pin) >> 4])->ODR ^= gpio_pin_table[(pin) & 0xF]
#define GPIO_READ(pin)   ((gpio_port_table[(pin) >> 4]->IDR & gpio_pin_table[(pin) & 0xF]) != GPIO_LOW)

extern const GPIO_TypeDef *gpio_port_table[];
extern const uint16_t gpio_pin_table[16];

void gpio_pin_init(gpio_pin_t pin, gpio_mode_t mode, uint32_t af);
void gpio_exti_init(gpio_pin_t pin, exti_edge_t edge, uint32_t irqp, cb_t func);

#endif