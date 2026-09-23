#include "usbd.h"

#ifdef USB_USE_CUSTOM_HID

#define TX_BUF_SIZE	2048
#define RX_BUF_SIZE	2048

static struct
{
	uint8_t tx_ring_buf[TX_BUF_SIZE];					
	uint8_t rx_ring_buf[RX_BUF_SIZE];	
    __IO uint32_t tx_write; 			 
	__IO uint32_t tx_read;
	__IO uint32_t tx_count; 	
    __IO uint32_t rx_write; 			 
	__IO uint32_t rx_read;
	__IO uint32_t rx_count; 			
} custom_hid;

void custom_hid_send_data(usbd_core_type *udev)
{
    if (custom_hid.tx_count >= USBD_CUSTOM_IN_MAXPACKET_SIZE)
    {
        if (custom_hid_class_send_report(udev, custom_hid.tx_ring_buf + custom_hid.tx_read, USBD_CUSTOM_IN_MAXPACKET_SIZE) == USB_OK)
        {
            custom_hid.tx_count -= USBD_CUSTOM_IN_MAXPACKET_SIZE;
            custom_hid.tx_read += USBD_CUSTOM_IN_MAXPACKET_SIZE;
            if (custom_hid.tx_read >= TX_BUF_SIZE)
            {
                custom_hid.tx_read -= TX_BUF_SIZE;
            }
        }
    }
}

void custom_hid_recv_data(const uint8_t *buf, uint32_t size)
{
    uint32_t i, data_len, remain_len;

    data_len = size;
    remain_len = RX_BUF_SIZE - custom_hid.rx_write;
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
    if ((custom_hid.tx_count + USBD_CUSTOM_IN_MAXPACKET_SIZE) > TX_BUF_SIZE)
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
    if (custom_hid.tx_write >= TX_BUF_SIZE)
    {
        custom_hid.tx_write -= TX_BUF_SIZE;
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
        if (++custom_hid.rx_read >= RX_BUF_SIZE) 
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
    if (++custom_hid.rx_read >= RX_BUF_SIZE) 
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
        if (++read >= RX_BUF_SIZE)
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
	if (custom_hid.rx_read >= RX_BUF_SIZE)
	{
		custom_hid.rx_read -= RX_BUF_SIZE;
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

#endif