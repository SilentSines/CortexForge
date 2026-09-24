#ifndef TIMER_H
#define TIMER_H

typedef enum
{
    TIMER1,  /* TIM1 Advanced */
    TIMER2,  /* TIM2 General  */
    TIMER3,  /* TIM3 General  */
    TIMER4,  /* TIM4 General  */
    TIMER5,  /* TIM5 General  */
    TIMER6,  /* TIM6 Basic    */
    TIMER7,  /* TIM7 Basic    */
    TIMER8,  /* TIM8 Advanced */
    TIMER9,  /* TIM9 General  */
    TIMER10, /* TIM10 General  */
    TIMER11, /* TIM11 General  */
    TIMER12, /* TIM12 General  */
    TIMER13, /* TIM13 General  */
    TIMER14, /* TIM14 General  */
    TIMER_MAX,
} timer_t;

void timer_init(timer_t timer, uint32_t time, uint32_t irqp, cb_t func);
void timer_cmd(timer_t timer, bool en);

#endif