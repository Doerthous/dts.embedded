/*
   The MIT License (MIT)

   Copyright (c) 2019 Doerthous

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   Authour: Doerthous <doerthous@gmail.com>
*/

#ifndef DTS_STM32F10X_HAL_TIMER
# include "stm32f10x_hal_timer.h"
# define timer_t stm32f10x_hal_timer_t
# define timer_config stm32f10x_hal_timer_config
# define timer_start stm32f10x_hal_timer_start
# define timer_restart stm32f10x_hal_timer_restart
# define timer_stop stm32f10x_hal_timer_stop
# define timer_tick stm32f10x_hal_timer_tick
# define timer_set_tick_freq stm32f10x_hal_timer_set_tick_freq
# define timer_set_frequency stm32f10x_hal_timer_set_frequency
# define timer_set_callback stm32f10x_hal_timer_set_callback
#else
# include <dts/embedded/hal/timer.h>
#endif // DTS_STM32F10X_HAL_TIMER

#include <dts/embedded/hal/interrupt.h>

#define ASSERT(expr) do {;} while(!(expr))


static void enable_intr(timer_t *t, void (*hdlr)(void *))
{
    irq_t irq;


    while (1) {
        if (t->tim == TIM1) {
            irq = TIM1_UP_IRQn;
            break;
        }
        if (t->tim == TIM2) {
            irq = TIM2_IRQn;
            break;
        }
        if (t->tim == TIM3) {
            irq = TIM3_IRQn;
            break;
        }
        return;
    }

    interrupt_set_handler(irq, hdlr);
	interrupt_set_handler_data(irq, (void *)t);
    interrupt_enable(irq);
}

static int rcc_clock_config(timer_t *t, FunctionalState state)
{
    if (t->tim == TIM1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, state);
        return 0;
    }
    if (t->tim == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, state);
        return 0;
    }
	if (t->tim == TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, state);
        return 0;
	}

    return -1;
}

static inline int get_source_clk(timer_t *t)
{
    return 72000000;
}

#define timer_cb_t void (*)(void *, void *)

static void isr_for_callback(void *t)
{
    timer_t *timer = (timer_t *)t;

    timer->callback(timer, timer->data);
    TIM_ClearITPendingBit(timer->tim, TIM_IT_Update);
}

#include <math.h>
int timer_set_frequency(timer_t *t, int freq)
{
    int clk, s, p, bs, bp, sm, smi, su;


    clk = get_source_clk(t);
    if ((freq == 0) || (freq > clk)) {
        return 0;
    }

    clk /= freq;
    smi = 0x10000;
	su = ((int)sqrt(clk))+1;
    for (s = clk/0x10000 + 1; s <= su; ++s) {
        p = clk / s;
        sm = clk % s;
        if (sm < smi) {
            smi = sm;
            bs = s;
            bp = p;
            if (sm == 0) {
                break;
            }
        }
    }

    t->tim->CR1 |= (1 << 2); // forbid software interrupt
    t->tim->PSC = bs-1;
    t->tim->ARR = bp-1;
	t->tim->EGR |= TIM_EventSource_Update;

    return 1;
}

int timer_set_callback(timer_t *t, void (*callback)(timer_t *, void *), void *d)
{
    t->callback = callback == NULL ? 
        (timer_cb_t)dts_hal_default_handler : (timer_cb_t)callback;


    if (callback != NULL) {
        TIM_ClearFlag(t->tim, TIM_FLAG_Update);
        TIM_ITConfig(t->tim, TIM_IT_Update, ENABLE);
        enable_intr(t, isr_for_callback);
    }
    else {
        TIM_ITConfig(t->tim, TIM_IT_Update, DISABLE);
        TIM_ClearFlag(t->tim, TIM_FLAG_Update);
    }

    t->data = d;

    return 1;
}

#include <stdarg.h>
int timer_config(timer_t *timer, int cmd, ...)
{
    va_list args;
    int ret = -1;
	
    
    va_start(args, cmd);

    switch (cmd) {
        case STM32F10X_HAL_TIMER_CMD_SET_FREQ:
            if (timer_set_frequency(timer, va_arg(args, int))) {
                ret = 0;
            }
            break;
        case STM32F10X_HAL_TIMER_CMD_SET_CALLBACK:
            if (timer_set_callback(timer, 
                va_arg(args, void (*)(timer_t *, void *)), NULL)) {
                timer->data = va_arg(args, void *);
                ret = 0;
            }
            break;
        case STM32F10X_HAL_TIMER_CMD_CLK_ON:
            rcc_clock_config(timer, ENABLE);
            break;
        case STM32F10X_HAL_TIMER_CMD_CLK_OFF:
            rcc_clock_config(timer, DISABLE);
            break;
    }

    va_end(args);

    return ret;
}

void timer_start(timer_t *timer)
{
    timer->tim->CR1 |= (1 << 2);
    timer->tim->CR1 |= (1 << 0);
}

void timer_stop(timer_t *timer)
{
    timer->tim->CR1 &= ~(1 << 0);
}

void timer_restart(timer_t *timer)
{
    timer->tim->CNT = 0;
    dts_hal_timer_start(timer);
}

/**
 * @brief Tick Feature
 *          User can use there interfaces to configure the frequency of counting
 *          of the given hw-timer. And access the **counting value**.
 *          This feature is mutually exclusive from **Callback** feature.
 */
size_t timer_tick(timer_t *timer)
{
	return timer->tick | timer->tim->CNT;
}
static void isr_for_tick(void *t)
{
    timer_t *timer = (timer_t *)t;
    timer->tick += 0x10000;
    TIM_ClearITPendingBit(timer->tim, TIM_IT_Update);
}
int timer_set_tick_freq(timer_t *timer, int freq)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    int src_clk = get_source_clk(timer);

    TIM_DeInit(timer->tim);
    timer->tick = 0;
    
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = src_clk/freq-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(timer->tim, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(timer->tim, TIM_FLAG_Update);
    TIM_ITConfig(timer->tim, TIM_IT_Update, ENABLE);
    enable_intr(timer, isr_for_tick);
	
    TIM_Cmd(timer->tim, ENABLE);
	
	return 1;
}
