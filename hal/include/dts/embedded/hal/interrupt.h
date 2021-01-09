/*
   The MIT License (MIT)

   Copyright (c) 2021 Doerthous

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

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "../../../dts_hal_interrupt.h"

#define irq_t dts_hal_irq_t 
#define irq_hdlr_t dts_hal_irq_hdlr_t  
#define intr_state_t dts_hal_intr_state_t 

#define interrupt_global_enable dts_hal_interrupt_global_enable 
#define interrupt_global_disable dts_hal_interrupt_global_disable 
#define interrupt_enable dts_hal_interrupt_enable 
#define interrupt_disable dts_hal_interrupt_disable 
#define interrupt_get_handler dts_hal_interrupt_get_handler 
#define interrupt_set_handler dts_hal_interrupt_set_handler 
#define interrupt_get_handler_data dts_hal_interrupt_get_handler_data 
#define interrupt_set_handler_data dts_hal_interrupt_set_handler_data 

#define interrupt_critical_enter dts_hal_interrupt_critical_enter 
#define interrupt_critical_exit dts_hal_interrupt_critical_exit 

#endif // INTERRUPT_H_
