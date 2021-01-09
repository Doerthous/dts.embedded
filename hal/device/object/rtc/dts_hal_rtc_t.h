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

#ifndef DTS_HAL_RTC_T_H_
#define DTS_HAL_RTC_T_H_

#include <stddef.h>
#include <stdint.h>
#include <time.h>

typedef struct tm dts_hal_rtc_time_t;

typedef struct
{
    int (*init)(void *rtc);
    int (*config)(void *rtc, int param, ...);
    int (*get)(void *rtc, void *time);
    int (*set)(void *rtc, void *time);
} dts_hal_rtc_method_t;

typedef struct {
    void *obj;
    dts_hal_rtc_method_t *method;
} dts_hal_rtc_t;


#define DTS_HAL_RTC_METHOD(type) dts_hal_##type##_rtc_method
#define DTS_HAL_RTC_METHOD_DEFINE(type, _init, _config, _get, _set) \
dts_hal_rtc_method_t DTS_HAL_RTC_METHOD(type) = \
{ \
    .init = (int (*)(void *))_init, \
    .config = (int (*)(void *,int,...))_config, \
    .get = (int (*)(void *,void *))_get, \
    .set = (int (*)(void *,void *))_set, \
};
#define DTS_HAL_RTC_METHOD_DECLARE(type) \
    extern dts_hal_rtc_method_t DTS_HAL_RTC_METHOD(type);

#define DTS_HAL_RTC_WRAPPER

#endif // DTS_HAL_RTC_T_H_
