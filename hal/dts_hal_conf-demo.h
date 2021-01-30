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

#ifndef DTS_HAL_CONF_DEMO_H_
#define DTS_HAL_CONF_DEMO_H_

//#define DTS_HAL_ASSERT_OUTPUT uart1
//#define DTS_HAL_ASSERT_ENABLE
//#define DTS_HAL_GPIO_INTERRUPT_ENABLE

//#define DTS_HAL_LOCK_ENABLE
#ifdef DTS_HAL_LOCK_ENABLE
# define DTS_HAL_LOCK void *hal_lock;
void dts_hal_lock_new(void *hal_obj);
void dts_hal_lock_delete(void *hal_obj);
void dts_hal_lock_lock(void *hal_obj);
void dts_hal_lock_unlock(void *hal_obj);
#else
# define DTS_HAL_LOCK
# define dts_hal_lock_new(hal_obj)
# define dts_hal_lock_delete(hal_obj)
# define dts_hal_lock_lock(hal_obj)
# define dts_hal_lock_unlock(hal_obj)
#endif // DTS_HAL_LOCK_ENABLE

//#define DTS_HAL_UART_LOCK_ENABLE
//#define DTS_HAL_UART_DEFAULT_BAUD_RATE 9600
//#define DTS_HAL_UART_PRINTF_BUFFER_SIZE 256
//#define DTS_HAL_UART_PRINTF_OUTPUT uart1
//#define DTS_HAL_UART_PRINTF_NO_PREFIX

#endif // DTS_HAL_CONF_DEMO_H_
