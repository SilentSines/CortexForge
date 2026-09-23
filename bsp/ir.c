#include "bsp.h"

#define IR_TX_FREQ 38000

#define TMR_TICK        560
#define LEADER_H_TIME   8950   // 9ms载波
#define LEADER_L_TIME   4450   // 4.5ms空闲
#define DATA_H_TIME     560    // 560μs载波
#define DATA_1_L_TIME   1680   // 1.68ms空闲(表示1)
#define DATA_0_L_TIME   560   // 560μs空闲(表示0)
#define STOP_H_TIME     560    // 560μs载波
#define STOP_L_TIME     560    // 560μs空闲
#define DELAY_TIME      100240 // 发送间隔

#define IR_TX_PWM_CH            TMR2_PWM_CH3_PA2
#define IR_TX_POLARITY          PWM_POLARITY_LOW
#define IR_TX_PROTOCOL_TIMER    TIMER7

#define IR_TX_STATUS_LED_EN 1
#define IR_TX_STATUS_LED    LED_D1

typedef enum 
{
    IR_STATUS_IDLE,
    IR_STATUS_LEADER_H,
    IR_STATUS_LEADER_L,
    IR_STATUS_DATA_H,
    IR_STATUS_DATA_L,
    IR_STATUS_STOP_H,
    IR_STATUS_STOP_L,
    IR_STATUS_DELAY,
} ir_status_t;

static struct 
{
    ir_status_t status;
    uint8_t byte_idx; 
    uint8_t bit_idx;
    uint32_t tick;
    uint16_t tick_data_l;

    uint8_t *buf;
    uint8_t size;
    uint8_t repeat;
    cb_t cb;
} ir_tx;

static void ir_carrier_on(void)
{
    pwm_start(IR_TX_PWM_CH);
}

void ir_carrier_off(void)
{
    pwm_stop(IR_TX_PWM_CH);
}

void ir_tx_handle(void)
{
    ir_tx.tick += TMR_TICK;
    switch (ir_tx.status)
    {
        case IR_STATUS_LEADER_H :   if (ir_tx.tick >= LEADER_H_TIME)
                                    {
                                        ir_tx.tick = 0;
                                        ir_carrier_off();
                                        ir_tx.status = IR_STATUS_LEADER_L;
                                    }
                                    break;

        case IR_STATUS_LEADER_L :   if (ir_tx.tick >= LEADER_L_TIME)
                                    {
                                        ir_tx.tick = 0;
                                        ir_carrier_on();
                                        ir_tx.status = IR_STATUS_DATA_H;
                                    }
                                    break;

        case IR_STATUS_DATA_H : if (ir_tx.tick >= DATA_H_TIME)
                                {
                                    ir_tx.tick = 0;
                                    ir_carrier_off();
                                    ir_tx.status = IR_STATUS_DATA_L;
                                    if (ir_tx.buf[ir_tx.byte_idx] & (1 << (7 - ir_tx.bit_idx)))
                                    {
                                        ir_tx.tick_data_l = DATA_1_L_TIME;
                                    }
                                    else
                                    {
                                        ir_tx.tick_data_l = DATA_0_L_TIME;
                                    }   
                                }
                                break;

        case IR_STATUS_DATA_L : if (ir_tx.tick >= ir_tx.tick_data_l)
                                {
                                    ir_tx.tick = 0;
                                    ir_carrier_on();
                                    if (++ir_tx.bit_idx >= 8)
                                    {
                                        ir_tx.bit_idx = 0;
                                        if (++ir_tx.byte_idx >= ir_tx.size)
                                        {
                                            ir_tx.status = IR_STATUS_STOP_H;
                                        }
                                        else
                                        {
                                            ir_tx.status = IR_STATUS_DATA_H;
                                        }
                                    }
                                    else
                                    {
                                        ir_tx.status = IR_STATUS_DATA_H;
                                    }
                                }
                                break;

        case IR_STATUS_STOP_H : if (ir_tx.tick >= STOP_H_TIME)
                                {
                                    ir_tx.tick = 0;
                                    ir_carrier_off();
                                    ir_tx.status = IR_STATUS_STOP_L;
                                }
                                break;

        case IR_STATUS_STOP_L : if (ir_tx.tick >= STOP_L_TIME)
                                {
                                    ir_tx.tick = 0;
                                    ir_tx.status = IR_STATUS_DELAY;
                                #if IR_TX_STATUS_LED_EN
                                    led_on(IR_TX_STATUS_LED);
                                #endif
                                }
                                break;

        case IR_STATUS_DELAY :  if (ir_tx.tick >= DELAY_TIME)
                                {
                                    ir_tx.tick = 0;
                                #if IR_TX_STATUS_LED_EN
                                    led_off(IR_TX_STATUS_LED);
                                #endif
                                    if (--ir_tx.repeat == 0)
                                    {
                                        timer_cmd(TIMER7, false);
                                        ir_tx.status = IR_STATUS_IDLE;
                                        if (ir_tx.cb)
                                        {
                                            ir_tx.cb();
                                        }
                                    }
                                    else
                                    {
                                        ir_tx.byte_idx = 0;
                                        ir_tx.bit_idx = 0;
                                        ir_carrier_on();
                                        ir_tx.status = IR_STATUS_LEADER_H;
                                    }
                                }
                                break;

        default : break; 
    }
}

void ir_tx_init(void)
{
    uint32_t res = SystemCoreClock / IR_TX_FREQ;
    pwm_init(IR_TX_PWM_CH, res, IR_TX_FREQ, PWM_POLARITY_HIGH);
    pwm_set_duty(IR_TX_PWM_CH, res >> 1);
    timer_init(IR_TX_PROTOCOL_TIMER, TMR_TICK, 1, ir_tx_handle);
}

void ir_tx_start(uint8_t *buf, uint16_t size, uint8_t repeat, cb_t func)
{
    ir_tx.buf = buf;
    ir_tx.size = size;
    ir_tx.repeat = repeat;
    ir_tx.cb = func;

    ir_tx.status = IR_STATUS_LEADER_H;
    ir_tx.byte_idx = 0;
    ir_tx.bit_idx = 0;
    ir_tx.tick = 0;

    ir_carrier_on();
    timer_cmd(IR_TX_PROTOCOL_TIMER, true);
}

void ir_tx_stop(void)
{
    timer_cmd(IR_TX_PROTOCOL_TIMER, false);
    ir_tx.status = IR_STATUS_IDLE;
    ir_carrier_off();
#if IR_TX_STATUS_LED_EN
    led_off(IR_TX_STATUS_LED);
#endif
}