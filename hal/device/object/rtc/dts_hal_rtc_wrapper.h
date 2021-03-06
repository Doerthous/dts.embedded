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

#ifndef DTS_HAL_RTC_WRAPPER_H_
#define DTS_HAL_RTC_WRAPPER_H_

// TODO: assert method not NULL

#define dts_hal_rtc_init(rtc) \
    (rtc)->method->init((rtc)->obj)
#define dts_hal_rtc_config(rtc, param, ret, ...) \
    (rtc)->method->config((rtc)->obj, param, ret, ##__VA_ARGS__)
#define dts_hal_rtc_get(rtc, t) \
    (rtc)->method->get((rtc)->obj, t)
#define dts_hal_rtc_set(rtc, t) \
    (rtc)->method->set((rtc)->obj, t)

#endif // DTS_HAL_RTC_WRAPPER_H_
