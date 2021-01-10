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

/*
    tick implement based on dts hal timer
*/

#include <dts/embedded/lib/tick.h>
#include <dts/embedded/hal/timer.h>

#if DTS_HAL_TIMER_TICK_SUPPORT != 1
# error DTS HAL timer tick feature not support
#endif // DTS_HAL_TIMER_TICK_SUPPORT

static timer_t *tmr;

void dts_hal_tick_init(timer_t *t)
{
    tmr = t;
    timer_set_tick_freq(tmr, 1000000);
}

size_t tick_ms(void)
{
    static size_t t, l;
    size_t n;

    n = timer_tick(tmr)/1000;
    if (l != n) {
        l = n;
        ++t;
    }

    return t;
}

size_t tick_us(void)
{
    return timer_tick(tmr);
}
