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

#ifndef SWSIM_HAL_RTC_H_
#define SWSIM_HAL_RTC_H_

#include <dts_hal_timer.h>
#include <time.h>

typedef struct tm swsim_hal_rtc_time_t;
typedef struct
{
    swsim_hal_rtc_time_t tm;
    dts_hal_timer_t *tmr;
} swsim_hal_rtc_t;

#if DTS_HAL_TIMER_SET_CALLBACK_SUPPORT != 1 \
  || DTS_HAL_TIMER_SET_FREQUENCY_SUPPORT != 1
# error DTS HAL Timer Not Support
#endif

int swsim_hal_rtc_init(swsim_hal_rtc_t *rtc);
int swsim_hal_rtc_config(swsim_hal_rtc_t *rtc, int cmd, ...);
int swsim_hal_rtc_set(swsim_hal_rtc_t *rtc, swsim_hal_rtc_time_t *time);
int swsim_hal_rtc_get(swsim_hal_rtc_t *rtc, swsim_hal_rtc_time_t *time);

#endif // SWSIM_HAL_RTC_H_
