#include "bsp.h"

#define WM8978_I2C_ADDR    0x1A
#define VOLUME_MAX		63		
#define VOLUME_STEP		1		
#define GAIN_MAX		63		
#define GAIN_STEP		1	

#define WM8978_USE_I2C                  0
#define WM8978_USE_SOFT_I2C_SIGNAL      1
#define WM8978_USE_SOFT_I2C_MULTIPLE    0

#if WM8978_USE_I2C
    #define WM8978_I2C I2C2
    #define WM8978_I2C_INIT()               {}
    #define WM8978_I2C_SEND(reg, buf, size) i2c_send(WM8978_I2C, WM8978_I2C_ADDR, reg, I2C_ADDR_8BIT, buf, size)    
#elif WM8978_USE_SOFT_I2C_SIGNAL
    #define WM8978_I2C_INIT()               {}
    #define WM8978_I2C_SEND(reg, buf, size) soft_i2c_send(WM8978_I2C_ADDR, reg, I2C_ADDR_8BIT, buf, size)  
#elif WM8978_USE_SOFT_I2C_MULTIPLE
    #define WM8978_SCL_PORT     GPIOB
    #define WM8978_SCL_PIN      GPIO_Pin_6
    #define WM8978_SDA_PORT     GPIOB
    #define WM8978_SDA_PIN      GPIO_Pin_7
    #define WM8978_I2C_INIT()               soft_i2c_init(&i2c)
    #define WM8978_I2C_SEND(reg, buf, size) soft_i2c_send(&i2c, WM8978_I2C_ADDR, reg, I2C_ADDR_8BIT, buf, size)  
#endif

static const uint16_t reg_default[] = 
{
	0x000, 0x000, 0x000, 0x000, 0x050, 0x000, 0x140, 0x000,
	0x000, 0x000, 0x000, 0x0FF, 0x0FF, 0x000, 0x100, 0x0FF,
	0x0FF, 0x000, 0x12C, 0x02C, 0x02C, 0x02C, 0x02C, 0x000,
	0x032, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x038, 0x00B, 0x032, 0x000, 0x008, 0x00C, 0x093, 0x0E9,
	0x000, 0x000, 0x000, 0x000, 0x003, 0x010, 0x010, 0x100,
	0x100, 0x002, 0x001, 0x001, 0x039, 0x039, 0x039, 0x039,
	0x001, 0x001
};

#if WM8978_USE_SOFT_I2C_MULTIPLE
static soft_i2c_t i2c = {WM8978_SCL_PORT, WM8978_SCL_PIN, WM8978_SDA_PORT, WM8978_SDA_PIN, 5};
#endif

static uint16_t reg_val[] = 
{
	0x000, 0x000, 0x000, 0x000, 0x050, 0x000, 0x140, 0x000,
	0x000, 0x000, 0x000, 0x0FF, 0x0FF, 0x000, 0x100, 0x0FF,
	0x0FF, 0x000, 0x12C, 0x02C, 0x02C, 0x02C, 0x02C, 0x000,
	0x032, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x038, 0x00B, 0x032, 0x000, 0x008, 0x00C, 0x093, 0x0E9,
	0x000, 0x000, 0x000, 0x000, 0x003, 0x010, 0x010, 0x100,
	0x100, 0x002, 0x001, 0x001, 0x039, 0x039, 0x039, 0x039,
	0x001, 0x001
};

static void wm8978_write_reg(uint8_t reg_addr, uint16_t data)
{
	uint8_t reg, value;

	reg = ((reg_addr << 1) & 0xFE) | ((data >> 8) & 0x1);
	value = data & 0xFF;
	WM8978_I2C_SEND(reg, &value, 1);
	reg_val[reg_addr] = data;
}

static uint16_t wm8978_read_reg(uint8_t reg_addr)
{
	return reg_val[reg_addr];
}

static void wm8978_reset(void)
{
	uint8_t i;

	wm8978_write_reg(0x00, 0);

	for (i = 0; i < sizeof(reg_default) / 2; i++)
	{
		reg_val[i] = reg_default[i];
	}
}

void wm8978_init(void)
{
	WM8978_I2C_INIT();
	wm8978_reset();	
	wm8978_set_cfg(WM8978_FORMAT_PHILLIPS, WM8978_16BIT);
	wm8978_set_audio_path(WM8978_IN_DAC | WM8978_IN_LINE, WM8978_OUT_SPK | WM8978_OUT_HP_L | WM8978_OUT_HP_R);
	wm8978_set_ear_vol(55);
	wm8978_set_spk_vol(40);
}

void wm8978_set_cfg(audio_format_t format, audio_bits_t bits)
{
	uint16_t data = 0;

	data |= (format << 3);
	data |= (bits << 5);
	wm8978_write_reg(4, data);
	wm8978_write_reg(6, 0x000);
}

void wm8978_out_mute(bool en)
{
	uint16_t data;

	if (en) 
	{
		data = wm8978_read_reg(52); 
		data |= (1u << 6);
		wm8978_write_reg(52, data);
		data = wm8978_read_reg(53); 
		data |= (1u << 6);
		wm8978_write_reg(53, data);
		data = wm8978_read_reg(54); 
		data |= (1u << 6);
		wm8978_write_reg(54, data);
		data = wm8978_read_reg(55); 
		data |= (1u << 6);
		wm8978_write_reg(55, data);
	}
	else	
	{
		data = wm8978_read_reg(52);
		data &= ~(1u << 6);
		wm8978_write_reg(52, data);
		data = wm8978_read_reg(53); 
		data &= ~(1u << 6);
		wm8978_write_reg(53, data);
		data = wm8978_read_reg(54);
		data &= ~(1u << 6);
		wm8978_write_reg(54, data);
		data = wm8978_read_reg(55); 
		data &= ~(1u << 6);
		wm8978_write_reg(55, data);
	}
}

void wm8978_power_down(void)
{
	wm8978_reset();			
}

void wm8978_set_audio_path(audio_input_t in_path, audio_output_t out_path)
{
	uint16_t data;

	if ((in_path == WM8978_IN_NONE) && (out_path == WM8978_OUT_NONE))
	{
		wm8978_power_down();
		return;
	}

	data = (1 << 3) | (3 << 0);
	if (out_path & WM8978_OUT_AUX) 	
	{
		data |= ((1 << 7) | (1 << 6));
	}
	if ((in_path & WM8978_IN_MIC_L) || (in_path & WM8978_IN_MIC_R))
	{
		data |= (1 << 4);
	}
	wm8978_write_reg(1, data);

	data = 0;
	if (out_path & WM8978_OUT_HP_L)
	{
		data |= (1 << 7);
	}
	if (out_path & WM8978_OUT_HP_R)
	{
		data |= (1 << 8);
	}
	if (in_path & WM8978_IN_MIC_L)
	{
		data |= ((1 << 4) | (1 << 2));
	}
	if (in_path & WM8978_IN_MIC_R)
	{
		data |= ((1 << 5) | (1 << 3));
	}
	if (in_path & WM8978_IN_LINE)
	{
		data |= ((1 << 4) | (1 << 5));
	}
	if (in_path & WM8978_IN_ADC)
	{
		data |= ((1 << 1) | (1 << 0));
	}
	wm8978_write_reg(2, data);	

	data = 0;
	if (out_path & WM8978_OUT_AUX)
	{
		data |= ((1 << 8) | (1 << 7));
	}
	if (out_path & WM8978_OUT_SPK)
	{
		data |= ((1 << 6) | (1 << 5));
	}
	if (out_path != WM8978_OUT_NONE)
	{
		data |= ((1 << 3) | (1 << 2));
	}
	if (in_path & WM8978_IN_DAC)
	{
		data |= ((1 << 1) | (1 << 0));
	}
	wm8978_write_reg(3, data);	

	data = 0 << 8;
	if (in_path & WM8978_IN_LINE)
	{
		data |= ((1 << 6) | (1 << 2));
	}
	if (in_path & WM8978_IN_MIC_R)
	{
		data |= ((1 << 5) | (1 << 4));
	}
	if (in_path & WM8978_IN_MIC_L)
	{
		data |= ((1 << 1) | (1 << 0));
	}
	wm8978_write_reg(44, data);	

	if (in_path & WM8978_IN_ADC)
	{
		data = (1 << 3) | (0 << 8) | (4 << 0);		
	}
	else
	{
		data = 0;
	}
	wm8978_write_reg(14, data);	

	if (in_path & WM8978_IN_ADC)
	{
		data = (0 << 7);
		wm8978_write_reg(27, data);	
		data = 0;
		wm8978_write_reg(28, data);	
		wm8978_write_reg(29, data);	
		wm8978_write_reg(30, data);	
	}

	data = 0;		
	wm8978_write_reg(32, data);
	wm8978_write_reg(33, data);
	wm8978_write_reg(34, data);

	data = (3 << 1) | (7 << 0);		
	wm8978_write_reg(35, data);

	data = 0;
	if ((in_path & WM8978_IN_MIC_L) || (in_path & WM8978_IN_MIC_R))
	{
		data |= (1 << 8);
	}
	if (in_path & WM8978_IN_AUX)
	{
		data |= (3 << 0);	
	}
	if (in_path & WM8978_IN_LINE)
	{
		data |= (3 << 4);	
	}
	wm8978_write_reg(47, data);	
	wm8978_write_reg(48, data);	

	data = 0xFF;
	wm8978_write_reg(15, data);	
	data = 0x1FF;
	wm8978_write_reg(16, data);

	data = 0;
	if (out_path & WM8978_OUT_SPK)
	{
		data |= (1 << 4);	
	}
	if (in_path & WM8978_IN_AUX)
	{
		data |= ((7 << 1) | (1 << 0));
	}
	wm8978_write_reg(43, data);

	data = 0;
	if (in_path & WM8978_IN_DAC)
	{
		data |= ((1 << 6) | (1 << 5));
	}
	if (out_path & WM8978_OUT_SPK)
	{
		data |=  ((1 << 2) | (1 << 1));
	}
	if (out_path & WM8978_OUT_AUX)
	{
		data |=  ((1 << 4) | (1 << 3));
	}
	wm8978_write_reg(49, data);

	data = 0;
	if (in_path & WM8978_IN_AUX)
	{
		data |= ((7 << 6) | (1 << 5));
	}
	if ((in_path & WM8978_IN_LINE) || (in_path & WM8978_IN_MIC_L) || (in_path & WM8978_IN_MIC_R))
	{
		data |= ((7 << 2) | (1 << 1));
	}
	if (in_path & WM8978_IN_DAC)
	{
		data |= (1 << 0);
	}
	wm8978_write_reg(50, data);
	wm8978_write_reg(51, data);

	data = 0;
	if (out_path & WM8978_OUT_AUX)
	{
		data |= (1 << 3);
	}
	wm8978_write_reg(56, data);

	data = 0;
	if (out_path & WM8978_OUT_AUX)
	{
		data |= ((1 << 4) |  (1 << 1));
	}
	wm8978_write_reg(57, data);

	if (in_path & WM8978_IN_DAC)
	{
		wm8978_write_reg(11, 255);
		wm8978_write_reg(12, 255 | 0x100);
	}
	else
	{
		wm8978_write_reg(11, 0);
		wm8978_write_reg(12, 0 | 0x100);
	}

	if (in_path & WM8978_IN_DAC)
	{
		wm8978_write_reg(10, 0);
	}
}

void wm8978_set_mic_gain(uint8_t gain)
{
	if (gain > GAIN_MAX)
	{
		gain = GAIN_MAX;
	}
	wm8978_write_reg(45, gain);
	wm8978_write_reg(46, gain | (1 << 8));
}

void wm8978_set_line_gain(uint8_t gain)
{
	uint16_t data;

	if (gain > 7)
	{
		gain = 7;
	}
	data = wm8978_read_reg(47);
	data &= 0x8F;
	data |= (gain << 4);
	wm8978_write_reg(47, data);	
	data = wm8978_read_reg(48);
	data &= 0x8F;
	data |= (gain << 4);
	wm8978_write_reg(48, data);
}

void wm8978_set_spk_vol(uint8_t volume)
{
	if (volume > 0x3F)
	{
		volume = 0x3F;
	}
	wm8978_write_reg(54, volume | 0x00); 
	wm8978_write_reg(55, volume | 0x100); 
}

void wm8978_set_ear_vol(uint8_t volume)
{
	if (volume > 0x3F)
	{
		volume = 0x3F;
	}
	wm8978_write_reg(52, volume | 0x00); 
	wm8978_write_reg(53, volume | 0x100);
}

uint8_t wm8978_get_ear_vol(void)
{
	return (uint8_t)(wm8978_read_reg(52) & 0x3F);
}

uint8_t wm8978_get_spk_vol(void)
{
	return (uint8_t)(wm8978_read_reg(54) & 0x3F);
}

void wm8978_notch_filter(uint16_t nfa0, uint16_t nfa1)
{
	uint16_t data;

	data = (1 << 7) | (nfa0 & 0x3F);
	wm8978_write_reg(27, data);	

	data = ((nfa0 >> 7) & 0x3F);
	wm8978_write_reg(28, data);	

	data = (nfa1 & 0x3F);
	wm8978_write_reg(29, data);	

	data = (1 << 8) | ((nfa1 >> 7) & 0x3F);
	wm8978_write_reg(30, data);	
}
