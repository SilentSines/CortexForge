#include "hal.h"
#include "i2c_cfg.h"

#if SOFT_I2C_SIGNAL == 1
static void soft_i2c_start(void)
{
	I2C_SDA_H();
	I2C_SCL_H();
	delay_us(SOFT_I2C_DELAY);
	I2C_SDA_L();
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_L();
	delay_us(SOFT_I2C_DELAY);
}

static void soft_i2c_stop(void)
{
	I2C_SDA_L();
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_H();
	delay_us(SOFT_I2C_DELAY);
	I2C_SDA_H();
	delay_us(SOFT_I2C_DELAY);
}

static void soft_i2c_send_byte(uint8_t byte)
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

static uint8_t soft_i2c_recv_byte(void)
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

static bool soft_i2c_wait_ack(void)
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

static void soft_i2c_ack(void)
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

static void soft_i2c_nack(void)
{
	I2C_SCL_L();
	I2C_SDA_H();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_H();	
	delay_us(SOFT_I2C_DELAY);
	I2C_SCL_L();
	delay_us(SOFT_I2C_DELAY);
}

void soft_i2c_init(void)
{
	gpio_pin_init(SOFT_I2C_SCL_PIN, GPIO_OUT_OD, GPIO_DRIVE_STRENGTH_MODERATE);
	gpio_pin_init(SOFT_I2C_SDA_PIN, GPIO_OUT_OD, GPIO_DRIVE_STRENGTH_MODERATE);
    
    soft_i2c_stop();
}

void soft_i2c_send(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size)
{
    uint16_t i;

    soft_i2c_start();
    soft_i2c_send_byte((slave_addr << 1) & 0xFE);
    if (!soft_i2c_wait_ack())
	{
		goto stop;
	}
	if (reg_width == I2C_REG_ADDR_16BIT)
	{
		soft_i2c_send_byte((reg_addr >> 8) & 0xFF);
		if (!soft_i2c_wait_ack())
		{
			goto stop;
		}
	}
    soft_i2c_send_byte(reg_addr & 0xFF);
    if (!soft_i2c_wait_ack())
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        soft_i2c_send_byte(buf[i]);
        if (!soft_i2c_wait_ack())
		{
			goto stop;
		}
    }
stop:
    soft_i2c_stop();
}

void soft_i2c_recv(uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size)
{
    uint16_t i;

    soft_i2c_start();
    soft_i2c_send_byte((slave_addr << 1) & 0xFE);
    if (!soft_i2c_wait_ack())
	{
		goto stop;
	}
    if (reg_width == I2C_REG_ADDR_16BIT)
	{
		soft_i2c_send_byte((reg_addr >> 8) & 0xFF);
		if (!soft_i2c_wait_ack())
		{
			goto stop;
		}
	}
    soft_i2c_send_byte(reg_addr & 0xFF);
    if (!soft_i2c_wait_ack())
	{
		goto stop;
	}

    soft_i2c_start();
    soft_i2c_send_byte((slave_addr << 1) | 0x01);
    if (!soft_i2c_wait_ack())
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        buf[i] = soft_i2c_recv_byte(); 
        if(i == (size - 1))
        {
            soft_i2c_nack();
        }
        else
        {
            soft_i2c_ack();
        }
    }
stop:
    soft_i2c_stop();
}

bool soft_i2c_check(uint8_t slave_addr)
{
	bool ack;

	soft_i2c_start();
    soft_i2c_send_byte((slave_addr << 1) & 0xFE);
    ack =  soft_i2c_wait_ack();
	soft_i2c_stop();
	return ack;
}
#elif SOFT_I2C_MULTIPLE == 1
static void soft_i2c_start(soft_i2c_t *i2c)
{
	I2C_SDA_H(i2c);
	I2C_SCL_H(i2c);
	delay_us(i2c->delay);
	I2C_SDA_L(i2c);
	delay_us(i2c->delay);
	I2C_SCL_L(i2c);
	delay_us(i2c->delay);
}

static void soft_i2c_stop(soft_i2c_t *i2c)
{
	I2C_SDA_L(i2c);
	delay_us(i2c->delay);
	I2C_SCL_H(i2c);
	delay_us(i2c->delay);
	I2C_SDA_H(i2c);
	delay_us(i2c->delay);
}

static void soft_i2c_send_byte(soft_i2c_t *i2c, uint8_t byte)
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

static uint8_t soft_i2c_recv_byte(soft_i2c_t *i2c)
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

static bool soft_i2c_wait_ack(soft_i2c_t *i2c)
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

static void soft_i2c_ack(soft_i2c_t *i2c)
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

static void soft_i2c_nack(soft_i2c_t *i2c)
{
	I2C_SCL_L(i2c);
	I2C_SDA_H(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_H(i2c);	
	delay_us(i2c->delay);
	I2C_SCL_L(i2c);
	delay_us(i2c->delay);
}

void soft_i2c_init(soft_i2c_t *i2c)
{
	gpio_pin_init(i2c->scl_pin, GPIO_OUT_OD, GPIO_DRIVE_STRENGTH_MODERATE);
	gpio_pin_init(i2c->sda_pin, GPIO_OUT_OD, GPIO_DRIVE_STRENGTH_MODERATE);

	soft_i2c_stop(i2c);
}

void soft_i2c_send(soft_i2c_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, const uint8_t *buf, uint16_t size)
{
	uint16_t i;

    soft_i2c_start(i2c);
    soft_i2c_send_byte(i2c, (slave_addr << 1) & 0xFE);
    if (!soft_i2c_wait_ack(i2c))
	{
		goto stop;
	}
	if (reg_width == I2C_REG_ADDR_16BIT)
	{
		soft_i2c_send_byte(i2c, (reg_addr >> 8) & 0xFF);
		if (!soft_i2c_wait_ack(i2c))
		{
			goto stop;
		}
	}
    soft_i2c_send_byte(i2c, reg_addr & 0xFF);
    if (!soft_i2c_wait_ack(i2c))
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        soft_i2c_send_byte(i2c, buf[i]);
        if (!soft_i2c_wait_ack(i2c))
		{
			goto stop;
		}
    }
stop:
    soft_i2c_stop(i2c);
}

void soft_i2c_recv(soft_i2c_t *i2c, uint8_t slave_addr, uint16_t reg_addr, i2c_reg_addr_width_t reg_width, uint8_t *buf, uint16_t size)
{
	uint16_t i;

    soft_i2c_start(i2c);
    soft_i2c_send_byte(i2c, (slave_addr << 1) & 0xFE);
    if (!soft_i2c_wait_ack(i2c))
	{
		goto stop;
	}
	if (reg_width == I2C_REG_ADDR_16BIT)
	{
		soft_i2c_send_byte(i2c, (reg_addr >> 8) & 0xFF);
		if (!soft_i2c_wait_ack(i2c))
		{
			goto stop;
		}
	}
    soft_i2c_send_byte(i2c, reg_addr & 0xFF);
    if (!soft_i2c_wait_ack(i2c))
	{
		goto stop;
	}

    soft_i2c_start(i2c);
    soft_i2c_send_byte(i2c, (slave_addr << 1) | 0x01);
    if (!soft_i2c_wait_ack(i2c))
	{
		goto stop;
	}
    for (i = 0; i < size; i++)
    {
        buf[i] = soft_i2c_recv_byte(i2c); 
        if(i == (size - 1))
        {
            soft_i2c_nack(i2c);
        }
        else
        {
            soft_i2c_ack(i2c);
        }
    }
stop:
	soft_i2c_stop(i2c);
}

bool soft_i2c_check(soft_i2c_t *i2c, uint8_t slave_addr)
{
	bool ack;

	soft_i2c_start(i2c);
    soft_i2c_send_byte(i2c, (slave_addr << 1) & 0xFE);
    ack =  soft_i2c_wait_ack(i2c);
	soft_i2c_stop(i2c);
	return ack;
}
#endif