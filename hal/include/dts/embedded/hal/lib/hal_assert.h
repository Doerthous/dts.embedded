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

#ifndef HAL_ASSERT_H_
#define HAL_ASSERT_H_

#include <dts_hal_conf.h>
#include <dts/embedded/hal/lib/uart_printf.h>

#if defined(DTS_HAL_ASSERT_ENABLE)
# define hal_assert(expr) \
    ((expr) ? (void)0 : \
        uart_printf(&DTS_HAL_ASSERT_OUTPUT, \
            "assert failed, file: %s, line: %d, func: %s\n", \
            (uint8_t *)__FILE__, __LINE__, (uint8_t *)__func__)
#else
# define hal_assert(expr) ((void)0)
#endif

#endif // HAL_ASSERT_H_

