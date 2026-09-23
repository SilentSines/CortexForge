#ifndef IR_H
#define IR_H

void ir_tx_init(void);
void ir_tx_start(uint8_t *buf, uint16_t size, uint8_t repeat, cb_t func);
void ir_tx_stop(void);

#endif