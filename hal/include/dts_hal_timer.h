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

#ifndef DTS_HAL_TIMER_H_
#define DTS_HAL_TIMER_H_

#include <dts_hal_conf.h>
#include "dts_hal_def.h"
#include <dts_hal_timer_t.h>

int dts_hal_timer_config(dts_hal_timer_t *timer, int cmd, ...);
void dts_hal_timer_start(dts_hal_timer_t *timer);
size_t dts_hal_timer_tick(dts_hal_timer_t *timer);
void dts_hal_timer_stop(dts_hal_timer_t *timer);

#if DTS_HAL_TIMER_RESTART_SUPPORT == 1
void dts_hal_timer_restart(dts_hal_timer_t *timer);
#endif // DTS_HAL_TIMER_RESTART_SUPPORT

#if DTS_HAL_TIMER_SET_CALLBACK_SUPPORT == 1
int dts_hal_timer_set_callback(dts_hal_timer_t *timer, 
    void (*callback)(dts_hal_timer_t *, void *), void *data);
#endif // DTS_HAL_TIMER_SET_CALLBACK_SUPPORT

#if DTS_HAL_TIMER_SET_FREQUENCY_SUPPORT == 1
int dts_hal_timer_set_frequency(dts_hal_timer_t *timer, int freq);
#endif // DTS_HAL_TIMER_SET_FREQUENCY_SUPPORT

#endif // DTS_HAL_TIMER_H_
