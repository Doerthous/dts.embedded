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

#ifndef DATETIME_H_
#define DATETIME_H_

#include <stdint.h>

typedef struct datetime
{
    uint32_t year; // [0, max_value]
    uint8_t mon; // [0, 11]
    uint8_t day; // [0, 30]
    uint8_t hour; // [0, 23]
    uint8_t min; // [0, 59]
    uint8_t sec; // [0, 59]
} datetime_t;

int datetime_compare(const datetime_t *dt1, const datetime_t *dt2);

int date_diff(const datetime_t *dt1, const datetime_t *dt2);
int time_diff(const datetime_t *dt1, const datetime_t *dt2);

/**
 * @brief      diff between dt1 and dt2 in second. (dt1 - dt2)(s)
 *
 * @param[in]  dt1   The datetime 1
 * @param[in]  dt2   The datetime 2
 *
 * @return     difference.
 */
double datetime_diff(const datetime_t *dt1, const datetime_t *dt2);


int time_add(datetime_t *now, int seconds);
int date_add(datetime_t *now, int days);
int datetime_add(datetime_t *now, double seconds);


/**
 * @brief      get weekday of a date
 *
 * @param[in]  y     year
 * @param[in]  m     month, [1, 12]
 * @param[in]  d     day, [1, 31]
 *
 * @return     weekday [0, 6], 0 for sunday, 1 for monday, etc.
 */
int datetime_week(int y, int m, int d);

#endif // DATETIME_H_
