#ifndef USBD_CUSTOM_HID_H
#define USBD_CUSTOM_HID_H

void usbd_custom_hid_monitor(usbd_core_type *udev);
void usbd_custom_hid_send(const uint8_t *buf, uint32_t size);
void usbd_custom_hid_send_byte(uint8_t data);
uint32_t usbd_custom_hid_recv(uint8_t *buf, uint32_t size);
bool usbd_custom_hid_recv_byte(uint8_t *data);
bool usbd_custom_hid_peek_rx(uint8_t *buf, uint32_t size);
void usbd_custom_hid_discard_rx(uint32_t size);
void usbd_custom_hid_clr_tx(void);
void usbd_custom_hid_clr_rx(void);
bool usbd_custom_hid_empty(void);
bool usbd_custom_hid_available(void);

#endif