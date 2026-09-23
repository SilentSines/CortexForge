#ifndef USBD_AUDIO_H
#define USBD_AUDIO_H

#define USBD_AUDIO_I2S    I2S3

/* mic ring buffer: one element = one 32-bit subframe (one channel sample),
 * counters (mic_wtotal/mic_rtotal/mic_delta) count subframes,
 * mic_rx_size = subframes per 1ms usb packet */
#define MIC_BUFFER_SIZE   1024
#define SPK_BUFFER_SIZE   4096
#define DMA_BUFFER_SIZE   384

typedef struct
{
  uint32_t audio_freq;
  uint32_t audio_bitw;
  //spk part
  uint16_t spk_buffer[SPK_BUFFER_SIZE];
  uint16_t *spk_roff;
  uint16_t *spk_woff;
  uint16_t *spk_rend;
  uint32_t spk_freq;
  uint32_t spk_wtotal;
  uint32_t spk_rtotal;
  uint16_t spk_threshold;
  uint16_t spk_calc;
  uint8_t  spk_stage;

  //mic part
  uint32_t mic_buffer[MIC_BUFFER_SIZE];
  uint32_t  *mic_roff;
  uint32_t  *mic_woff;
  uint32_t  *mic_wend;
  uint32_t mic_wtotal;
  uint32_t mic_rtotal;
  uint32_t mic_wtotal_last;   /* wtotal snapshot at last adj window */
  int32_t  mic_adjust;        /* pending length adjust in bytes */
  uint32_t mic_delta;
  uint16_t mic_calc;
  uint16_t mic_adj_count;
  uint8_t  mic_adj_stage;
  uint8_t  mic_stage;

  uint8_t mic_mute;
  uint8_t spk_mute;

  uint16_t mic_volume;
  uint16_t spk_volume;

  uint32_t spk_tx_size;
  uint32_t mic_rx_size;

  uint32_t spk_enable;
  uint32_t mic_enable;

} usbd_audio_t;

void usbd_audio_init(void);
void usbd_audio_param_config(void);
void usbd_audio_monitor(usbd_core_type *udev);
void usbd_audio_set_spk_freq(uint32_t freq);
void usbd_audio_set_spk_volume(uint16_t volume);
void usbd_audio_set_spk_mute(uint8_t mute);
void usbd_audio_spk_alt_setting(uint32_t alt_seting);
void usbd_audio_spk_put_data(uint8_t *data, uint32_t len);
uint8_t usbd_audio_spk_feedback(uint8_t *feedback);
void usbd_audio_set_mic_freq(uint32_t freq);
void usbd_audio_set_mic_volume(uint16_t volume);
void usbd_audio_set_mic_mute(uint8_t mute);
void usbd_audio_mic_alt_setting(uint32_t alt_seting);
uint16_t usbd_audio_mic_get_data(uint8_t *buf);

#endif