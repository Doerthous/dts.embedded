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

#ifndef DTS_ELIB_TIMER_H_
#define DTS_ELIB_TIMER_H_

#include <stddef.h>

typedef struct
{
    size_t start;
    size_t interval;
    size_t (*tick)(void);
} dts_elib_timer_t;

dts_elib_timer_t *
dts_elib_timer_init(dts_elib_timer_t *timer, size_t (*tick)(void));
void dts_elib_timer_start(dts_elib_timer_t *timer, size_t interval);
void dts_elib_timer_stop(dts_elib_timer_t *timer);
void dts_elib_timer_restart(dts_elib_timer_t *timer);
int dts_elib_timer_expired(dts_elib_timer_t *timer);
size_t dts_elib_timer_remain(dts_elib_timer_t *timer);

#endif // DTS_ELIB_TIMER_H_
