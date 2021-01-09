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

#ifndef STM32F10X_HAL_INTERRUPT_H_
#define STM32F10X_HAL_INTERRUPT_H_

#include <stm32f10x.h>

typedef IRQn_Type stm32f10x_hal_irq_t;
typedef uint8_t stm32f10x_hal_intr_state_t;

typedef void (*stm32f10x_hal_irq_hdlr_t)(void *);

void stm32f10x_hal_interrupt_global_enable(void);
void stm32f10x_hal_interrupt_global_disable(void);
void stm32f10x_hal_interrupt_enable(stm32f10x_hal_irq_t irq);
void stm32f10x_hal_interrupt_disable(stm32f10x_hal_irq_t irq);
stm32f10x_hal_irq_hdlr_t stm32f10x_hal_interrupt_get_handler(stm32f10x_hal_irq_t irq);
void stm32f10x_hal_interrupt_set_handler(stm32f10x_hal_irq_t irq, stm32f10x_hal_irq_hdlr_t handler);
void *stm32f10x_hal_interrupt_get_handler_data(stm32f10x_hal_irq_t irq);
void stm32f10x_hal_interrupt_set_handler_data(stm32f10x_hal_irq_t irq, void *data);

stm32f10x_hal_intr_state_t stm32f10x_hal_interrupt_critical_enter(void);
void stm32f10x_hal_interrupt_critical_exit(stm32f10x_hal_intr_state_t state);

#endif // STM32F10X_HAL_INTERRUPT_H_
