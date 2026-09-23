#include "usbd.h"

#ifdef USB_USE_AUDIO

static usbd_audio_t usbd_audio;
uint16_t spk_dma_buffer[DMA_BUFFER_SIZE];
uint16_t mic_dma_buffer[DMA_BUFFER_SIZE];

static void mic_buf_update(uint16_t *buf)
{
    uint16_t i, n = usbd_audio.mic_rx_size; /* 32-bit subframes per 1ms */

    if (usbd_audio.mic_stage)
    {
        /* 24-bit data in 32-bit frame arrives as two halfwords, msb half
         * first; merge into one word, the sample sits in bits [31:8] */
        for (i = 0; i < n; ++i)
        {
            usbd_audio.mic_woff[i] = ((uint32_t)buf[2 * i] << 16) | buf[2 * i + 1];
        }
        usbd_audio.mic_woff += n;
        usbd_audio.mic_wtotal += n;
        if (usbd_audio.mic_woff >= usbd_audio.mic_wend)
        {
            usbd_audio.mic_woff = usbd_audio.mic_buffer;
        }
        if (usbd_audio.mic_stage == 2)
        {
            if (1024 == ++usbd_audio.mic_calc)
            {
                /* feedback loop: compare real i2s production vs usb consumption
                 * over the last window, then spread the error as +-6 bytes
                 * (one stereo frame) per usb packet until absorbed */
                uint32_t produced = usbd_audio.mic_wtotal - usbd_audio.mic_wtotal_last;
                uint32_t consumed = usbd_audio.mic_delta;
                usbd_audio.mic_wtotal_last = usbd_audio.mic_wtotal;
                usbd_audio.mic_delta = 0;

                if (produced > consumed)
                { /* i2s faster than usb: grow packets to drain the ring */
                    usbd_audio.mic_adjust = (int32_t)(produced - consumed) * 3; /* bytes, 3 bytes/subframe */
                    usbd_audio.mic_adj_stage = 1;
                }
                else if (consumed > produced)
                { /* i2s slower than usb: shrink packets to refill the ring */
                    usbd_audio.mic_adjust = (int32_t)(consumed - produced) * 3;
                    usbd_audio.mic_adj_stage = 2;
                }
                else
                {
                    usbd_audio.mic_adjust = 0;
                    usbd_audio.mic_adj_stage = 0;
                }
                usbd_audio.mic_calc = 0;
                if (usbd_audio.mic_rtotal >= 0x80000000)
                {
                    CRITICAL_SECTION(
                    usbd_audio.mic_rtotal -= 0x80000000;
                    usbd_audio.mic_wtotal -= 0x80000000;
                    usbd_audio.mic_wtotal_last -= 0x80000000;);
                }
            }
            /* ring wraps at mic_wend, so resync one packet before the writer
             * can lap the reader and overwrite unread samples */
            if (usbd_audio.mic_wtotal >= usbd_audio.mic_rtotal +
                (uint32_t)(usbd_audio.mic_wend - usbd_audio.mic_buffer) - usbd_audio.mic_rx_size)
            {
                CRITICAL_SECTION(
                usbd_audio.mic_delta = usbd_audio.mic_wtotal = usbd_audio.mic_rtotal = 0;
                usbd_audio.mic_wtotal_last = 0;
                usbd_audio.mic_woff = usbd_audio.mic_roff = usbd_audio.mic_buffer;
                usbd_audio.mic_stage = 0;
                usbd_audio.mic_adj_stage = 0;
                usbd_audio.mic_adj_count = 0;
                usbd_audio.mic_calc = 0;
                usbd_audio.mic_adjust = 0;);
            }
        }
    }
}

static void mic_rx_cb(void *param)
{
    uint32_t flag = (uint32_t)param;
    mic_buf_update(mic_dma_buffer + (flag == DMA_HDT_FLAG ? 0 : usbd_audio.mic_rx_size * 2));
}

void usbd_audio_init(void)
{
    usbd_audio.audio_freq = AUDIO_FREQ_48K;
    usbd_audio.audio_bitw = AUDIO_BITW_16;
    usbd_audio.mic_volume = 0x100; /* unity until the host sets the level */
    usbd_audio_param_config();
    i2s_bus_init(USBD_AUDIO_I2S, I2S_MASTER_HALF_RX, false);
    i2s_set_cfg(USBD_AUDIO_I2S, I2S_AUDIO_FREQUENCY_48K, I2S_DATA_24BIT_CHANNEL_32BIT);
    i2s_set_rx_dma(USBD_AUDIO_I2S, mic_dma_buffer, usbd_audio.mic_rx_size << 1, true, mic_rx_cb);
    i2s_rx_start(USBD_AUDIO_I2S);
}

void usbd_audio_param_config(void)
{
    usbd_audio.spk_freq = usbd_audio.audio_freq;

    usbd_audio.spk_tx_size = (usbd_audio.audio_freq / 1000) * (usbd_audio.audio_bitw / 8) * AUDIO_SPK_CHANEL_NUM / 2;
    usbd_audio.mic_rx_size = (usbd_audio.audio_freq / 1000) * AUDIO_MIC_CHANEL_NUM; /* 32-bit subframes per ms */

    memset(usbd_audio.spk_buffer, 0, SPK_BUFFER_SIZE * sizeof(uint16_t));
    memset(usbd_audio.mic_buffer, 0, MIC_BUFFER_SIZE * sizeof(usbd_audio.mic_buffer[0]));

    usbd_audio.mic_wend = usbd_audio.mic_woff = usbd_audio.mic_roff = usbd_audio.mic_buffer;
    usbd_audio.spk_rend = usbd_audio.spk_woff = usbd_audio.spk_roff = usbd_audio.spk_buffer;

    /* reset mic stream state machine (called from set_*_freq too) */
    CRITICAL_SECTION(
    usbd_audio.mic_wtotal = usbd_audio.mic_rtotal = 0;
    usbd_audio.mic_wtotal_last = 0;
    usbd_audio.mic_delta = 0;
    usbd_audio.mic_calc = 0;
    usbd_audio.mic_adj_count = 0;
    usbd_audio.mic_adj_stage = 0;
    usbd_audio.mic_adjust = 0;
    usbd_audio.mic_stage = 1;);

    while (usbd_audio.mic_wend < usbd_audio.mic_buffer + MIC_BUFFER_SIZE)
    {
        usbd_audio.mic_wend += usbd_audio.mic_rx_size;
    }
    usbd_audio.mic_wend -= usbd_audio.mic_rx_size;

    while (usbd_audio.spk_rend < usbd_audio.spk_buffer + SPK_BUFFER_SIZE)
    {
        usbd_audio.spk_rend += usbd_audio.spk_tx_size;
    }
    usbd_audio.spk_rend -= usbd_audio.spk_tx_size;
}

void usbd_audio_monitor(usbd_core_type *udev)
{
    
}

void usbd_audio_set_spk_freq(uint32_t freq)
{
    if (usbd_audio.audio_freq != freq)
    {
        usbd_audio.audio_freq = freq;
        i2s_rx_stop(USBD_AUDIO_I2S);
        usbd_audio_param_config();
        i2s_set_cfg(USBD_AUDIO_I2S, usbd_audio.audio_freq, I2S_DATA_24BIT_CHANNEL_32BIT);
        i2s_set_rx_dma(USBD_AUDIO_I2S, mic_dma_buffer, usbd_audio.mic_rx_size << 1, true, mic_rx_cb);
        i2s_rx_start(USBD_AUDIO_I2S);
    }
}

void usbd_audio_set_spk_volume(uint16_t volume)
{
    usbd_audio.spk_volume = (volume * 0x50) / 0x64 + 0x30 - 1;
}

void usbd_audio_set_spk_mute(uint8_t mute)
{
    usbd_audio.spk_mute = mute;
}

void usbd_audio_spk_alt_setting(uint32_t alt_seting)
{
    if (alt_seting == 0)
    {

    }
    else
    {

    }
}

void usbd_audio_spk_put_data(uint8_t *data, uint32_t len)
{
    uint16_t ulen = len / 2, i;
    uint16_t *u16data = (uint16_t *)data;

    switch (usbd_audio.spk_stage)
    {
        case 0 :    usbd_audio.spk_woff = usbd_audio.spk_roff = usbd_audio.spk_buffer;
                    usbd_audio.spk_wtotal = usbd_audio.spk_rtotal = 0;
                    usbd_audio.spk_stage = 1;
                    usbd_audio.spk_threshold = SPK_BUFFER_SIZE / 2;
                    break;

        case 1 :    if (usbd_audio.spk_wtotal >= SPK_BUFFER_SIZE / 2)
                    {
                        usbd_audio.spk_stage = 2;
                    }
                    break;
        case 2  :   break;
    }
    for (i = 0; i < ulen; ++i)
    {
        *usbd_audio.spk_woff++ = *u16data++;
        if (usbd_audio.spk_woff >= usbd_audio.spk_rend)
        {
            usbd_audio.spk_woff = usbd_audio.spk_buffer;
        }
    }
    usbd_audio.spk_wtotal += ulen;
}

uint8_t usbd_audio_spk_feedback(uint8_t *feedback)
{
    uint32_t feedback_value = (usbd_audio.spk_freq);
    feedback_value = ((feedback_value / 1000) << 14) | ((feedback_value % 1000) << 4);
    feedback[0] = (uint8_t)(feedback_value);
    feedback[1] = (uint8_t)(feedback_value >> 8);
    feedback[2] = (uint8_t)(feedback_value >> 16);
    return 3;
}

void usbd_audio_set_mic_freq(uint32_t freq)
{
    if (usbd_audio.audio_freq != freq)
    {
        usbd_audio.audio_freq = freq;
        i2s_rx_stop(USBD_AUDIO_I2S);
        usbd_audio_param_config();
        i2s_set_cfg(USBD_AUDIO_I2S, usbd_audio.audio_freq, I2S_DATA_24BIT_CHANNEL_32BIT);
        i2s_set_rx_dma(USBD_AUDIO_I2S, mic_dma_buffer, usbd_audio.mic_rx_size << 1, true, mic_rx_cb);
        i2s_rx_start(USBD_AUDIO_I2S);
    }
}

void usbd_audio_set_mic_volume(uint16_t volume)
{
    usbd_audio.mic_volume = volume;
    if (usbd_audio.mic_volume > 0x100) /* 0x100 = 0dB, no boost above unity */
    {
        usbd_audio.mic_volume = 0x100;
    }
}

void usbd_audio_set_mic_mute(uint8_t mute)
{
    usbd_audio.mic_mute = mute;
}

void usbd_audio_mic_alt_setting(uint32_t alt_seting)
{
    if (alt_seting == 0)
    {
 
    }
    else
    {

    }
}

uint16_t usbd_audio_mic_get_data(uint8_t *buf)
{
    uint16_t len = usbd_audio.mic_rx_size * 3; /* 3 bytes per subframe (packed 24-bit) */
    uint16_t i;
    uint8_t *buf_8 = buf;


    switch (usbd_audio.mic_stage)
    {
        case 0 :    usbd_audio.mic_stage = 1;
                    memset(buf, 0, len);
                    return len;

        case 1 :    if (usbd_audio.mic_wtotal - usbd_audio.mic_rtotal >= MIC_BUFFER_SIZE / 2)
                    {
                        usbd_audio.mic_stage = 2;
                    }
                    return len;
    }

    switch (usbd_audio.mic_adj_stage)
    {
        case 0 :    break;

        case 1 :    if (usbd_audio.mic_adjust >= 6)
                    {
                        len += 6;
                        usbd_audio.mic_adjust -= 6;
                    }
                    else
                    {
                        usbd_audio.mic_adjust = 0;
                        usbd_audio.mic_adj_stage = 0;
                    }
                    break;
        case 2 :    if (usbd_audio.mic_adjust >= 6)
                    {
                        len -= 6;
                        usbd_audio.mic_adjust -= 6;
                    }
                    else
                    {
                        usbd_audio.mic_adjust = 0;
                        usbd_audio.mic_adj_stage = 0;
                    }
                    break;
    }

    /* 24-bit sample sits in bits [31:8] of the ring word; digital gain,
     * 0x100 = 0dB (set by host via mic volume control), then pack to
     * 3-byte little-endian subframes */
    for (i = 0; i < len / 3; ++i)
    {
        int32_t sample = ((int32_t)*usbd_audio.mic_roff++ >> 8) * usbd_audio.mic_volume >> 8;
        *buf_8++ = (uint8_t)sample;
        *buf_8++ = (uint8_t)(sample >> 8);
        *buf_8++ = (uint8_t)(sample >> 16);
        if (usbd_audio.mic_roff >= usbd_audio.mic_wend)
        {
            usbd_audio.mic_roff = usbd_audio.mic_buffer;
        }
    }

    if (usbd_audio.mic_wtotal <= usbd_audio.mic_rtotal)
    { /* underrun: resync stream instead of deadlock; keep mic_delta so the
         rate-diff window is not biased by the skipped samples */
        usbd_audio.mic_rtotal = usbd_audio.mic_wtotal;
        usbd_audio.mic_roff = usbd_audio.mic_woff;
        usbd_audio.mic_adj_stage = 0;
        usbd_audio.mic_adjust = 0;
    }

    usbd_audio.mic_rtotal += len / 3;
    usbd_audio.mic_delta += len / 3;
    return len;
}

#endif