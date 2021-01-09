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

#ifndef DTS_HAL_UART_T_H_
#define DTS_HAL_UART_T_H_

#include <dts_hal_gpio.h>
#include <dts_hal_timer.h>
#include <swsim_hal_uart_cmd.h>
#include "../../lib/uhal_com.h"
#include <stddef.h>
#include <stdint.h>

typedef struct dts_hal_uart
{
    struct uhal_com _;

    size_t (*write)(void *uart, uint8_t *data, size_t size);
    size_t (*read)(void *uart, uint8_t *buff, size_t size);

    int baud_rate;

    void (*callback)(struct dts_hal_uart *uart, int event);

    struct {
        dts_hal_timer_t *tmr;
        uint16_t idx: 4;
        uint16_t dlen: 4;
        uint16_t complete: 1;
        uint16_t callback_enable: 1;
        uint16_t data;
        dts_hal_gpio_t *pin;
    } tx;

    struct {
        dts_hal_timer_t *tmr;
        uint16_t idx: 4;
        uint16_t dlen: 4;
        uint16_t complete: 1;
        uint16_t callback_enable: 1;
        uint16_t shift_reg;
        uint16_t data;
        dts_hal_gpio_t *pin;
    } rx;

    void (*low_level_init)(struct dts_hal_uart *uart);
} dts_hal_uart_t;

#if DTS_HAL_TIMER_SET_CALLBACK_SUPPORT != 1 \
  || DTS_HAL_TIMER_SET_FREQUENCY_SUPPORT != 1
# error DTS HAL Timer Not Support
#endif

#endif // DTS_HAL_UART_T_H_
