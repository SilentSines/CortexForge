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

#define	I2C_SW_FIXED	    1
#define	I2C_SW_INSTANCE   0

#if I2C_SW_FIXED == 1
    void i2c_sw_init(void);
    void i2c_sw_send(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, const uint8_t *buf, uint16_t size);
    void i2c_sw_recv(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, uint8_t *buf, uint16_t size);
    bool i2c_sw_check(uint8_t slave_addr);
#elif I2C_SW_INSTANCE == 1
    typedef struct
    {
        GPIO_TypeDef *scl_port;
	    uint32_t scl_pin;				
        GPIO_TypeDef *sda_port;
	    uint32_t sda_pin;			
        uint8_t delay;								
    } i2c_sw_t;

    void i2c_sw_init(i2c_sw_t *i2c);
    void i2c_sw_send(i2c_sw_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, const uint8_t *buf, uint16_t size);
    void i2c_sw_recv(i2c_sw_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_mode_t reg_mode, uint8_t *buf, uint16_t size);
    bool i2c_sw_check(i2c_sw_t *i2c, uint8_t slave_addr);
#endif

#endif