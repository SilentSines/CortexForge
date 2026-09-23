#include "usbd.h"
 
usbd_core_type usb_core_dev;

static void usb_clock48m_select(usb_clk48_s clk_s)
{
    crm_clocks_freq_type clocks_struct;

    if (clk_s == USB_CLK_HICK)
    {
        crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);
        crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);
        acc_write_c1(7980);
        acc_write_c2(8000);
        acc_write_c3(8020);
        acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
    }
    else
    {
        crm_clocks_freq_get(&clocks_struct);
        switch (clocks_struct.sclk_freq)
        {
            case 48000000 : crm_usb_clock_div_set(CRM_USB_DIV_1); break;
            case 72000000 : crm_usb_clock_div_set(CRM_USB_DIV_1_5); break;
            case 96000000 : crm_usb_clock_div_set(CRM_USB_DIV_2); break;
            case 120000000 : crm_usb_clock_div_set(CRM_USB_DIV_2_5); break;
            case 144000000 : crm_usb_clock_div_set(CRM_USB_DIV_3); break;
            case 168000000 : crm_usb_clock_div_set(CRM_USB_DIV_3_5); break;
            case 192000000 : crm_usb_clock_div_set(CRM_USB_DIV_4); break;
            default : break;
        }
    }
}

void usbd_init(void)
{
    usb_clock48m_select(USB_CLK_HICK);
    crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);
    usbd_audio_init();
    nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);
    usbd_core_init(&usb_core_dev, USB, &USB_CLASS_HANDLER, &USB_DESC_HANDLER, 0);
    usbd_connect(&usb_core_dev);
}

void usbd_deinit(void)
{
    usbd_disconnect(&usb_core_dev);
    crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, FALSE);
}

void usbd_monitor(void)
{
    if (usb_core_dev.conn_state == USB_CONN_STATE_CONFIGURED)
    {
        USB_MONITOR_FUNC(&usb_core_dev);
    }
}

bool usbd_is_connected(void)
{
    return (usb_core_dev.conn_state == USB_CONN_STATE_CONFIGURED);
}

void usb_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

void usb_delay_us(uint32_t us)
{
    delay_us(us);
}

void USBFS_L_CAN1_RX0_IRQHandler(void)
{
    usbd_irq_handler(&usb_core_dev);
}