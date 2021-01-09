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

#ifndef TIMER_H_
#define TIMER_H_

#include "../../../dts_hal_timer.h"

#define timer_t dts_hal_timer_t
#define timer_config dts_hal_timer_config
#define timer_start dts_hal_timer_start
#define timer_tick dts_hal_timer_tick
#define timer_stop dts_hal_timer_stop

#if DTS_HAL_TIMER_RESTART_SUPPORT == 1
# define timer_restart dts_hal_timer_restart
#endif // DTS_HAL_TIMER_RESTART_SUPPORT

#if DTS_HAL_TIMER_SET_CALLBACK_SUPPORT == 1
# define timer_set_callback dts_hal_timer_set_callback
#endif // DTS_HAL_TIMER_SET_CALLBACK_SUPPORT

#if DTS_HAL_TIMER_SET_FREQUENCY_SUPPORT == 1
# define timer_set_frequency dts_hal_timer_set_frequency
#endif // DTS_HAL_TIMER_SET_FREQUENCY_SUPPORT

#endif // TIMER_H_
