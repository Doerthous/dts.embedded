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
    delay implement based on elib tick
*/

#include <dts/embedded/lib/delay.h>
#include <dts/embedded/lib/tick.h>

#if defined(DTS_ELIB_DELAY_AS_FPTR)
void dts_elib_delay_ms_tick(size_t ms)
#else
void dts_elib_delay_ms(size_t ms)
#endif
{
    size_t s = tick_ms();
    while (s+ms > tick_ms());
}

#if defined(DTS_ELIB_DELAY_AS_FPTR)
void dts_elib_delay_us_tick(size_t us)
#else
void dts_elib_delay_us(size_t us)
#endif
{
    size_t s = tick_us();
    while (s+us > tick_us());
}
