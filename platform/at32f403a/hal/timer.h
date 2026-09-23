#ifndef TIMER_H
#define TIMER_H

typedef enum
{
    TIMER1, /* TMR1 Advanced */
    TIMER2, /* TMR2 General  */
    TIMER3, /* TMR3 General  */
    TIMER4, /* TMR4 General  */
    TIMER5, /* TMR5 General  */
    TIMER6, /* TMR6 Basic    */
    TIMER7, /* TMR7 Basic    */
    TIMER8, /* TMR8 Advanced */
    TIMER9, /* TMR9 General */
    TIMER10, /* TMR10 General */
    TIMER11, /* TMR11 General */
    TIMER12, /* TMR12 General */
    TIMER13, /* TMR13 General */
    TIMER14, /* TMR14 General */
    TIMER_MAX,
} timer_t;

void timer_init(timer_t timer, uint32_t time, uint32_t irqp, cb_t func);
void timer_cmd(timer_t timer, bool en);

#endif