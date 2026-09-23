#ifndef I2S_H
#define I2S_H

typedef enum
{
	I2S1, 
	I2S2, 
	I2S3, 
	I2S4,
} i2s_port_t;

typedef enum 
{
    I2S_MASTER_FULL_DUPLEX,   
    I2S_MASTER_HALF_TX,        
    I2S_MASTER_HALF_RX,       
    I2S_SLAVE_FULL_DUPLEX,
    I2S_SLAVE_HALF_TX,    
    I2S_SLAVE_HALF_RX
} 
i2s_mode_t;

void i2s_bus_init(i2s_port_t i2s, i2s_mode_t mode, bool mck_en);
void i2s_set_cfg(i2s_port_t i2s, i2s_audio_sampling_freq_type samplerate, i2s_data_channel_format_type format);
void i2s_set_tx_dma(i2s_port_t i2s, uint16_t *buf, uint32_t len, bool dbuf_en, cb_arg_t func);
void i2s_set_rx_dma(i2s_port_t i2s, uint16_t *buf, uint32_t len, bool dbuf_en, cb_arg_t func);
void i2s_tx_start(i2s_port_t i2s);
void i2s_tx_stop(i2s_port_t i2s);
void i2s_rx_start(i2s_port_t i2s);
void i2s_rx_stop(i2s_port_t i2s);

#endif
