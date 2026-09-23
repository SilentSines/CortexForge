#ifndef ENCODER_H
#define ENCODER_H

#if 1
    #define ENC_DIR_NONE 0
    #define ENC_DIR_CW   1
    #define ENC_DIR_CCW -1
#else
    #define ENC_DIR_NONE 0
    #define ENC_DIR_CW  -1
    #define ENC_DIR_CCW  1
#endif

void encoder_init(void);
void encoder_monitor(void);
void encoder_put_event(int8_t event);
int8_t encoder_get_event(void);
void encoder_clr_fifo(void);
void encoder_cmd(bool en);

#endif