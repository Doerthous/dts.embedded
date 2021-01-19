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

#include <dts_elib_timer.h>

dts_elib_timer_t *
dts_elib_timer_init(dts_elib_timer_t *timer, size_t (*tick)(void))
{
    timer->tick = tick;
	
	return timer;
}

void dts_elib_timer_start(dts_elib_timer_t *timer, size_t intv)
{
    timer->interval = intv;
    timer->start = timer->tick();
}

void dts_elib_timer_stop(dts_elib_timer_t *timer)
{
    timer->interval = 0;
}

void dts_elib_timer_restart(dts_elib_timer_t *timer)
{
    timer->start = timer->tick();
}

int dts_elib_timer_expired(dts_elib_timer_t *timer)
{
    return timer->interval > 0 ?
        ((timer->tick() - timer->start) > timer->interval) : 0;
}

size_t dts_elib_timer_remain(dts_elib_timer_t *timer)
{
    return timer->interval > 0 ?
        (timer->interval - (timer->tick() - timer->start)) : 0;
}
