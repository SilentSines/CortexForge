#ifndef I2C_H
#define I2C_H

typedef enum
{
    I2C_REG_ADDR_8BIT = 0,
    I2C_REG_ADDR_16BIT, 
} i2c_reg_addr_width_t;

#define	SOFT_I2C_SIGNAL	    0
#define	SOFT_I2C_MULTIPLE   1

#if SOFT_I2C_SIGNAL == 1
    void soft_i2c_init(void);
    void soft_i2c_send(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size);
    void soft_i2c_recv(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size);
    bool soft_i2c_check(uint8_t slave_addr);
#elif SOFT_I2C_MULTIPLE == 1
    typedef struct
    {
	    gpio_pin_t scl_pin;
	    gpio_pin_t sda_pin;
        uint8_t delay;								
    } soft_i2c_t;

    void soft_i2c_init(soft_i2c_t *i2c);
    void soft_i2c_send(soft_i2c_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size);
    void soft_i2c_recv(soft_i2c_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size);
    bool soft_i2c_check(soft_i2c_t *i2c, uint8_t slave_addr);
#endif

#endif