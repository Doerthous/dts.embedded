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

#ifndef DTS_STM32F10X_HAL_INTERRUPT
# include "stm32f10x_hal_interrupt.h"
# define irq_t stm32f10x_hal_irq_t 
# define irq_hdlr_t stm32f10x_hal_irq_hdlr_t  
# define intr_state_t stm32f10x_hal_intr_state_t 
# define interrupt_global_enable stm32f10x_hal_interrupt_global_enable 
# define interrupt_global_disable stm32f10x_hal_interrupt_global_disable 
# define interrupt_enable stm32f10x_hal_interrupt_enable 
# define interrupt_disable stm32f10x_hal_interrupt_disable 
# define interrupt_get_handler stm32f10x_hal_interrupt_get_handler 
# define interrupt_set_handler stm32f10x_hal_interrupt_set_handler 
# define interrupt_get_handler_data stm32f10x_hal_interrupt_get_handler_data 
# define interrupt_set_handler_data stm32f10x_hal_interrupt_set_handler_data 
# define interrupt_critical_enter stm32f10x_hal_interrupt_critical_enter 
# define interrupt_critical_exit stm32f10x_hal_interrupt_critical_exit 
#else
# include <dts/embedded/hal/interrupt.h>
#endif // DTS_STM32F10X_HAL_INTERRUPT


#define IRQ_COUNT       60

static void *hdlr_data[IRQ_COUNT];
static irq_hdlr_t handlers[IRQ_COUNT] = 
{
    [0 ... IRQ_COUNT-1] = (irq_hdlr_t)dts_hal_default_handler,
};
static intr_state_t state;


#define INVOKE_HANDLER(irq) \
    handlers[irq](hdlr_data[irq]);

#define DEFINE_HANDLER(name) \
    void name##_IRQHandler(void) { \
        INVOKE_HANDLER(name##_IRQn)\
    }


// stm32f10x irq handlers
DEFINE_HANDLER(USART1);




#ifdef __STM32F10x_TIM_H
DEFINE_HANDLER(TIM1_UP);
DEFINE_HANDLER(TIM2);
DEFINE_HANDLER(TIM3);
#endif


#ifdef __STM32F10x_EXTI_H
DEFINE_HANDLER(EXTI0);
DEFINE_HANDLER(EXTI1);
DEFINE_HANDLER(EXTI2);
DEFINE_HANDLER(EXTI3);
DEFINE_HANDLER(EXTI4);
DEFINE_HANDLER(EXTI9_5);
DEFINE_HANDLER(EXTI15_10);
#endif



// interfaces
void interrupt_global_enable(void)
{
    __enable_irq();
    state = 1;
}

void interrupt_global_disable(void)
{
    __disable_irq();
    state = 0;
}

void interrupt_enable(irq_t irq)
{
  #if defined(USE_STDPERIPH_DRIVER)
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  #else
    #error "not support for register operations"
  #endif 
}

void interrupt_disable(irq_t irq)
{
  #if defined(USE_STDPERIPH_DRIVER)
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
  #else
    #error "not support for register operations"
  #endif
}



irq_hdlr_t interrupt_get_handler(irq_t irq)
{
    return handlers[irq];
}

void interrupt_set_handler(irq_t irq, irq_hdlr_t handler)
{
    handlers[irq] = handler == NULL ? 
        (irq_hdlr_t)dts_hal_default_handler : handler;
}

void *interrupt_get_handler_data(irq_t irq)
{
    return hdlr_data[irq];
}

void interrupt_set_handler_data(irq_t irq, void *data)
{
    hdlr_data[irq] = data;
}



intr_state_t interrupt_critical_enter(void)
{
    intr_state_t old = state;
    

    interrupt_global_disable();
    
    return old;
}

void interrupt_critical_exit(intr_state_t state)
{
    if (state) {
        interrupt_global_enable();
    }
}
