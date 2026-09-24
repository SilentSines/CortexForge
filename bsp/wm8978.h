#ifndef WM8978_H
#define WM8978_H

typedef enum
{
    WM8978_IN_NONE   = 0x00,
    WM8978_IN_MIC_L  = 0x01,
    WM8978_IN_MIC_R  = 0x02,
    WM8978_IN_LINE   = 0x04,
    WM8978_IN_AUX    = 0x08,
    WM8978_IN_DAC    = 0x10,
    WM8978_IN_ADC    = 0x20,
} audio_input_t;

typedef enum
{
    WM8978_OUT_NONE  = 0x00,
    WM8978_OUT_HP_L  = 0x01,
    WM8978_OUT_HP_R  = 0x02,
    WM8978_OUT_SPK   = 0x04,
    WM8978_OUT_AUX   = 0x08,
} audio_output_t;

typedef enum
{
    WM8978_FORMAT_PHILLIPS = 2,
    WM8978_FORMAT_MSB      = 1,
    WM8978_FORMAT_LSB      = 0,
    WM8978_FORMAT_PCM      = 3
} audio_format_t;

typedef enum
{
    WM8978_16BIT = 0,
    WM8978_24BIT = 2,
    WM8978_32BIT = 3
} audio_bits_t;

void wm8978_init(void);
void wm8978_set_cfg(audio_format_t format, audio_bits_t bits);
void wm8978_out_mute(bool en);
void wm8978_power_down(void);
void wm8978_set_audio_path(audio_input_t in_path, audio_output_t out_path);
void wm8978_set_mic_gain(uint8_t gain);
void wm8978_set_line_gain(uint8_t gain);
void wm8978_set_spk_vol(uint8_t volume);
void wm8978_set_ear_vol(uint8_t volume);
uint8_t wm8978_get_spk_vol(void);
uint8_t wm8978_get_ear_vol(void);
void wm8978_notch_filter(uint16_t nfa0, uint16_t nfa1);

#endif
