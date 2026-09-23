/**
  **************************************************************************
  * @file     cdc_custom_hid_class.c
  * @brief    usb cdc + custom hid composite class type
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
#include "usbd_core.h"
#include "cdc_custom_hid_class.h"
#include "cdc_custom_hid_desc.h"

#ifdef USB_USE_CDC_CUSTOM_HID

/** @defgroup USB_cdc_custom_hid_class
  * @brief usb device cdc + custom hid composite class
  * @brief setup requests are routed by wIndex interface number:
  *        interface 0/1 -> cdc, interface 2 -> custom hid
  * @{
  */

/** @defgroup USB_cdc_custom_hid_class_private_functions
  * @{
  */

static usb_sts_type class_init_handler(void *udev);
static usb_sts_type class_clear_handler(void *udev);
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup);
static usb_sts_type class_ept0_tx_handler(void *udev);
static usb_sts_type class_ept0_rx_handler(void *udev);
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_sof_handler(void *udev);
static usb_sts_type class_event_handler(void *udev, usbd_event_type event);

/* ---- cdc private (same logic as cdc demo) ---- */
static usb_sts_type cdc_struct_init(cdc_struct_type *pcdc);
static void usb_vcp_cmd_process(void *udev, uint8_t cmd, uint8_t *buff, uint16_t len);
/* ---- custom hid private (same logic as custom_hid demo) ---- */
static void usb_hid_buf_process(void *udev, uint8_t *report, uint16_t len, uint8_t ept_num);

/* defined in cdc_custom_hid.c: feeds the hid rx ring buffer */
extern void custom_hid_recv_data(const uint8_t *buf, uint32_t size);

/* default cdc line coding */
linecoding_type linecoding =
{
  115200,
  0,
  0,
  8
};

/* composite data struct */
static cdc_custom_hid_struct_type cdc_custom_hid_struct;

/* usb device composite class handler */
usbd_class_handler cdc_custom_hid_class_handler =
{
  class_init_handler,
  class_clear_handler,
  class_setup_handler,
  class_ept0_tx_handler,
  class_ept0_rx_handler,
  class_in_handler,
  class_out_handler,
  class_sof_handler,
  class_event_handler,
  &cdc_custom_hid_struct
};

/**
  * @brief  initialize usb endpoint
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_init_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  custom_hid_type *pcshid = &pcomp->hid;

  /* open cdc endpoints */
  usbd_ept_open(pudev, USBD_CDC_INT_EPT, EPT_INT_TYPE, USBD_CDC_CMD_MAXPACKET_SIZE);
  usbd_ept_open(pudev, USBD_CDC_BULK_IN_EPT, EPT_BULK_TYPE, USBD_CDC_IN_MAXPACKET_SIZE);
  usbd_ept_open(pudev, USBD_CDC_BULK_OUT_EPT, EPT_BULK_TYPE, USBD_CDC_OUT_MAXPACKET_SIZE);

  /* set cdc out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_CDC_BULK_OUT_EPT, pcdc->g_rx_buff, USBD_CDC_OUT_MAXPACKET_SIZE);

  /* open custom hid endpoints */
  usbd_ept_open(pudev, USBD_CUSTOM_HID_IN_EPT, EPT_INT_TYPE, USBD_CUSTOM_IN_MAXPACKET_SIZE);
  usbd_ept_open(pudev, USBD_CUSTOM_HID_OUT_EPT, EPT_INT_TYPE, USBD_CUSTOM_OUT_MAXPACKET_SIZE);

  /* set custom hid out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_CUSTOM_HID_OUT_EPT, pcshid->g_rxhid_buff, USBD_CUSTOM_OUT_MAXPACKET_SIZE);

  cdc_struct_init(pcdc);

  pcshid->send_state = 0;

  return status;
}

/**
  * @brief  clear endpoint or other state
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_clear_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  /* close cdc endpoints */
  usbd_ept_close(pudev, USBD_CDC_INT_EPT);
  usbd_ept_close(pudev, USBD_CDC_BULK_IN_EPT);
  usbd_ept_close(pudev, USBD_CDC_BULK_OUT_EPT);

  /* close custom hid endpoints */
  usbd_ept_close(pudev, USBD_CUSTOM_HID_IN_EPT);
  usbd_ept_close(pudev, USBD_CUSTOM_HID_OUT_EPT);

  return status;
}

/**
  * @brief  usb device class setup request handler
  *         dispatch by wIndex interface number
  * @param  udev: to the structure of usbd_core_type
  * @param  setup: setup packet
  * @retval status of usb_sts_type
  */
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  custom_hid_type *pcshid = &pcomp->hid;
  uint16_t len;
  uint8_t *buf;

  /* interface recipient: route by target interface number */
  if((setup->bmRequestType & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_INTERFACE)
  {
    if(setup->wIndex == CDC_CUSTOM_HID_CDC_CTRL_ITF || setup->wIndex == CDC_CUSTOM_HID_CDC_DATA_ITF)
    {
      /* ------------------- cdc interface request ------------------- */
      switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
      {
        /* class request */
        case USB_REQ_TYPE_CLASS:
          if(setup->wLength)
          {
            if(setup->bmRequestType & USB_REQ_DIR_DTH)
            {
              usb_vcp_cmd_process(udev, setup->bRequest, pcdc->g_cmd, setup->wLength);
              usbd_ctrl_send(pudev, pcdc->g_cmd, setup->wLength);
            }
            else
            {
              pcdc->g_req = setup->bRequest;
              pcdc->g_len = setup->wLength;
              usbd_ctrl_recv(pudev, pcdc->g_cmd, pcdc->g_len);
            }
          }
          break;
        /* standard request */
        case USB_REQ_TYPE_STANDARD:
          switch(setup->bRequest)
          {
            case USB_STD_REQ_GET_DESCRIPTOR:
              usbd_ctrl_unsupport(pudev);
              break;
            case USB_STD_REQ_GET_INTERFACE:
              usbd_ctrl_send(pudev, (uint8_t *)&pcdc->alt_setting, 1);
              break;
            case USB_STD_REQ_SET_INTERFACE:
              pcdc->alt_setting = setup->wValue;
              break;
            case USB_STD_REQ_CLEAR_FEATURE:
              break;
            case USB_STD_REQ_SET_FEATURE:
              break;
            default:
              usbd_ctrl_unsupport(pudev);
              break;
          }
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
    }
    else if(setup->wIndex == CDC_CUSTOM_HID_HID_ITF)
    {
      /* ------------------- custom hid interface request ------------------- */
      switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
      {
        /* class request */
        case USB_REQ_TYPE_CLASS:
          switch(setup->bRequest)
          {
            case HID_REQ_SET_PROTOCOL:
              pcshid->hid_protocol = (uint8_t)setup->wValue;
              break;
            case HID_REQ_GET_PROTOCOL:
              usbd_ctrl_send(pudev, (uint8_t *)&pcshid->hid_protocol, 1);
              break;
            case HID_REQ_SET_IDLE:
              pcshid->hid_set_idle = (uint8_t)(setup->wValue >> 8);
              break;
            case HID_REQ_GET_IDLE:
              usbd_ctrl_send(pudev, (uint8_t *)&pcshid->hid_set_idle, 1);
              break;
            case HID_REQ_SET_REPORT:
              pcshid->hid_state = HID_REQ_SET_REPORT;
              usbd_ctrl_recv(pudev, pcshid->hid_set_report, setup->wLength);
              break;
            case HID_REQ_GET_REPORT:
              usbd_ctrl_send(pudev, pcshid->hid_get_report, setup->wLength);
              break;
            default:
              usbd_ctrl_unsupport(pudev);
              break;
          }
          break;
        /* standard request */
        case USB_REQ_TYPE_STANDARD:
          switch(setup->bRequest)
          {
            case USB_STD_REQ_GET_DESCRIPTOR:
              if(setup->wValue >> 8 == HID_REPORT_DESC)
              {
                len = MIN(USBD_CDC_CUSTOM_HID_SIZ_REPORT_DESC, setup->wLength);
                buf = (uint8_t *)g_usbd_cdc_custom_hid_report;
                usbd_ctrl_send(pudev, (uint8_t *)buf, len);
              }
              else if(setup->wValue >> 8 == HID_DESCRIPTOR_TYPE)
              {
                len = MIN(9, setup->wLength);
                buf = (uint8_t *)g_cdc_custom_hid_usb_desc;
                usbd_ctrl_send(pudev, (uint8_t *)buf, len);
              }
              else
              {
                usbd_ctrl_unsupport(pudev);
              }
              break;
            case USB_STD_REQ_GET_INTERFACE:
              usbd_ctrl_send(pudev, (uint8_t *)&pcshid->alt_setting, 1);
              break;
            case USB_STD_REQ_SET_INTERFACE:
              pcshid->alt_setting = setup->wValue;
              break;
            case USB_STD_REQ_CLEAR_FEATURE:
              break;
            case USB_STD_REQ_SET_FEATURE:
              break;
            default:
              usbd_ctrl_unsupport(pudev);
              break;
          }
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
    }
    else
    {
      usbd_ctrl_unsupport(pudev);
    }
    return status;
  }

  /* other recipients: endpoint/device recipient class-specific requests go to cdc */
  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    case USB_REQ_TYPE_CLASS:
      if(setup->wLength)
      {
        if(setup->bmRequestType & USB_REQ_DIR_DTH)
        {
          usb_vcp_cmd_process(udev, setup->bRequest, pcdc->g_cmd, setup->wLength);
          usbd_ctrl_send(pudev, pcdc->g_cmd, setup->wLength);
        }
        else
        {
          pcdc->g_req = setup->bRequest;
          pcdc->g_len = setup->wLength;
          usbd_ctrl_recv(pudev, pcdc->g_cmd, pcdc->g_len);
        }
      }
      break;
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}

/**
  * @brief  usb device class endpoint 0 in status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_tx_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */
  (void)udev;

  return status;
}

/**
  * @brief  usb device class endpoint 0 out status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_rx_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  custom_hid_type *pcshid = &pcomp->hid;
  uint32_t recv_len = usbd_get_recv_len(pudev, 0);

  if(pcdc->g_req == SET_LINE_CODING)
  {
    /* cdc class process */
    usb_vcp_cmd_process(udev, pcdc->g_req, pcdc->g_cmd, recv_len);
  }

  if(pcshid->hid_state == HID_REQ_SET_REPORT)
  {
    /* hid buffer process */
    usb_hid_buf_process(udev, pcshid->hid_set_report, recv_len, 0);
    pcshid->hid_state = 0;
  }

  return status;
}

/**
  * @brief  usb device class transmision complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  custom_hid_type *pcshid = &pcomp->hid;
  usb_sts_type status = USB_OK;

  if(ept_num == (USBD_CDC_BULK_IN_EPT & 0x7F))
  {
    pcdc->g_tx_completed = 1;
  }
  else if(ept_num == (USBD_CUSTOM_HID_IN_EPT & 0x7F))
  {
    pcshid->send_state = 0;
  }
  else
  {
    /* ...user code... */
  }

  return status;
}

/**
  * @brief  usb device class endpoint receive data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  custom_hid_type *pcshid = &pcomp->hid;

  if(ept_num == (USBD_CDC_BULK_OUT_EPT & 0x7F))
  {
    /* cdc: get endpoint receive data length, set recv flag */
    pcdc->g_rxlen = usbd_get_recv_len(pudev, ept_num);
    pcdc->g_rx_completed = 1;
  }
  else if(ept_num == (USBD_CUSTOM_HID_OUT_EPT & 0x7F))
  {
    /* hid: get endpoint receive data length */
    uint32_t recv_len = usbd_get_recv_len(pudev, ept_num);

    /* hid buffer process */
    usb_hid_buf_process(udev, pcshid->g_rxhid_buff, recv_len, USBD_CUSTOM_HID_OUT_EPT);

    /* start receive next packet */
    usbd_ept_recv(pudev, USBD_CUSTOM_HID_OUT_EPT, pcshid->g_rxhid_buff, USBD_CUSTOM_OUT_MAXPACKET_SIZE);
  }
  else
  {
    /* ...user code... */
  }

  return status;
}

/**
  * @brief  usb device class sof handler
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_sof_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */
  (void)udev;

  return status;
}

/**
  * @brief  usb device class event handler
  * @param  udev: to the structure of usbd_core_type
  * @param  event: usb device event
  * @retval status of usb_sts_type
  */
static usb_sts_type class_event_handler(void *udev, usbd_event_type event)
{
  (void)udev;

  usb_sts_type status = USB_OK;
  switch(event)
  {
    case USBD_RESET_EVENT:

      /* ...user code... */

      break;
    case USBD_SUSPEND_EVENT:

      /* ...user code... */

      break;
    case USBD_WAKEUP_EVENT:
      /* ...user code... */

      break;
    default:
      break;
  }
  return status;
}

/* ================= cdc part (same logic as cdc demo) ================= */

/**
  * @brief  usb device cdc init
  * @param  pcdc: to the structure of cdc_struct
  * @retval status of usb_sts_type
  */
static usb_sts_type cdc_struct_init(cdc_struct_type *pcdc)
{
  pcdc->g_tx_completed = 1;
  pcdc->g_rx_completed = 0;
  pcdc->alt_setting = 0;
  pcdc->linecoding.bitrate = linecoding.bitrate;
  pcdc->linecoding.data = linecoding.data;
  pcdc->linecoding.format = linecoding.format;
  pcdc->linecoding.parity = linecoding.parity;
  return USB_OK;
}

/**
  * @brief  usb device class rx data process
  * @param  udev: to the structure of usbd_core_type
  * @param  recv_data: receive buffer
  * @retval receive data len
  */
uint16_t usb_vcp_get_rxdata(void *udev, uint8_t *recv_data)
{
  uint16_t i_index = 0;
  uint16_t tmp_len = 0;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;

  if(pcdc->g_rx_completed == 0)
  {
    return 0;
  }
  pcdc->g_rx_completed = 0;
  tmp_len = pcdc->g_rxlen;
  for(i_index = 0; i_index < pcdc->g_rxlen; i_index ++)
  {
    recv_data[i_index] = pcdc->g_rx_buff[i_index];
  }

  usbd_ept_recv(pudev, USBD_CDC_BULK_OUT_EPT, pcdc->g_rx_buff, USBD_CDC_OUT_MAXPACKET_SIZE);

  return tmp_len;
}

/**
  * @brief  usb device class send data
  * @param  udev: to the structure of usbd_core_type
  * @param  send_data: send data buffer
  * @param  len: send length
  * @retval error status
  */
error_status usb_vcp_send_data(void *udev, uint8_t *send_data, uint16_t len)
{
  error_status status = SUCCESS;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  if(pcdc->g_tx_completed)
  {
    pcdc->g_tx_completed = 0;
    usbd_ept_send(pudev, USBD_CDC_BULK_IN_EPT, send_data, len);
  }
  else
  {
    status = ERROR;
  }
  return status;
}

/**
  * @brief  usb device cdc request function
  * @param  udev: to the structure of usbd_core_type
  * @param  cmd: request number
  * @param  buff: request buffer
  * @param  len: buffer length
  * @retval none
  */
static void usb_vcp_cmd_process(void *udev, uint8_t cmd, uint8_t *buff, uint16_t len)
{
  (void)len;

  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  cdc_struct_type *pcdc = &pcomp->cdc;
  switch(cmd)
  {
    case SET_LINE_CODING:
      pcdc->linecoding.bitrate = (uint32_t)(buff[0] | (buff[1] << 8) | (buff[2] << 16) | (buff[3] <<24));
      pcdc->linecoding.format = buff[4];
      pcdc->linecoding.parity = buff[5];
      pcdc->linecoding.data = buff[6];
#ifdef USB_VIRTUAL_COMPORT
      /* set hardware usart */
      //usb_usart_config(pcdc->linecoding);
#endif
      break;

    case GET_LINE_CODING:
      buff[0] = (uint8_t)pcdc->linecoding.bitrate;
      buff[1] = (uint8_t)(pcdc->linecoding.bitrate >> 8);
      buff[2] = (uint8_t)(pcdc->linecoding.bitrate >> 16);
      buff[3] = (uint8_t)(pcdc->linecoding.bitrate >> 24);
      buff[4] = (uint8_t)(pcdc->linecoding.format);
      buff[5] = (uint8_t)(pcdc->linecoding.parity);
      buff[6] = (uint8_t)(pcdc->linecoding.data);
      break;

    default:
      break;
  }
}

/* ================= custom hid part (same logic as custom_hid demo) ================= */

/**
  * @brief  usb device class send report
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @retval status of usb_sts_type
  */
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len)
{
  usb_sts_type status = USB_FAIL;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)pudev->class_handler->pdata;
  custom_hid_type *pcshid = &pcomp->hid;

  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED && pcshid->send_state == 0)
  {
    pcshid->send_state = 1;
    usbd_ept_send(pudev, USBD_CUSTOM_HID_IN_EPT, report, len);
    status = USB_OK;
  }
  return status;
}

/**
  * @brief  usb device hid report function
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @param  ept_num: endpoint number
  * @retval none
  */
static void usb_hid_buf_process(void *udev, uint8_t *report, uint16_t len, uint8_t ept_num)
{
  UNUSED(udev);
  UNUSED(len);

  if (ept_num == USBD_CUSTOM_HID_OUT_EPT)
  {
    if (report[0])
    {
      custom_hid_recv_data(&report[1], report[0]);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif