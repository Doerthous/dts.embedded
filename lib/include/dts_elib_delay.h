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

#ifndef DTS_EMBEDDED_LIB_DELAY_H_
#define DTS_EMBEDDED_LIB_DELAY_H_

#include <stddef.h>
#include <dts_elib_conf.h>

#if !defined(DTS_ELIB_DELAY_AS_FPTR)
void dts_elib_delay_ms(size_t ms);
void dts_elib_delay_us(size_t us);

#else
typedef void (*dts_elib_delay_t)(size_t);

extern dts_elib_delay_t dts_elib_delay_ms;
extern dts_elib_delay_t dts_elib_delay_us;

dts_elib_delay_t dts_elib_delay_ms_set(dts_elib_delay_t delay_ms);
dts_elib_delay_t dts_elib_delay_us_set(dts_elib_delay_t delay_us);
#endif

#endif // DTS_EMBEDDED_LIB_DELAY_H_
