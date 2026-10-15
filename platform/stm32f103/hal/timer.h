#ifndef TIMER_H
#define TIMER_H

typedef enum
{
    TIMER_UNIT_1, /* TIM1 Advanced */
    TIMER_UNIT_2, /* TIM2 General  */
    TIMER_UNIT_3, /* TIM3 General  */
    TIMER_UNIT_4, /* TIM4 General  */
    TIMER_UNIT_5, /* TIM5 General  */
    TIMER_UNIT_6, /* TIM6 Basic    */
    TIMER_UNIT_7, /* TIM7 Basic    */
    TIMER_UNIT_8, /* TIM8 Advanced */
    TIMER_UNIT_MAX,
} timer_unit_t;

void timer_init(timer_unit_t timer, uint32_t time, uint32_t irqp, cb_t func);
void timer_cmd(timer_unit_t timer, bool en);

#endif