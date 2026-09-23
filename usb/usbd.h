#ifndef USBD_H
#define USBD_H

#include "bsp.h"

#include "usbd_core.h"
#include "usbd_int.h"

#ifdef USB_USE_CDC
  #include "cdc/cdc_class.h"
  #include "cdc/cdc_desc.h"
  #include "cdc/usbd_cdc.h"

  #define USB_CLASS_HANDLER   cdc_class_handler
  #define USB_DESC_HANDLER    cdc_desc_handler
  #define USB_MONITOR_FUNC    usbd_cdc_monitor
#elif defined USB_USE_CUSTOM_HID
  #include "custom_hid/custom_hid_class.h"
  #include "custom_hid/custom_hid_desc.h"
  #include "custom_hid/usbd_custom_hid.h"

  #define USB_CLASS_HANDLER   custom_hid_class_handler
  #define USB_DESC_HANDLER    custom_hid_desc_handler
  #define USB_MONITOR_FUNC    usbd_custom_hid_monitor
#elif defined USB_USE_AUDIO
  #include "audio/audio_class.h"
  #include "audio/audio_desc.h"
  #include "audio/usbd_audio.h"

  #define USB_CLASS_HANDLER   audio_class_handler
  #define USB_DESC_HANDLER    audio_desc_handler
  #define USB_MONITOR_FUNC    usbd_audio_monitor
#elif defined USB_USE_CDC_CUSTOM_HID
  #include "cdc_custom_hid/cdc_custom_hid_class.h"
  #include "cdc_custom_hid/cdc_custom_hid_desc.h"
  #include "cdc_custom_hid/usbd_cdc_custom_hid.h"

  #define USB_CLASS_HANDLER   cdc_custom_hid_class_handler
  #define USB_DESC_HANDLER    cdc_custom_hid_desc_handler
  #define USB_MONITOR_FUNC    usbd_cdc_custom_hid_monitor
#endif

void usbd_init(void);
void usbd_deinit(void);
void usbd_monitor(void);
bool usbd_is_connected(void);

#endif
