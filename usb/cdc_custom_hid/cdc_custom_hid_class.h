/**
  **************************************************************************
  * @file     cdc_custom_hid_class.h
  * @brief    usb cdc + custom hid composite class header file
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
#ifndef __CDC_CUSTOM_HID_CLASS_H
#define __CDC_CUSTOM_HID_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"

/** @defgroup USB_cdc_custom_hid_class_definition
  * @{
  */

/**
  * @brief composite endpoint define
  *        cdc:  bulk in 0x81 / bulk out 0x01 / int in 0x82
  *        hid:  int in 0x83 / int out 0x03
  */
#define USBD_CDC_BULK_IN_EPT              0x81
#define USBD_CDC_BULK_OUT_EPT             0x01
#define USBD_CDC_INT_EPT                  0x82
#define USBD_CUSTOM_HID_IN_EPT            0x83
#define USBD_CUSTOM_HID_OUT_EPT           0x03

/**
  * @brief cdc in and out max packet size define
  */
#define USBD_CDC_IN_MAXPACKET_SIZE        0x40
#define USBD_CDC_OUT_MAXPACKET_SIZE       0x40
#define USBD_CDC_CMD_MAXPACKET_SIZE       0x08

/**
  * @brief hid in and out max packet size define
  */
#define USBD_CUSTOM_IN_MAXPACKET_SIZE     0x40
#define USBD_CUSTOM_OUT_MAXPACKET_SIZE    0x40

/**
  * @brief composite interface index define
  */
#define CDC_CUSTOM_HID_CDC_CTRL_ITF       0
#define CDC_CUSTOM_HID_CDC_DATA_ITF       1
#define CDC_CUSTOM_HID_HID_ITF            2

/**
  * @brief usb hid report descriptor size define
  */
#define USBD_CDC_CUSTOM_HID_SIZ_REPORT_DESC      34

/**
  * @brief hid report id define(keep same as custom_hid demo)
  */
#define HID_REPORT_ID_1                   0x01
#define HID_REPORT_ID_2                   0x02
#define HID_REPORT_ID_3                   0x03
#define HID_REPORT_ID_4                   0x04
#define HID_REPORT_ID_5                   0x05
#define HID_REPORT_ID_6                   0xF0

/**
  * @brief cdc class struct(same as cdc demo)
  */
typedef struct
{
  uint32_t alt_setting;
  uint8_t g_rx_buff[USBD_CDC_OUT_MAXPACKET_SIZE];
  uint8_t g_cmd[USBD_CDC_CMD_MAXPACKET_SIZE];
  uint8_t g_req;
  uint16_t g_len, g_rxlen;
  __IO uint8_t g_tx_completed, g_rx_completed;
  linecoding_type linecoding;
}cdc_struct_type;

/**
  * @brief custom hid class struct(same as custom_hid demo)
  */
typedef struct
{
  uint8_t g_rxhid_buff[USBD_CUSTOM_OUT_MAXPACKET_SIZE];
  uint8_t g_txhid_buff[USBD_CUSTOM_IN_MAXPACKET_SIZE];

  uint32_t hid_protocol;
  uint32_t hid_set_idle;
  uint32_t alt_setting;

  uint8_t hid_set_report[64];
  uint8_t hid_get_report[64];
  uint8_t hid_state;
  uint8_t send_state;
}custom_hid_type;

/**
  * @brief composite class struct
  */
typedef struct
{
  cdc_struct_type cdc;
  custom_hid_type hid;
}cdc_custom_hid_struct_type;

extern uint8_t g_usbd_cdc_custom_hid_report[USBD_CDC_CUSTOM_HID_SIZ_REPORT_DESC];
extern uint8_t g_cdc_custom_hid_usb_desc[9];
extern usbd_class_handler cdc_custom_hid_class_handler;

uint16_t usb_vcp_get_rxdata(void *udev, uint8_t *recv_data);
error_status usb_vcp_send_data(void *udev, uint8_t *send_data, uint16_t len);
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
