#ifndef USBD_CDC_H
#define USBD_CDC_H

void usbd_cdc_monitor(usbd_core_type *udev);
void usbd_cdc_send(const uint8_t *buf, uint32_t size);
void usbd_cdc_send_byte(uint8_t data);
uint32_t usbd_cdc_recv(uint8_t *buf, uint32_t size);
bool usbd_cdc_recv_byte(uint8_t *data);
bool usbd_cdc_peek_rx(uint8_t *buf, uint32_t size);
void usbd_cdc_discard_rx(uint32_t size);
void usbd_cdc_clr_tx(void);
void usbd_cdc_clr_rx(void);

#endif