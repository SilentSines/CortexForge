#ifndef TIMER_H
#define TIMER_H

typedef enum
{
    TMR_UNIT_1,    /* Advanced */
    TMR_UNIT_2,    /* General  */
    TMR_UNIT_3,    /* General  */
    TMR_UNIT_4,    /* General  */
    TMR_UNIT_5,    /* General  */
    TMR_UNIT_6,    /* Basic    */
    TMR_UNIT_7,    /* Basic    */
    TMR_UNIT_8,    /* Advanced */
    TMR_UNIT_9,    /* General  */
    TMR_UNIT_10,   /* General  */
    TMR_UNIT_11,   /* General  */
    TMR_UNIT_12,   /* General  */
    TMR_UNIT_13,   /* General  */
    TMR_UNIT_14,   /* General  */
    TMR_UNIT_MAX,
} tmr_unit_t;

void timer_init(tmr_unit_t timer, uint32_t time, uint32_t irqp, cb_t func);
void timer_cmd(tmr_unit_t timer, bool en);

#endif