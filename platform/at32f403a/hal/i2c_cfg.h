#ifndef I2C_CFG_H
#define I2C_CFG_H

#if SOFT_I2C_SIGNAL == 1
    #define SOFT_I2C_SCL_PIN GPIO_PB10
    #define SOFT_I2C_SDA_PIN GPIO_PB11
    #define SOFT_I2C_DELAY 2
    #define I2C_SCL_H()     GPIO_OUT_H(SOFT_I2C_SCL_PIN)
    #define I2C_SCL_L()     GPIO_OUT_L(SOFT_I2C_SCL_PIN)		
    #define I2C_SDA_H()     GPIO_OUT_H(SOFT_I2C_SDA_PIN)
    #define I2C_SDA_L()     GPIO_OUT_L(SOFT_I2C_SDA_PIN)
    #define I2C_SCL_READ()  GPIO_READ(SOFT_I2C_SCL_PIN)
    #define I2C_SDA_READ()  GPIO_READ(SOFT_I2C_SDA_PIN)
#elif SOFT_I2C_MULTIPLE == 1
    #define I2C_SCL_H(x)    GPIO_OUT_H(x->scl_pin)
    #define I2C_SCL_L(x)    GPIO_OUT_L(x->scl_pin)
    #define I2C_SDA_H(x)    GPIO_OUT_H(x->sda_pin)
    #define I2C_SDA_L(x)    GPIO_OUT_L(x->sda_pin)
    #define I2C_SCL_READ(x) GPIO_READ(x->scl_pin)
    #define I2C_SDA_READ(x) GPIO_READ(x->sda_pin)
#endif

#endif