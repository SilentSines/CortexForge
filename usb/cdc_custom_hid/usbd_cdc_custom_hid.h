#ifndef USBD_CDC_CUSTOM_HID_H
#define USBD_CDC_CUSTOM_HID_H

/* ---------------- composite api ---------------- */
void usbd_cdc_custom_hid_monitor(usbd_core_type *udev);

/* ---------------- cdc(virtual com port) api ---------------- */
void usbd_cdc_monitor(usbd_core_type *udev);
void usbd_cdc_send(const uint8_t *buf, uint32_t size);
void usbd_cdc_send_byte(uint8_t data);
uint32_t usbd_cdc_recv(uint8_t *buf, uint32_t size);
bool usbd_cdc_recv_byte(uint8_t *data);
bool usbd_cdc_peek_rx(uint8_t *buf, uint32_t size);
void usbd_cdc_discard_rx(uint32_t size);
void usbd_cdc_clr_tx(void);
void usbd_cdc_clr_rx(void);

/* ---------------- custom hid api ---------------- */
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
