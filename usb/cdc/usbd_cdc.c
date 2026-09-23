#include "usbd.h"

#ifdef USB_USE_CDC

#define STDIO_CDC 1
#define TX_BUF_SIZE	1024
#define RX_BUF_SIZE	1024

static struct
{
	uint8_t tx_ring_buf[TX_BUF_SIZE];					
	uint8_t rx_ring_buf[RX_BUF_SIZE];		 			 
	uint32_t tx_write;
    uint32_t tx_read;	
 	uint32_t tx_count; 
    uint32_t rx_write; 
	uint32_t rx_read;
	uint32_t rx_count; 			
} cdc;

static void cdc_send_data(usbd_core_type *udev)
{
    static uint8_t send_zero_packet = 0;
    uint16_t data_len, remain_len;

    if (cdc.tx_count || send_zero_packet)
    {
        send_zero_packet = (cdc.tx_count > 0) ? 1 : 0;
    
        data_len = (cdc.tx_count > USBD_CDC_IN_MAXPACKET_SIZE) ? USBD_CDC_IN_MAXPACKET_SIZE : cdc.tx_count;
        remain_len = TX_BUF_SIZE - cdc.tx_read;
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
    cdc_struct_type *pcdc = (cdc_struct_type *)udev->class_handler->pdata;

    if(pcdc->g_rx_completed)
    {
        pcdc->g_rx_completed = 0;

        data_len = pcdc->g_rxlen;
        remain_len = RX_BUF_SIZE - cdc.rx_write;
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
			if (cdc.tx_count < TX_BUF_SIZE)
			{
				break;
			}
			else if (cdc.tx_count == TX_BUF_SIZE)
			{
                return;
			}
		}

		cdc.tx_ring_buf[cdc.tx_write] = buf[i];
		if (++cdc.tx_write >= TX_BUF_SIZE)
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
		if (++cdc.rx_read >= RX_BUF_SIZE)
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
		if (++cdc.rx_read >= RX_BUF_SIZE)
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
        if (++read >= RX_BUF_SIZE)
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
	if (cdc.rx_read >= RX_BUF_SIZE)
	{
		cdc.rx_read -= RX_BUF_SIZE;
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