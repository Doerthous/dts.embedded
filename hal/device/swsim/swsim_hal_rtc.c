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

#ifndef DTS_SWSIM_HAL_RTC
# include "swsim_hal_rtc.h"
# define rtc_time_t swsim_hal_rtc_time_t
# define rtc_t swsim_hal_rtc_t
# define rtc_init swsim_hal_rtc_init
# define rtc_config swsim_hal_rtc_config
# define rtc_set swsim_hal_rtc_set
# define rtc_get swsim_hal_rtc_get
#else
# include <dts/embedded/hal/rtc.h>
#endif // DTS_SWSIM_HAL_RTC

#include <dts/embedded/hal/timer.h>

#define RTC_YEAR_START      1970
#define RTC_MONTH_BASE      1
#define RTC_DAY_BASE        1

/**
 * @brief      Determines if leap year.
 *
 * @param      year  The year
 *
 * @return     1 if leap year, 0 otherwise.
 * 
 * @note       0 is also a leap year.
 */
#define is_leap_year(year) \
    ((((((year)%4) == 0) && (((year)%100)!=0)) || ((((year)%400)==0))) ? 1 : 0)

static void rtc_tmr_cb(timer_t * t, void *d)
{
    const static uint8_t md[12] = {
        31,28,31,30,31,30,31,31,30,31,30,31,
    };
    rtc_t *r;


    r = (rtc_t *)d;

    r->tm.tm_sec += 1;
    if (r->tm.tm_sec == 60) {
        r->tm.tm_sec = 0;
        r->tm.tm_min += 1;
        if (r->tm.tm_min == 60) {
            r->tm.tm_min = 0;
            r->tm.tm_hour += 1;
            if (r->tm.tm_hour == 24) {
                r->tm.tm_hour = 0;
                r->tm.tm_mday += 1;
                if (r->tm.tm_mday == 
                    is_leap_year(r->tm.tm_year) + 
                    md[r->tm.tm_mon-RTC_MONTH_BASE]) { // todo what?
                    r->tm.tm_mday = RTC_DAY_BASE;
                    r->tm.tm_mon += 1;
                    if (r->tm.tm_mon == RTC_MONTH_BASE+12) {
                        r->tm.tm_mon = RTC_MONTH_BASE;
                        r->tm.tm_year += 1;
                    }
                }
            }
        }
    }
}

int  rtc_init(rtc_t *rtc)
{
    if (timer_set_callback(rtc->tmr, rtc_tmr_cb, rtc)) {
        if (timer_set_frequency(rtc->tmr, 1)) {
            timer_start(rtc->tmr);
			return 1;
        }
    }

    return 0;   
}

int  rtc_config(rtc_t *rtc, int cmd, ...)
{
    return -1;
}

int  rtc_set(rtc_t *rtc, rtc_time_t *time)
{
    rtc->tm = *time;
    return 1;
}

int  rtc_get(rtc_t *rtc, rtc_time_t *time)
{
    *time = rtc->tm;
    return 1;
}
