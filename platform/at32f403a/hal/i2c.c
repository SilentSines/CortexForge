#include "hal.h"
#include "i2c_cfg.h"

#if I2C_SW_FIXED == 1
static void i2c_sw_start(void)
{
	I2C_SDA_H();
	I2C_SCL_H();
	delay_us(SOFT_I2C_DELAY);
	I2C_SDA_L();
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_L();
	delay_us(SOFT_I2C_DELAY);
}

static void i2c_sw_stop(void)
{
	I2C_SDA_L();
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_H();
	delay_us(SOFT_I2C_DELAY);
	I2C_SDA_H();
	delay_us(SOFT_I2C_DELAY);
}

static void i2c_sw_send_byte(uint8_t byte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (byte & 0x80)
		{
			I2C_SDA_H();
		}
		else
		{
			I2C_SDA_L();
		}
		delay_us(SOFT_I2C_DELAY);
		I2C_SCL_H();
		delay_us(SOFT_I2C_DELAY);
		I2C_SCL_L();
		byte <<= 1;
	}
	I2C_SDA_H();
}

static uint8_t i2c_sw_recv_byte(void)
{
	uint8_t i;
	uint8_t data = 0;

	I2C_SDA_H();
	for (i = 0; i < 8; i++)
	{
		data <<= 1;
		I2C_SCL_L();
		delay_us(SOFT_I2C_DELAY);
		I2C_SCL_H();
		delay_us(SOFT_I2C_DELAY);
		if (I2C_SDA_READ())
		{
			data++;
		}
	}
	return data;
}

static bool i2c_sw_wait_ack(void)
{
	uint16_t retry = 0xFFFF;

	I2C_SDA_H();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_H();	
	delay_us(SOFT_I2C_DELAY);
	do
	{
		if (--retry == 0)
		{
			return false;
		}
	} while (I2C_SDA_READ());
	I2C_SCL_L();
	delay_us(SOFT_I2C_DELAY);
	return true;
}

static void i2c_sw_ack(void)
{
	I2C_SCL_L();
	I2C_SDA_L();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_H();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_L();
	delay_us(SOFT_I2C_DELAY);
	I2C_SDA_H();	
}

static void i2c_sw_nack(void)
{
	I2C_SCL_L();
	I2C_SDA_H();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_H();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_L();
	delay_us(SOFT_I2C_DELAY);
}

void i2c_sw_init(void)
{
	gpio_pin_init(SOFT_I2C_SCL_PIN, GPIO_OUT_OD, GPIO_AF_NONE);
	gpio_pin_init(SOFT_I2C_SDA_PIN, GPIO_OUT_OD, GPIO_AF_NONE);
    
    i2c_sw_stop();
}

void i2c_sw_send(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size)
{
    uint16_t i;

    i2c_sw_start();
    i2c_sw_send_byte((slave_addr << 1) & 0xFE);
    if (!i2c_sw_wait_ack())
	{
		goto stop;
	}
	if (reg_width == I2C_REG_ADDR_16BIT)
	{
		i2c_sw_send_byte((reg_addr >> 8) & 0xFF);
		if (!i2c_sw_wait_ack())
		{
			goto stop;
		}
	}
    i2c_sw_send_byte(reg_addr & 0xFF);
    if (!i2c_sw_wait_ack())
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        i2c_sw_send_byte(buf[i]);
        if (!i2c_sw_wait_ack())
		{
			goto stop;
		}
    }
stop:
    i2c_sw_stop();
}

void i2c_sw_recv(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size)
{
    uint16_t i;

    i2c_sw_start();
    i2c_sw_send_byte((slave_addr << 1) & 0xFE);
    if (!i2c_sw_wait_ack())
	{
		goto stop;
	}
    if (reg_width == I2C_REG_ADDR_16BIT)
	{
		i2c_sw_send_byte((reg_addr >> 8) & 0xFF);
		if (!i2c_sw_wait_ack())
		{
			goto stop;
		}
	}
    i2c_sw_send_byte(reg_addr & 0xFF);
    if (!i2c_sw_wait_ack())
	{
		goto stop;
	}

    i2c_sw_start();
    i2c_sw_send_byte((slave_addr << 1) | 0x01);
    if (!i2c_sw_wait_ack())
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        buf[i] = i2c_sw_recv_byte(); 
        if(i == (size - 1))
        {
            i2c_sw_nack();
        }
        else
        {
            i2c_sw_ack();
        }
    }
stop:
    i2c_sw_stop();
}

bool i2c_sw_check(uint8_t slave_addr)
{
	bool ack;

	i2c_sw_start();
    i2c_sw_send_byte((slave_addr << 1) & 0xFE);
    ack =  i2c_sw_wait_ack();
	i2c_sw_stop();
	return ack;
}
#elif I2C_SW_INSTANCE == 1
static void i2c_sw_start(i2c_sw_t *i2c)
{
	I2C_SDA_H(i2c);
	I2C_SCL_H(i2c);
	delay_us(i2c->delay);
	I2C_SDA_L(i2c);
	delay_us(i2c->delay);
	I2C_SCL_L(i2c);
	delay_us(i2c->delay);
}

static void i2c_sw_stop(i2c_sw_t *i2c)
{
	I2C_SDA_L(i2c);
	delay_us(i2c->delay);
	I2C_SCL_H(i2c);
	delay_us(i2c->delay);
	I2C_SDA_H(i2c);
	delay_us(i2c->delay);
}

static void i2c_sw_send_byte(i2c_sw_t *i2c, uint8_t byte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (byte & 0x80)
		{
			I2C_SDA_H(i2c);
		}
		else
		{
			I2C_SDA_L(i2c);
		}
		delay_us(i2c->delay);
		I2C_SCL_H(i2c);
		delay_us(i2c->delay);
		I2C_SCL_L(i2c);
		byte <<= 1;
	}
	I2C_SDA_H(i2c);
}

static uint8_t i2c_sw_recv_byte(i2c_sw_t *i2c)
{
	uint8_t i;
	uint8_t data = 0;

	I2C_SDA_H(i2c);
	for (i = 0; i < 8; i++)
	{
		data <<= 1;
		I2C_SCL_L(i2c);
		delay_us(i2c->delay);
		I2C_SCL_H(i2c);
		delay_us(i2c->delay);
		if (I2C_SDA_READ(i2c))
		{
			data++;
		}
	}
	return data;
}

static bool i2c_sw_wait_ack(i2c_sw_t *i2c)
{
	uint16_t retry = 0xFFFF;

	I2C_SDA_H(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_H(i2c);	
	delay_us(i2c->delay);
	do
	{
		if (--retry == 0)
		{
			return false;
		}
	} while (I2C_SDA_READ(i2c));
	I2C_SCL_L(i2c);
	delay_us(i2c->delay);
	return true;
}

static void i2c_sw_ack(i2c_sw_t *i2c)
{
	I2C_SCL_L(i2c);
	I2C_SDA_L(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_H(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_L(i2c);
	delay_us(i2c->delay);
	I2C_SDA_H(i2c);	
}

static void i2c_sw_nack(i2c_sw_t *i2c)
{
	I2C_SCL_L(i2c);
	I2C_SDA_H(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_H(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_L(i2c);
	delay_us(i2c->delay);
}

void i2c_sw_init(i2c_sw_t *i2c)
{
	gpio_pin_init(i2c->scl_pin, GPIO_OUT_OD, GPIO_AF_NONE);
	gpio_pin_init(i2c->sda_pin, GPIO_OUT_OD, GPIO_AF_NONE);

	i2c_sw_stop(i2c);
}

void i2c_sw_send(i2c_sw_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size)
{
	uint16_t i;

    i2c_sw_start(i2c);
    i2c_sw_send_byte(i2c, (slave_addr << 1) & 0xFE);
    if (!i2c_sw_wait_ack(i2c))
	{
		goto stop;
	}
	if (reg_width == I2C_REG_ADDR_16BIT)
	{
		i2c_sw_send_byte(i2c, (reg_addr >> 8) & 0xFF);
		if (!i2c_sw_wait_ack(i2c))
		{
			goto stop;
		}
	}
    i2c_sw_send_byte(i2c, reg_addr & 0xFF);
    if (!i2c_sw_wait_ack(i2c))
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        i2c_sw_send_byte(i2c, buf[i]);
        if (!i2c_sw_wait_ack(i2c))
		{
			goto stop;
		}
    }
stop:
    i2c_sw_stop(i2c);
}

void i2c_sw_recv(i2c_sw_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size)
{
	uint16_t i;

    i2c_sw_start(i2c);
    i2c_sw_send_byte(i2c, (slave_addr << 1) & 0xFE);
    if (!i2c_sw_wait_ack(i2c))
	{
		goto stop;
	}
	if (reg_width == I2C_REG_ADDR_16BIT)
	{
		i2c_sw_send_byte(i2c, (reg_addr >> 8) & 0xFF);
		if (!i2c_sw_wait_ack(i2c))
		{
			goto stop;
		}
	}
    i2c_sw_send_byte(i2c, reg_addr & 0xFF);
    if (!i2c_sw_wait_ack(i2c))
	{
		goto stop;
	}

    i2c_sw_start(i2c);
    i2c_sw_send_byte(i2c, (slave_addr << 1) | 0x01);
    if (!i2c_sw_wait_ack(i2c))
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        buf[i] = i2c_sw_recv_byte(i2c); 
        if(i == (size - 1))
        {
            i2c_sw_nack(i2c);
        }
        else
        {
            i2c_sw_ack(i2c);
        }
    }
stop:
	i2c_sw_stop(i2c);
}

bool i2c_sw_check(i2c_sw_t *i2c, uint8_t slave_addr)
{
	bool ack;

	i2c_sw_start(i2c);
    i2c_sw_send_byte(i2c, (slave_addr << 1) & 0xFE);
    ack =  i2c_sw_wait_ack(i2c);
	i2c_sw_stop(i2c);
	return ack;
}
#endif