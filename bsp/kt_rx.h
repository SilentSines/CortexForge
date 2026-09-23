#ifndef KT_RX_H
#define KT_RX_H

#define KT_NUM 2

#define KT_FREQ_MIN_HZ  640000  
#define KT_FREQ_MAX_HZ  690000    
#define KT_VOLUME_MIN   22   
#define KT_VOLUME_MAX   31     

void kt_rx_port_init(void);
void kt_rx_init(uint32_t *freq);
void kt_rx_cmd(uint8_t id, bool en);
void kt_rx_set_freq(uint8_t id, uint32_t freq);
uint32_t kt_rx_get_freq(uint8_t id);
void kt_rx_set_fast_freq(uint8_t id, uint32_t freq);
uint8_t kt_rx_check_pll(uint8_t id);
uint8_t kt_rx_check_snr(uint8_t id);
uint8_t kt_rx_sample_rssi(uint8_t id);
void kt_rx_set_volume(uint8_t id, uint8_t vol);
uint8_t kt_rx_get_volume(uint8_t id);
void kt_rx_hw_mute(uint8_t id, uint8_t en);
uint8_t kt_rx_get_rssi(uint8_t id, uint8_t chip_sel);
uint8_t kt_rx_get_snr(uint8_t id, uint8_t chip_sel);
uint8_t kt_rx_get_af(uint8_t id);
uint8_t kt_rx_get_diversity(uint8_t id);
bool kt_rx_get_pilot(uint8_t id, uint8_t chip_sel, uint16_t *buf);

#endif