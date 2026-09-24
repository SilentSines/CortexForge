#ifndef I2C_H
#define I2C_H

typedef enum 
{
    I2C_ADDR_8BIT = 0,   
    I2C_ADDR_16BIT,  
} i2c_reg_mode_t;

// void i2c_init(I2C_TypeDef *i2c, uint32_t speed);
// void i2c_send(I2C_TypeDef *i2c, uint8_t slave_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size);
// void i2c_recv(I2C_TypeDef *i2c, uint8_t slave_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size);
// bool i2c_check(I2C_TypeDef *i2c, uint8_t slave_addr);

#define	SOFT_I2C_SIGNAL	    1
#define	SOFT_I2C_MULTIPLE   0

#if SOFT_I2C_SIGNAL == 1
    void soft_i2c_init(void);
    void soft_i2c_send(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, const uint8_t *buf, uint16_t size);
    void soft_i2c_recv(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, uint8_t *buf, uint16_t size);
    bool soft_i2c_check(uint8_t slave_addr);
#elif SOFT_I2C_MULTIPLE == 1
    typedef struct
    {
        GPIO_TypeDef *scl_port;
	    uint32_t scl_pin;				
        GPIO_TypeDef *sda_port;
	    uint32_t sda_pin;			
        uint8_t delay;								
    } soft_i2c_t;

    void soft_i2c_init(soft_i2c_t *i2c);
    void soft_i2c_send(soft_i2c_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, const uint8_t *buf, uint16_t size);
    void soft_i2c_recv(soft_i2c_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, uint8_t *buf, uint16_t size);
    bool soft_i2c_check(soft_i2c_t *i2c, uint8_t slave_addr);
#endif

#endif