#ifndef I2C_H
#define I2C_H

typedef enum
{
    I2C_REG_ADDR_8BIT = 0,
    I2C_REG_ADDR_16BIT, 
} i2c_reg_addr_width_t;

#define	I2C_SW_FIXED	  0
#define	I2C_SW_INSTANCE   1

#if I2C_SW_FIXED == 1
    void i2c_sw_init(void);
    void i2c_sw_send(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size);
    void i2c_sw_recv(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size);
    bool i2c_sw_check(uint8_t slave_addr);
#elif I2C_SW_INSTANCE == 1
    typedef struct
    {
	    gpio_pin_t scl_pin;
	    gpio_pin_t sda_pin;
        uint8_t delay;								
    } i2c_sw_t;

    void i2c_sw_init(i2c_sw_t *i2c);
    void i2c_sw_send(i2c_sw_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size);
    void i2c_sw_recv(i2c_sw_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size);
    bool i2c_sw_check(i2c_sw_t *i2c, uint8_t slave_addr);
#endif

#endif