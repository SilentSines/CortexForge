#include "bsp.h"

#include "KT_WirelessMicRxdrv.h"

#define KT_MUTE_LEVEL 0
#define KT_HW_MUTE_EN 0
#define KT_I2S_EN 1
#define kt_delay_ms  delay_ms

#define KT_RX_MASTER_I2S_ADDR 0x31
#define KT_RX_MASTER_I2S_ADDR_PIN_BITS 3
#define KT_RX_SLAVE_I2S_ADDR 0x35
#define KT_RX_SLAVE_I2S_ADDR_PIN_BITS 3

#define KT_RX_USE_I2C_HW            0
#define KT_RX_USE_I2C_SW_FIXED      0
#define KT_RX_USE_I2C_SW_INSTANCE   1

typedef struct
{
#if KT_RX_USE_I2C_SW_INSTANCE
    i2c_sw_t i2c;      
#endif   
    gpio_pin_t en_pin;      
    uint8_t addr;       
    uint8_t has_slave;   
    uint8_t slave_addr;
} kt_dev_t;

#if KT_RX_USE_I2C_SW_INSTANCE
static const kt_dev_t kt_dev_table[KT_RX_NUM] = 
{
    {{GPIO_PB13, GPIO_PB12, 2}, GPIO_PC1, KT_RX_MASTER_I2S_ADDR | (1 << KT_RX_MASTER_I2S_ADDR_PIN_BITS), 1, KT_RX_SLAVE_I2S_ADDR | (1 << KT_RX_SLAVE_I2S_ADDR_PIN_BITS)},
    {{GPIO_PB15, GPIO_PB14, 2}, GPIO_PC1, KT_RX_MASTER_I2S_ADDR | (1 << KT_RX_MASTER_I2S_ADDR_PIN_BITS), 1, KT_RX_SLAVE_I2S_ADDR | (1 << KT_RX_SLAVE_I2S_ADDR_PIN_BITS)},
    {{GPIO_PC5,  GPIO_PC4,  2}, GPIO_PC1, KT_RX_MASTER_I2S_ADDR | (1 << KT_RX_MASTER_I2S_ADDR_PIN_BITS), 1, KT_RX_SLAVE_I2S_ADDR | (1 << KT_RX_SLAVE_I2S_ADDR_PIN_BITS)},
    {{GPIO_PC3,  GPIO_PC2,  2}, GPIO_PC1, KT_RX_MASTER_I2S_ADDR | (1 << KT_RX_MASTER_I2S_ADDR_PIN_BITS), 1, KT_RX_SLAVE_I2S_ADDR | (1 << KT_RX_SLAVE_I2S_ADDR_PIN_BITS)},
};

static i2c_sw_t *i2c_ptr;
static uint8_t kt_rx_id; 
#endif

#if KT_I2S_EN
const strI2s i2s_cfg[KT_RX_NUM] =
{
    {KT_I2S_LRCLK48K, KT_I2S_MCLK12P288M ,KT_I2S_SLAVE, KT_I2S_MONO, KT_I2S_RIGHT, KT_I2S_I2SMODE, KT_I2S_LENGHT24BIT},
    {KT_I2S_LRCLK48K, KT_I2S_MCLK12P288M, KT_I2S_SLAVE, KT_I2S_MONO, KT_I2S_RIGHT, KT_I2S_I2SMODE, KT_I2S_LENGHT24BIT},
    {KT_I2S_LRCLK48K, KT_I2S_MCLK12P288M ,KT_I2S_SLAVE, KT_I2S_MONO, KT_I2S_RIGHT, KT_I2S_I2SMODE, KT_I2S_LENGHT24BIT},
    {KT_I2S_LRCLK48K, KT_I2S_MCLK12P288M, KT_I2S_SLAVE, KT_I2S_MONO, KT_I2S_RIGHT, KT_I2S_I2SMODE, KT_I2S_LENGHT24BIT},
};
#endif

static void KT_Bus_Write(uint16_t reg_addr, uint8_t data, uint8_t chip)
{
    uint8_t slave_addr = (chip == 0) ? kt_dev_table[kt_rx_id].addr : kt_dev_table[kt_rx_id].slave_addr;

#if KT_RX_USE_I2C_SW_INSTANCE
    i2c_sw_send(i2c_ptr, slave_addr, reg_addr, I2C_REG_ADDR_16BIT, &data, 1);
#endif
}

static uint8_t KT_Bus_Read(uint16_t reg_addr, uint8_t chip)
{
    uint8_t data;
    uint8_t slave_addr = (chip == 0) ? kt_dev_table[kt_rx_id].addr : kt_dev_table[kt_rx_id].slave_addr;

#if KT_RX_USE_I2C_SW_INSTANCE
    i2c_sw_recv(i2c_ptr, slave_addr, reg_addr, I2C_REG_ADDR_16BIT, &data, 1);
#endif
    return data;
}

#include "KT_WirelessMicRxdrv.c"

#if KT_HW_MUTE_EN
const gpio_pin_t kt_mute_pin_table[KT_RX_NUM] = 
{
    GPIO_PC14,
    GPIO_PC15,
};   
#endif

static void kt_rx_sel_xtal(void)
{
    uint8_t data, state;

#ifdef XTAL_DUAL
    return;
#endif

    data = KT_Bus_Read(0x0012, chipSel);
#ifdef XTAL_24M_ONLY
#if XTAL_24M_FREQ
    KT_Bus_Write(0x0012, data | 0x80, chipSel);
#else
    return;
#endif
#endif

#ifdef XTAL_24P576M_ONLY
#if XTAL_24P576M_FREQ
    KT_Bus_Write(0x0012, data | 0x80, chipSel);
#else
    return;
#endif
#endif

    do
    {
        data = KT_Bus_Read(0x00a0, chipSel);
    } while (!(data & 0x02));
    data = KT_Bus_Read(0x00a0, chipSel);
    KT_Bus_Write(0x00a0, data | 0x02, chipSel);
    data = KT_Bus_Read(0x0015, chipSel); // rst dll
    KT_Bus_Write(0x0015, data | 0x40, chipSel);
    data = KT_Bus_Read(0x002a, chipSel); // ifadc rst
    KT_Bus_Write(0x002a, data | 0x80, chipSel);
    kt_delay_ms(10);
    data = KT_Bus_Read(0x0015, chipSel);
    KT_Bus_Write(0x0015, data & ~0x40, chipSel);
    do
    {
        data = KT_Bus_Read(0x0180, chipSel);
        state = (data & 0x08) >> 3;
    } while (!state);
    data = KT_Bus_Read(0x002a, chipSel); // ifadc rst
    KT_Bus_Write(0x002a, data & ~0x80, chipSel);
}

static void kt_rx_select_port(uint8_t id)
{
    if (id >= KT_RX_NUM)
	{
		return;
	}

#if KT_RX_USE_I2C_SW_INSTANCE
    i2c_ptr = (i2c_sw_t *)&kt_dev_table[id].i2c;
    kt_rx_id = id;
#endif		
}

static void kt_rx_fast_rssi(uint32_t freq)
{
    uint8_t freq_h, freq_m, freq_l, data;

    data = KT_Bus_Read(0x0108, chipSel);
    KT_Bus_Write(0x0108, data | BIT3, chipSel); // SCAN_MODE=1;

    freq = freq & 0x000FFFFF;

    freq_h = (freq >> 12);
    freq_m = ((freq & 0x00000FFF) >> 4);
    freq_l = ((freq & 0x0000000F) << 4);

    KT_Bus_Write(0x0045, freq_h, chipSel);
    KT_Bus_Write(0x0046, freq_m, chipSel);

    data = KT_Bus_Read(0x0047, chipSel);
    KT_Bus_Write(0x0047, (data & 0x0F) | freq_l, chipSel);

    if (KT_Bus_Read(0x010f, chipSel) == 0x10) // a
    {
        data = KT_Bus_Read(0x0053, chipSel);
        KT_Bus_Write(0x0053, data & ~0x40, chipSel); // rfamp_int_en=0
    }

    data = KT_Bus_Read(0x0047, chipSel);
    KT_Bus_Write(0x0047, data | BIT0, chipSel); // chan_valid=1;
}

static uint8_t kt_rx_get_pll(void)
{
    uint8_t data = 0x00;

    data = KT_Bus_Read(0x0061, chipSel) & 0x01;
    return data;
}

static void kt_rx_snr_mute(void)
{
    uint8_t data = 0;

    if (KT_WirelessMicRx_GetSNR() > AUTOMUTE_SNR_LOWTH)
    {
        data = KT_Bus_Read(0x0087, chipSel); // SOFT_SNR_MUTE写为0
        KT_Bus_Write(0x0087, data & ~0x02, chipSel);
    }
}

#if KT_I2S_EN
static void kt_i2s_init(const strI2s *cfg)
{
    uint8_t data;

    KT_Bus_Write(0x0052, 0x04, chipSel);
    if (cfg->masterOrSlave == 1) // mater
    {
        data = KT_Bus_Read(0x0050, chipSel);
        KT_Bus_Write(0x0050, (data & 0xc0) | ((cfg->stereoOrMono) << 5) | ((cfg->leftOrRight) << 4) | ((cfg->mode) << 2) | (cfg->dataLength), chipSel);
        data = KT_Bus_Read(0x0051, chipSel);
        if ((cfg->LRCLK) == KT_I2S_LRCLK48K)
        {
            KT_Bus_Write(0x0051, (data & 0xf0) | (cfg->mode) | 0x18, chipSel);
        }
        else if ((cfg->LRCLK) == KT_I2S_LRCLK192K)
        {
            KT_Bus_Write(0x0051, (data & 0xf0) | (cfg->mode) | 0x14, chipSel);
        }
        else // 96K
        {
            if ((cfg->MCLK) == KT_I2S_MCLK24P576M)
            {
                KT_Bus_Write(0x0051, (data & 0xf0) | (cfg->mode) | 0x1c, chipSel);
            }
            else
            {
                KT_Bus_Write(0x0051, (data & 0xf0) | (cfg->mode) | 0x10, chipSel);
            }
        }
        KT_Bus_Write(0x0052, 0x02, chipSel); // i2s_master_en= 1
    }
    else
    {
        KT_Bus_Write(0x0052, 0x01, chipSel); 
        KT_Bus_Write(0x004d, 0x01, chipSel); 
        data = KT_Bus_Read(0x004d, chipSel);
        KT_Bus_Write(0x004d, data | 0x80, chipSel); 
        data = KT_Bus_Read(0x0050, chipSel);
        KT_Bus_Write(0x0050, (data & 0xc0) | ((cfg->stereoOrMono) << 5) | ((cfg->leftOrRight) << 4) | ((cfg->mode) << 2) | (cfg->dataLength), chipSel);
        data = KT_Bus_Read(0x0050, chipSel);
        KT_Bus_Write(0x0050, data | 0x40, chipSel); 
        data = KT_Bus_Read(0x0052, chipSel);
        KT_Bus_Write(0x0052, data | 0x08, chipSel); 
    }
}
#endif

void kt_rx_port_init(void)
{
    uint8_t i;
    
	for (i = 0; i < KT_RX_NUM; i++)
	{
        gpio_pin_init(kt_dev_table[i].en_pin, GPIO_OUT_PP, GPIO_AF_NONE);
        GPIO_OUT_L(kt_dev_table[i].en_pin);

    #if KT_HW_MUTE_EN
    #if KT_MUTE_LEVEL
        gpio_pin_init(kt_mute_pin_table[i], GPIO_OUT_PP_HIGH, GPIO_AF_NONE);
    #else
        gpio_pin_init(kt_mute_pin_table[i], GPIO_OUT_PP_HIGH, GPIO_AF_NONE);
    #endif
    #endif
	}

    delay_ms(5);

    for (i = 0; i < KT_RX_NUM; i++)
	{
        GPIO_OUT_H(kt_dev_table[i].en_pin);
	}

#if 1
    delay_ms(70);
#endif
}

void kt_rx_init(uint32_t *freq)
{
    uint8_t i;
    uint8_t data;

    for (i = 0; i < KT_RX_NUM; i++)
    {
    #if KT_RX_USE_I2C_SW_INSTANCE
        kt_rx_select_port(i);
        i2c_sw_init(i2c_ptr);
    #endif

    #if WDT_EN
        wdt_feed();
    #endif

        chipSel = 0;		
		while(!KT_WirelessMicRx_PreInit()){};
        while(!KT_WirelessMicRx_Init()){};
        kt_rx_sel_xtal();
        KT_WirelessMicRx_Tune(freq[i]);
    #if KT_I2S_EN
        kt_i2s_init(&i2s_cfg[i]);    
    #endif
        KT_Bus_Write(INT_EN, 0x00, chipSel);
        delay_ms(10);
        KT_Bus_Write(INT_FLAG, 0x00, chipSel);
        // 关闭所有中断，并清除中断标志 条件1
        KT_Bus_Write(INT_EN, 0x00, chipSel);
        delay_ms(10);
        KT_Bus_Write(INT_FLAG, 0x00, chipSel);
        // 关闭自动调节AFC(自动频率控制)使能 条件2 AFC 通常指的是 自动频率控制（Automatic Frequency Control）
        data = KT_Bus_Read(SW_CFG14, chipSel);
        // 开
        KT_Bus_Write(SW_CFG14, (data & 0xFE) | 0x01, chipSel);
        // 关
        KT_Bus_Write(SW_CFG14, (data & 0xFE), chipSel);
        // 0:20 1:40 2:60 3:90 (kHz)
        data = KT_Bus_Read(AFC_CFG0, chipSel);
        KT_Bus_Write(AFC_CFG0, (data & 0x3f) | (0 << 6), chipSel);
        // AFC 手动使能
        data = KT_Bus_Read(AFC_CFG1, chipSel);
        KT_Bus_Write(AFC_CFG1, (data & 0xfd) | (1 << 1), chipSel);
        // 不使能 导频(pilot)和 使能信噪比(SNR)的自动静音功能 条件3
        data = KT_Bus_Read(SW_CFG0, chipSel);
        KT_Bus_Write(SW_CFG0, (data & 0x5f) | (1 << 5) | (0 << 7), chipSel);
        delay_ms(10);
        // 设置音量 条件5
        KT_WirelessMicRx_Volume(0);
        kt_rx_hw_mute(i, true);
        data = KT_Bus_Read(DE_EXP_CFG, chipSel);
        // 24ms扩展时间常数
        // 关闭去加重
        // 开启扩展
        // 先去加重，再扩展
        KT_Bus_Write(DE_EXP_CFG, (data & 0x82) | (0x01 << 4) | (0x01 << 3) | (0x01 << 2) | (0x01), chipSel);
        //			chipSel = 1;
        // 指示分集是主或者从，可读可写
        //		KT_Bus_Write(SW_CFG6,0x00,chipSel);
        // 是否使能分集(先读取 再设置，否则寄存器有一定的影响)
        KT_WirelessMicRx_SW_Diversity(1);

    #if WDT_EN
        wdt_feed();
    #endif

        if (!kt_dev_table[i].has_slave)
        {
            continue;
        }

        // 开始配置从机芯片
        chipSel = 1;
        // 读取芯片ID判断通讯是否正常
        while (!KT_WirelessMicRx_PreInit()){};
        // 初始化芯片
        while (!KT_WirelessMicRx_Init()){};
        // 防止内部片选发生改变
        chipSel = 1;
        kt_rx_sel_xtal();
        // 设置接收频率
        KT_WirelessMicRx_Tune(freq[i]);
        // 数字信道使能和新旧模式选择
        //  data = KT_Bus_Read(0x0241,chipSel);
        //  KT_Bus_Write(0x0241,data&0x7F,chipSel);
        // 关闭所有中断，并清除中断标志 1
        KT_Bus_Write(0x0055, 0x00, chipSel);
        KT_Bus_Write(0x0059, 0x00, chipSel);
        // 关闭AFC使能 2
        data = KT_Bus_Read(0x0218, chipSel);
        KT_Bus_Write(0x0218, data & 0x01, chipSel);
        // 不使能 导频(pilot)和信噪比(SNR)的自动静音功能 3
        //  KT_Bus_Write(0x0100,0x42,chipSel);
        // 使能 导频(pilot)和信噪比(SNR)的自动静音功能 3
        data = KT_Bus_Read(0x0100, chipSel);
        KT_Bus_Write(0x0100, (data & 0x5f) | (1 << 5) | (0 << 7), chipSel);
        delay_ms(10);
        KT_WirelessMicRx_SW_Diversity(1);
    }
	delay_ms(500);
#if WDT_EN
    wdt_feed();
#endif
	chipSel = 0;
}

void kt_rx_cmd(uint8_t id, bool en)
{
    if (id >= KT_RX_NUM)
	{
		return;
	}
    if (en)
    {
        GPIO_OUT_H(kt_dev_table[id].en_pin);
    }
    else
    {
        GPIO_OUT_L(kt_dev_table[id].en_pin);
    }
}

void kt_rx_set_freq(uint8_t id, uint32_t freq)
{
    if (id >= KT_RX_NUM)
    {
        return;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
	KT_WirelessMicRx_Tune(freq);

#if KT_I2S_EN
    kt_i2s_init(&i2s_cfg[id]);
#endif

    if (!kt_dev_table[id].has_slave)
    {
        return;
    }

	chipSel = 1;
	KT_WirelessMicRx_Tune(freq);	
}

uint32_t kt_rx_get_freq(uint8_t id)
{
    uint32_t freq;
    uint8_t temp[3];

    if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
    temp[0] = KT_Bus_Read(0x0047, chipSel) & 0xf0;
    temp[0] >>= 4;
    temp[1] = KT_Bus_Read(0x0046, chipSel);
    temp[2] = KT_Bus_Read(0x0045, chipSel);
    freq = (temp[2] << 12) | (temp[1] << 4) | temp[0];
    return freq;
}

void kt_rx_set_fast_freq(uint8_t id, uint32_t freq)
{
    if (id >= KT_RX_NUM)
    {
        return;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
    kt_rx_fast_rssi(freq);

#if KT_I2S_EN
    kt_i2s_init(&i2s_cfg[id]);
#endif

    if (!kt_dev_table[id].has_slave)
    {
        return;
    }

    chipSel = 1;
    kt_rx_fast_rssi(freq);
}

uint8_t kt_rx_check_pll(uint8_t id)
{
    uint8_t pll[2];

    if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
    pll[0] = kt_rx_get_pll();
    kt_rx_snr_mute();

    if (!kt_dev_table[id].has_slave)
    {
        return pll[0];
    }

    chipSel = 1;
    pll[1] = kt_rx_get_pll();
    kt_rx_snr_mute();
    return (pll[0] && pll[1]);
}

uint8_t kt_rx_check_snr(uint8_t id)
{
    uint8_t snr[2];

    if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
    snr[0] = KT_WirelessMicRx_GetSNR();

    if (!kt_dev_table[id].has_slave)
    {
        return snr[0];
    }

    chipSel = 1;
    snr[1] = KT_WirelessMicRx_GetSNR();
    return MAX(snr[0], snr[1]);
}

uint8_t kt_rx_sample_rssi(uint8_t id)
{
    uint8_t rssi[2];

    if (id >= KT_RX_NUM)
    {
        return 0xFF;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
    rssi[0] = KT_WirelessMicRx_GetRSSI();

    if (!kt_dev_table[id].has_slave)
    {
        return rssi[0];
    }

    chipSel = 1;
    rssi[1] = KT_WirelessMicRx_GetRSSI();
    return MAX(rssi[0], rssi[1]);
}

void kt_rx_set_volume(uint8_t id, uint8_t vol)
{
    if (id >= KT_RX_NUM)
    {
        return;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif   

    chipSel = 0;
	KT_WirelessMicRx_Volume(vol);	
}

uint8_t kt_rx_get_volume(uint8_t id)
{
    uint8_t data;

if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif   

    chipSel = 0;
    data = KT_Bus_Read(0x0201, chipSel);
    return (data & 0x1F);
}

void kt_rx_hw_mute(uint8_t id, uint8_t en)
{
    if (id >= KT_RX_NUM)
    {
        return;
    }

#if KT_HW_MUTE_EN
    if (en)
    {
    #if KT_MUTE_LEVEL
        GPIO_OUT_H(kt_mute_pin_table[id]);
    #else
        GPIO_OUT_L(kt_mute_pin_table[id]);
    #endif
    }
    else
    {
    #if KT_MUTE_LEVEL
        GPIO_OUT_L(kt_mute_pin_table[id]);
    #else
        GPIO_OUT_H(kt_mute_pin_table[id]);
    #endif
    }
#else
    UNUSED(en);
#endif
}

uint8_t kt_rx_get_rssi(uint8_t id, uint8_t chip_sel)
{
if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif     

    chipSel = chip_sel;
    return KT_WirelessMicRx_GetRSSI();
}

uint8_t kt_rx_get_snr(uint8_t id, uint8_t chip_sel)
{
if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif     

    chipSel = chip_sel;
    return KT_WirelessMicRx_GetSNR();
}

uint8_t kt_rx_get_af(uint8_t id)
{
    if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif   

    chipSel = 0;
    return KT_WirelessMicRx_GetAF();
}

uint8_t kt_rx_get_diversity(uint8_t id)
{
    uint8_t data;

    if (id >= KT_RX_NUM)
    {
        return 0;
    }

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif

    chipSel = 0;
    data = KT_Bus_Read(0x0106, 0) & 0x01;
	return data;
}

bool kt_rx_get_pilot(uint8_t id, uint8_t chip_sel, uint16_t *buf)
{
    uint8_t i, data;
    uint16_t addr_aux_data = AUX_DATA_START_ADDR;

    if (id >= KT_RX_NUM)
    {
        return false;
    }   

#if KT_RX_USE_I2C_SW_INSTANCE
    kt_rx_select_port(id);
#endif    

    chipSel = chip_sel;
    data = KT_Bus_Read(0x0209, chipSel) & 0xC0;
    if (data != PILOT_SIGNAL_VALID)
    {
        return false;
    }
    for (i = 0; i < MAX_AUX_DATA_COUNT; i++)
    {
        data = KT_Bus_Read(addr_aux_data, chipSel);
        buf[i] = ((uint16_t)data << 8) | KT_Bus_Read(addr_aux_data + 1, chipSel);
        addr_aux_data += 2;
    }
    return true;
}