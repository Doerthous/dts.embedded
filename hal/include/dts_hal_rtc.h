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

#ifndef DTS_HAL_RTC_H_
#define DTS_HAL_RTC_H_

#include <dts_hal_conf.h>
#include "dts_hal_def.h"
#include <dts_hal_rtc_t.h>

int dts_hal_rtc_init(dts_hal_rtc_t *rtc);
int dts_hal_rtc_config(dts_hal_rtc_t *rtc, int cmd, ...);
int dts_hal_rtc_set(dts_hal_rtc_t *rtc, dts_hal_rtc_time_t *time);
int dts_hal_rtc_get(dts_hal_rtc_t *rtc, dts_hal_rtc_time_t *time);

#ifdef DTS_HAL_RTC_WRAPPER
# include <dts_hal_rtc_wrapper.h>
#endif // DTS_HAL_RTC_WRAPPER

#endif // DTS_HAL_RTC_H_
