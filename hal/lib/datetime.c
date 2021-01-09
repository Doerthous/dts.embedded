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

#include "datetime.h"


//#define DEBUG
#ifdef DEBUG
//#include <stdio.h>
#include <lib/uart_printf.h>
#define printf(fmt, ...) uart_printf(&uart1, fmt, __VA_ARGS__);
#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
            printf("assert fault: %s(%d): %s\n", __FILE__, __LINE__, __func__); \
        } \
        do {;} while (!(expr)); \
    } while (!(expr))
#endif


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


/**
 * @brief      count leap year in [1, year]
 *
 * @param      year  The year
 *
 * @return     leap year count.
 *
 * @note       0 is not a valid value. but normal leap determine algorithm will
 *             see 0 as as leap year, though in fact 0 year not exists. if need
 *             0 as a valid value, just return true when it's 0
 */
#define ZERO_LEAP_YEAR  1 // 1 if 0-year is leap year, 0 if is common year.
#define leap_year_count(year) \
    ((year) ? \
        ((((year-1))/4)-(((year-1))/100)+(((year-1))/400)+ZERO_LEAP_YEAR) \
        : ZERO_LEAP_YEAR)

/**
 * @brief      count the number of days passed start from this year, util the
 *             given date.
 *
 * @param[in]  dt    the given date.
 *
 * @return     the number of days passed in this year.
 */
static int passed_days_in_year(const datetime_t *dt)
{
    int8_t sub_32n[] = {0,-1,-5,-6,-8,-9,-11,-12,-13,-15,-16,-18};
    
    return (dt->mon<<5) + sub_32n[dt->mon] + dt->day
        + (dt->mon<=1?0:is_leap_year(dt->year));
}

/**
 * @brief      from dt->year-01-01 go throuth "days" to the new date in this
 *             year. make sure days is no more than 365 or 366 in a leap year.
 *
 * @param      dt    datetime
 * @param[in]  days  the number of days, must less than the total days of the
 *                   given year
 */
static void goto_date_of_year(datetime_t *dt, uint32_t days)
{
    uint8_t mon_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};


    mon_days[1] += is_leap_year(dt->year);

    for (dt->mon = 0; days >= mon_days[dt->mon]; ++dt->mon) {
        days -= mon_days[dt->mon];
    }

    dt->day = days;
}

/**
 * @brief      convert time to seconds
 *
 * @param[in]  dt    datetime
 *
 * @return     seconds passed in the given date, start from the begin of the day
 *             specified by date.
 */
static inline int time_to_seconds(const datetime_t *dt)
{
    return (((dt->hour * 60) + dt->min) * 60) + dt->sec;
}

static int date_compare(const datetime_t *dt1, const datetime_t *dt2)
{
    if (dt1->year != dt2->year) {
        return dt1->year > dt2->year ? 1 : -1;
    }

    if (dt1->mon != dt2->mon) {
        return dt1->mon > dt2->mon ? 1 : -1;
    }

    if (dt1->day != dt2->day) {
        return dt1->day > dt2->day ? 1 : -1;
    }

    return 0;
}

static int time_compare(const datetime_t *dt1, const datetime_t *dt2)
{
    if (dt1->hour != dt2->hour) {
        return dt1->hour > dt2->hour ? 1 : -1;
    }

    if (dt1->min != dt2->min) {
        return dt1->min > dt2->min ? 1 : -1;
    }

    if (dt1->sec != dt2->sec) {
        return dt1->sec > dt2->sec ? 1 : -1;
    }

    return 0;
}

int datetime_compare(const datetime_t *dt1, const datetime_t *dt2)
{
    int cmp;

    if ((cmp = date_compare(dt1, dt2)) != 0) {
        return cmp;
    }

    return time_compare(dt1, dt2);
}

int date_add(datetime_t *now, int days)
{
    int passed_days;
    uint32_t new_year = 0;


    passed_days = passed_days_in_year(now);

    // if days is less than 365, we directly add passed_days on it.
    // if not, we do nothing, in case of days overflow after add passed_days.
    if (!(days/365)) { // if -365 < days < 356
        passed_days > 0 ? (days += passed_days, passed_days = 0) : 0;
    }

    // increase year by days.
    // 
    // support each year has 365 days, by year+=days/365, we move "year" 
    // forward. But, because some years may have 366 day, thus we may move far 
    // away, so we need to go back, witch is implemeted by days -= the number 
    // of leap year between new year and the old one.
    // 
    // also, if we need to add passed_days on days.
    // 
    // loop until days go in (-356, 356)
    while (days/365) { // if days <= -365 or days >= 356
        new_year = now->year + days/365;
        days %= 365;
        days -= (leap_year_count(new_year) - leap_year_count(now->year));
        now->year = new_year;
        passed_days > 0 ? (days += passed_days, passed_days = 0) : 0;
    }
    
    // if days < 0, we go back a year
    if (days < 0) {
        now->year -= 1;
        days += (is_leap_year(now->year) + 365);
    }

    goto_date_of_year(now, days);

    return 1;
}

int time_add(datetime_t *now, int seconds)
{
    int acc = 0;


    if (seconds != 0) {
        seconds = time_to_seconds(now) + seconds;

        acc = seconds/86400;
        seconds %= 86400;

        if (seconds < 0) {
            seconds += 86400;
            acc -= 1;
        }

        now->hour = seconds / 3600;
        now->min = (seconds%3600) / 60;
        now->sec = (seconds%3600) % 60;
    }

    return acc;
}

int datetime_add(datetime_t *now, double seconds)
{
    int days;
    int secs;


    if (seconds == 0) {
        return 1;
    }

    days = seconds / 86400;
    secs = (seconds - (days * 86400));

    date_add(now, days + time_add(now, secs));

    return 1;
}

int date_diff(const datetime_t *dt1, const datetime_t *dt2)
{
    int diff = 0;


    diff += (leap_year_count(dt1->year) - leap_year_count(dt2->year));
    
    diff += ((dt1->year - dt2->year) * 365);

    diff += (passed_days_in_year(dt1) - passed_days_in_year(dt2));

    return diff;
}

int time_diff(const datetime_t *dt1, const datetime_t *dt2)
{
    return time_to_seconds(dt1) - time_to_seconds(dt2);
}

double datetime_diff(const datetime_t *dt1, const datetime_t *dt2)
{
    double diff = 86400;


    diff *= date_diff(dt1, dt2);
    diff += time_diff(dt1, dt2);

    return diff;
}

int datetime_week(int y, int m, int d)
{
    return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
}
