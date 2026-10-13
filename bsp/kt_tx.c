#include "bsp.h"

#include "KT_WirelessMicTxdrv.h"

#define KT_TX_NUM 1
#define KT_TX_I2S_ADDR 0x35

#define Delay_ms  delay_ms

static const i2c_sw_t i2c_table[KT_TX_NUM] = 
{
	{GPIO_PE14, GPIO_PE15, 2},
};

static i2c_sw_t *i2c_ptr;

static void KT_Bus_Write(uint8_t reg_addr, uint16_t data)
{
    uint8_t buf[2];

    buf[0] = (data >> 8) & 0xFF;
    buf[1] = data & 0xFF;
	i2c_sw_send(i2c_ptr, KT_TX_I2S_ADDR, reg_addr, I2C_REG_ADDR_8BIT, buf, 2);
}

static uint16_t KT_Bus_Read(uint8_t reg_addr)
{
    uint8_t buf[2];
 
	i2c_sw_recv(i2c_ptr, KT_TX_I2S_ADDR, reg_addr, I2C_REG_ADDR_8BIT, buf, 2);
	return (uint16_t)((buf[0] << 8) | buf[1]);
}

#include "KT_WirelessMicTxdrv.c"

static const gpio_pin_t kt_en_pin_table[KT_TX_NUM] = 
{
    GPIO_PE10,
};

static void kt_tx_select_port(uint8_t id)
{
    if (id >= KT_TX_NUM)
	{
		return;
	}

    i2c_ptr = (i2c_sw_t *)&i2c_table[id];	
}

void kt_tx_port_init(void)
{
    uint8_t i;
    
	for (i = 0; i < KT_TX_NUM; i++)
	{
        gpio_pin_init(kt_en_pin_table[i], GPIO_OUT_PP, GPIO_AF_NONE);
        GPIO_OUT_L(kt_en_pin_table[i]);
	}

    delay_ms(5);

    for (i = 0; i < KT_TX_NUM; i++)
	{
        GPIO_OUT_H(kt_en_pin_table[i]);
	}

#if 1
    delay_ms(70);
#endif
}

void kt_tx_init(uint32_t *freq)
{
    uint8_t i;
    for (i = 0; i < KT_TX_NUM; i++)
    {
        kt_tx_select_port(i);
        i2c_sw_init(i2c_ptr);

    #if WDT_EN
        wdt_feed();
    #endif

        while(!KT_WirelessMicTx_PreInit());
        while(!KT_WirelessMicTx_Init());

        KT_WirelessMicTx_Tune(freq[i]);
        KT_WirelessMicTx_EQGAIN(EQ_25H,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_40H,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_63H,EQ_GAIN_0dB);

        KT_WirelessMicTx_EQGAIN(EQ_100H,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_160H,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_250H,EQ_GAIN_0dB);

        KT_WirelessMicTx_EQGAIN(EQ_400H,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_630H,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_1KH,EQ_GAIN_0dB);

        KT_WirelessMicTx_EQGAIN(EQ_1K6,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_2K5,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_4KH,EQ_GAIN_0dB);

        KT_WirelessMicTx_EQGAIN(EQ_6K3,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_10K,EQ_GAIN_0dB);
        KT_WirelessMicTx_EQGAIN(EQ_16K,EQ_GAIN_0dB);

        KT_WirelessMicTx_EQSW(EQ_DISABLE);
        KT_WirelessMicTx_ECHO(ECHO_DISABLE,ECHO_RATIO_0,ECHO_DELAY_22ms);
        delay_ms(200);
            
        KT_WirelessMicTx_PAGain(63);
        KT_WirelessMicTx_PASW(PA_ON);

        KT_WirelessMicTx_Pilot_Fdev(PILOT_FDEV_5K);
        KT_WirelessMicTx_Pilot(PILOT_DISABLE);
    }
}

void kt_tx_set_freq(uint8_t id, uint32_t freq)
{
    if (id >= KT_TX_NUM)
    {
        return;
    }

    kt_tx_select_port(id);

    uint8_t pa;
 
    pa = KT_Bus_Read(0x11) & 0x3F;
    KT_WirelessMicTx_PAGain(0);
    KT_WirelessMicTx_PASW(PA_OFF);                        
    delay_ms(5);

    KT_WirelessMicTx_Tune(freq);
    KT_WirelessMicTx_PAGain(pa);
    KT_WirelessMicTx_PASW(PA_ON);
}