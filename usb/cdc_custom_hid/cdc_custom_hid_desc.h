/**
  **************************************************************************
  * @file     cdc_custom_hid_desc.h
  * @brief    usb cdc + custom hid composite device descriptor header file
  **************************************************************************
  *
  * Copyright (c) 2025, Artery Technology, All rights reserved.
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __CDC_CUSTOM_HID_DESC_H
#define __CDC_CUSTOM_HID_DESC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"

/** @defgroup USB_cdc_custom_hid_desc_definition
  * @{
  */

/**
  * @brief usb bcd number define
  */
#define CDC_CUSTOM_HID_BCD_NUM                   0x0110

/**
  * @brief usb interface association descriptor (IAD) define
  *        (not provided by usb_std.h, defined locally)
  */
#define USB_DEVICE_IAD_DESC_LEN          0x08
#define USB_DESCIPTOR_TYPE_IAD           0x0B

/**
  * @brief usb descriptor size define
  *        9(cfg) + 8(iad cdc) + 9+5+5+4+5+7(if0 cdc ctrl) + 9+7+7(if1 cdc data)
  *        + 8(iad hid) + 9+9+7+7(if2 hid) = 115
  */
#define USBD_CDC_CUSTOM_HID_CONFIG_DESC_SIZE     115
#define USBD_CDC_CUSTOM_HID_SIZ_STRING_LANGID    4
#define USBD_CDC_CUSTOM_HID_SIZ_STRING_SERIAL    0x1A

/**
  * @brief usb string define(product configuration, interface)
  */
#define USBD_CDC_CUSTOM_HID_DESC_CONFIGURATION_STRING   "CDC+HID Composite Config"
#define USBD_CDC_CUSTOM_HID_DESC_INTERFACE_CDC_STRING   "Virtual ComPort Interface"
#define USBD_CDC_CUSTOM_HID_DESC_INTERFACE_HID_STRING   "Custom HID Interface"

/**
  * @brief usb cdc interrupt endpoint interval define
  */
#define CDC_CUSTOM_HID_BINTERVAL_TIME            0x0A

/**
  * @brief usb custom hid interrupt endpoint interval define
  *        (same value as custom_hid demo)
  */
#define CUSHID_BINTERVAL_TIME                    0x0A

/**
  * @brief usb mcu id address define
  */
#define         MCU_ID1                   (0x1FFFF7E8)
#define         MCU_ID2                   (0x1FFFF7EC)
#define         MCU_ID3                   (0x1FFFF7F0)

/**
  * @}
  */

extern usbd_desc_handler cdc_custom_hid_desc_handler;

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
