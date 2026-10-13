#ifndef KT_TX_H
#define KT_TX_H

void kt_tx_port_init(void);
void kt_tx_init(uint32_t *freq);
void kt_tx_set_freq(uint8_t id, uint32_t freq);

#endif