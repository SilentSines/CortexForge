/**
  **************************************************************************
  * @file     cdc_custom_hid.c
  * @brief    usb cdc + custom hid composite monitor layer
  **************************************************************************
  *
  * Monitor layer for the composite device. Keeps the original ring-buffer
  * behavior of both single-class demos:
  *  - cdc  : transparent byte stream with 1k tx/rx ring buffers
  *  - hid  : 64-byte report frames (first byte = payload length,
  *           up to 63 payload bytes) with 2k tx/rx ring buffers
  *
  * All cdc_* / custom_hid_* api keep the same names and behavior as the
  * single-class demos, so application code does not need changes.
  *
  **************************************************************************
  */
#include "usbd.h"

#ifdef USB_USE_CDC_CUSTOM_HID

#define STDIO_CDC 1

#define CDC_TX_BUF_SIZE	 1024
#define CDC_RX_BUF_SIZE	 1024
#define HID_TX_BUF_SIZE	 2048
#define HID_RX_BUF_SIZE	 2048

/* ---------------- cdc ring buffer ---------------- */
static struct
{
	uint8_t tx_ring_buf[CDC_TX_BUF_SIZE];
	uint8_t rx_ring_buf[CDC_RX_BUF_SIZE];
	uint32_t tx_write;
    uint32_t tx_read;
 	uint32_t tx_count;
    uint32_t rx_write;
	uint32_t rx_read;
	uint32_t rx_count;
} cdc;

/* ---------------- custom hid ring buffer ---------------- */
static struct
{
	uint8_t tx_ring_buf[HID_TX_BUF_SIZE];
	uint8_t rx_ring_buf[HID_RX_BUF_SIZE];
    __IO uint32_t tx_write;
	__IO uint32_t tx_read;
	__IO uint32_t tx_count;
    __IO uint32_t rx_write;
	__IO uint32_t rx_read;
	__IO uint32_t rx_count;
} custom_hid;

/* ================= cdc part (same logic as usbd_cdc.c) ================= */

static void cdc_send_data(usbd_core_type *udev)
{
    static uint8_t send_zero_packet = 0;
    uint16_t data_len, remain_len;

    if (cdc.tx_count || send_zero_packet)
    {
        send_zero_packet = (cdc.tx_count > 0) ? 1 : 0;

        data_len = (cdc.tx_count > USBD_CDC_IN_MAXPACKET_SIZE) ? USBD_CDC_IN_MAXPACKET_SIZE : cdc.tx_count;
        remain_len = CDC_TX_BUF_SIZE - cdc.tx_read;
        if (remain_len > data_len)
        {
            if (usb_vcp_send_data(udev, &cdc.tx_ring_buf[cdc.tx_read], data_len) == SUCCESS)
            {
                cdc.tx_read += data_len;
                cdc.tx_count -= data_len;
            }
            else
            {
                if (data_len == 0)
                {
                    send_zero_packet = 1;
                }
            }
        }
        else
        {
            if (usb_vcp_send_data(udev, &cdc.tx_ring_buf[cdc.tx_read], remain_len) == SUCCESS)
            {
                cdc.tx_read = 0;
                cdc.tx_count -= remain_len;
            }
            else
            {
                if (remain_len == 0)
                {
                    send_zero_packet = 1;
                }
                return;
            }

            data_len -= remain_len;
            if (data_len)
            {
                if (usb_vcp_send_data(udev, &cdc.tx_ring_buf[cdc.tx_read], data_len) == SUCCESS)
                {
                    cdc.tx_read += data_len;
                    cdc.tx_count -= data_len;
                }
                else
                {
                    if (data_len == 0)
                    {
                        send_zero_packet = 1;
                    }
                }
            }
        }
    }
}

static void cdc_recv_data(usbd_core_type *udev)
{
    uint16_t i, data_len, remain_len;
    cdc_custom_hid_struct_type *pcomp = (cdc_custom_hid_struct_type *)udev->class_handler->pdata;
    cdc_struct_type *pcdc = &pcomp->cdc;

    if(pcdc->g_rx_completed)
    {
        pcdc->g_rx_completed = 0;

        data_len = pcdc->g_rxlen;
        remain_len = CDC_RX_BUF_SIZE - cdc.rx_write;
        remain_len = (remain_len > data_len) ? data_len : remain_len;

        for (i = 0; i < remain_len; i++)
        {
            cdc.rx_ring_buf[cdc.rx_write++] = pcdc->g_rx_buff[i];
        }
        cdc.rx_count += remain_len;

        data_len -= remain_len;
        if (data_len > 0)
        {
            cdc.rx_write = 0;
            for (i = 0; i < data_len; i++)
            {
                cdc.rx_ring_buf[cdc.rx_write++] = pcdc->g_rx_buff[remain_len + i];
            }
            cdc.rx_count += data_len;
        }

        usbd_ept_recv(udev, USBD_CDC_BULK_OUT_EPT, pcdc->g_rx_buff, USBD_CDC_OUT_MAXPACKET_SIZE);
    }
}

void usbd_cdc_monitor(usbd_core_type *udev)
{
    cdc_send_data(udev);
    cdc_recv_data(udev);
}

void usbd_cdc_send(const uint8_t *buf, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < size; i++)
	{
		while (1)
		{
			if (cdc.tx_count < CDC_TX_BUF_SIZE)
			{
				break;
			}
			else if (cdc.tx_count == CDC_TX_BUF_SIZE)
			{
                return;
			}
		}

		cdc.tx_ring_buf[cdc.tx_write] = buf[i];
		if (++cdc.tx_write >= CDC_TX_BUF_SIZE)
		{
			cdc.tx_write = 0;
		}
		cdc.tx_count++;
	}
}

void usbd_cdc_send_byte(uint8_t data)
{
    usbd_cdc_send(&data, 1);
}

uint32_t usbd_cdc_recv(uint8_t *buf, uint32_t size)
{
    uint32_t i;
    uint32_t recv_len = (cdc.rx_count < size) ? cdc.rx_count : size;

    for (i = 0; i < recv_len; i++)
    {
        buf[i] = cdc.rx_ring_buf[cdc.rx_read];
		if (++cdc.rx_read >= CDC_RX_BUF_SIZE)
		{
			cdc.rx_read = 0;
		}
		cdc.rx_count--;
    }
    return recv_len;
}

bool usbd_cdc_recv_byte(uint8_t *data)
{
    if (cdc.rx_count == 0)
	{
		return false;
	}
	else
	{
		*data = cdc.rx_ring_buf[cdc.rx_read];
		if (++cdc.rx_read >= CDC_RX_BUF_SIZE)
		{
			cdc.rx_read = 0;
		}
		cdc.rx_count--;
		return true;
	}
}

bool usbd_cdc_peek_rx(uint8_t *buf, uint32_t size)
{
    uint32_t i, read, count;

    count = cdc.rx_count;
    if (count < size)
	{
		return false;
	}

    read = cdc.rx_read;
    for (i = 0; i < size; i++)
    {
        buf[i] = cdc.rx_ring_buf[read];
        if (++read >= CDC_RX_BUF_SIZE)
		{
			read = 0;
		}
    }
    return true;
}

void usbd_cdc_discard_rx(uint32_t size)
{
    uint32_t count;

    count = cdc.rx_count;
	if (count == 0)
	{
		return;
	}
	if (count > size)
	{
		count = size;
	}

	cdc.rx_read += count;
	if (cdc.rx_read >= CDC_RX_BUF_SIZE)
	{
		cdc.rx_read -= CDC_RX_BUF_SIZE;
	}
	cdc.rx_count -= count;
}

void usbd_cdc_clr_tx(void)
{
    cdc.tx_count = 0;
    cdc.tx_write = 0;
    cdc.tx_read = 0;
}

void usbd_cdc_clr_rx(void)
{
    cdc.rx_count = 0;
    cdc.rx_write = 0;
    cdc.rx_read = 0;
}

void usbd_cdc_custom_hid_monitor(usbd_core_type *udev)
{
    usbd_cdc_monitor(udev);
    usbd_custom_hid_monitor(udev);
}

/* ============ custom hid part (same logic as usbd_custom_hid.c) ============ */

void custom_hid_send_data(usbd_core_type *udev)
{
    if (custom_hid.tx_count >= USBD_CUSTOM_IN_MAXPACKET_SIZE)
    {
        if (custom_hid_class_send_report(udev, custom_hid.tx_ring_buf + custom_hid.tx_read, USBD_CUSTOM_IN_MAXPACKET_SIZE) == USB_OK)
        {
            custom_hid.tx_count -= USBD_CUSTOM_IN_MAXPACKET_SIZE;
            custom_hid.tx_read += USBD_CUSTOM_IN_MAXPACKET_SIZE;
            if (custom_hid.tx_read >= HID_TX_BUF_SIZE)
            {
                custom_hid.tx_read -= HID_TX_BUF_SIZE;
            }
        }
    }
}

void custom_hid_recv_data(const uint8_t *buf, uint32_t size)
{
    uint32_t i, data_len, remain_len;

    data_len = size;
    remain_len = HID_RX_BUF_SIZE - custom_hid.rx_write;
    remain_len = (remain_len > data_len) ? data_len : remain_len;

    for (i = 0; i < remain_len; i++)
    {
        custom_hid.rx_ring_buf[custom_hid.rx_write++] = buf[i];
    }
    custom_hid.rx_count += remain_len;

    data_len -= remain_len;
    if (data_len > 0)
    {
        custom_hid.rx_write = 0;
        for (i = 0; i < data_len; i++)
        {
            custom_hid.rx_ring_buf[custom_hid.rx_write++] = buf[remain_len + i];
        }
        custom_hid.rx_count += data_len;
    }
}

void usbd_custom_hid_monitor(usbd_core_type *udev)
{
    custom_hid_send_data(udev);
}

void usbd_custom_hid_send(const uint8_t *buf, uint32_t size)
{
    if ((custom_hid.tx_count + USBD_CUSTOM_IN_MAXPACKET_SIZE) > HID_TX_BUF_SIZE)
    {
        return;
    }
    if (size < (USBD_CUSTOM_IN_MAXPACKET_SIZE - 1))
    {
        memset(custom_hid.tx_ring_buf + custom_hid.tx_write + 1, 0, USBD_CUSTOM_IN_MAXPACKET_SIZE - 1);
    }
    else
    {
        size = USBD_CUSTOM_IN_MAXPACKET_SIZE - 1;
    }
    custom_hid.tx_ring_buf[custom_hid.tx_write] = size;
    memcpy(custom_hid.tx_ring_buf + custom_hid.tx_write + 1, buf, size);
    custom_hid.tx_count += USBD_CUSTOM_IN_MAXPACKET_SIZE;
    custom_hid.tx_write += USBD_CUSTOM_IN_MAXPACKET_SIZE;
    if (custom_hid.tx_write >= HID_TX_BUF_SIZE)
    {
        custom_hid.tx_write -= HID_TX_BUF_SIZE;
    }
}

void usbd_custom_hid_send_byte(uint8_t data)
{
    usbd_custom_hid_send(&data, 1);
}

uint32_t usbd_custom_hid_recv(uint8_t *buf, uint32_t size)
{
    uint32_t i, count, recv_len;

    CRITICAL_SECTION(count = custom_hid.rx_count);
    recv_len = (count < size) ? count : size;
    for (i = 0; i < recv_len; i++)
    {
        buf[i] = custom_hid.rx_ring_buf[custom_hid.rx_read];
        CRITICAL_SECTION(
        if (++custom_hid.rx_read >= HID_RX_BUF_SIZE)
        {
            custom_hid.rx_read = 0;
        }
        custom_hid.rx_count--);
    }
    return recv_len;
}

bool usbd_custom_hid_recv_byte(uint8_t *data)
{
    uint32_t count;

    CRITICAL_SECTION(count = custom_hid.rx_count);
    if (count == 0)
	{
		return false;
	}

    *data = custom_hid.rx_ring_buf[custom_hid.rx_read];
    CRITICAL_SECTION(
    if (++custom_hid.rx_read >= HID_RX_BUF_SIZE)
    {
        custom_hid.rx_read = 0;
    }
    custom_hid.rx_count--);
    return true;
}

bool usbd_custom_hid_peek_rx(uint8_t *buf, uint32_t size)
{
    uint32_t i, read, count;

    CRITICAL_SECTION(count = custom_hid.rx_count);
    if (count < size)
	{
		return false;
	}

    read = custom_hid.rx_read;
    for (i = 0; i < size; i++)
    {
        buf[i] = custom_hid.rx_ring_buf[read];
        if (++read >= HID_RX_BUF_SIZE)
		{
			read = 0;
		}
    }
    return true;

}

void usbd_custom_hid_discard_rx(uint32_t size)
{
    uint32_t count;

    CRITICAL_SECTION(count = custom_hid.rx_count);
	if (count == 0)
	{
		return;
	}
	if (count > size)
	{
		count = size;
	}
    CRITICAL_SECTION(
	custom_hid.rx_read += count;
	if (custom_hid.rx_read >= HID_RX_BUF_SIZE)
	{
		custom_hid.rx_read -= HID_RX_BUF_SIZE;
	}
	custom_hid.rx_count -= count);
}

void usbd_custom_hid_clr_tx(void)
{
    custom_hid.tx_count = 0;
    custom_hid.tx_write = 0;
    custom_hid.tx_read = 0;
}

void usbd_custom_hid_clr_rx(void)
{
    custom_hid.rx_count = 0;
    custom_hid.rx_write = 0;
    custom_hid.rx_read = 0;
}

bool usbd_custom_hid_empty(void)
{
    return custom_hid.tx_count ? false : true;
}

bool usbd_custom_hid_available(void)
{
    return custom_hid.rx_count ? true : false;
}

#ifdef STDIO_CDC
int fputc(int ch, FILE *f)
{
	UNUSED(f);
	usbd_cdc_send_byte(ch);
	return ch;
}

int fgetc(FILE *f)
{
	uint8_t data;

	UNUSED(f);
	if (usbd_cdc_recv_byte(&data) == false)
	{
		data = 0xFF;
	}
	return data;
}
#endif

#endif