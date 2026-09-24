#ifndef I2C_CFG_H
#define I2C_CFG_H

#if SOFT_I2C_SIGNAL == 1
    #define SOFT_I2C_SCL_PORT  GPIOH
    #define SOFT_I2C_SCL_PIN GPIO_PIN_4
    #define SOFT_I2C_SDA_PORT  GPIOH
    #define SOFT_I2C_SDA_PIN GPIO_PIN_5
    #define SOFT_I2C_DELAY 2
    #define I2C_SCL_H()  SOFT_I2C_SCL_PORT->BSRR = SOFT_I2C_SCL_PIN				
    #define I2C_SCL_L()  SOFT_I2C_SCL_PORT->BSRR = ((uint32_t)SOFT_I2C_SCL_PIN << 16)			
    #define I2C_SDA_H()  SOFT_I2C_SDA_PORT->BSRR = SOFT_I2C_SDA_PIN				
    #define I2C_SDA_L()  SOFT_I2C_SDA_PORT->BSRR = ((uint32_t)SOFT_I2C_SDA_PIN << 16)
    #define I2C_SCL_READ()  ((SOFT_I2C_SCL_PORT->IDR & SOFT_I2C_SCL_PIN) != RESET)				
    #define I2C_SDA_READ()  ((SOFT_I2C_SDA_PORT->IDR & SOFT_I2C_SDA_PIN) != RESET)	
#elif SOFT_I2C_MULTIPLE == 1
    #define I2C_SCL_H(x)  x->scl_port->BSRR = x->scl_pin				
    #define I2C_SCL_L(x)  x->scl_port->BSRR = (x->scl_pin << 16)		
    #define I2C_SDA_H(x)  x->sda_port->BSRR = x->sda_pin				
    #define I2C_SDA_L(x)  x->sda_port->BSRR = (x->sda_pin << 16)	
    #define I2C_SCL_READ(x)  ((x->scl_port->IDR & x->scl_pin) != RESET)				
    #define I2C_SDA_READ(x)  ((x->sda_port->IDR & x->sda_pin) != RESET)
#endif

#endif