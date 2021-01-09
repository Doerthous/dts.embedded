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

#ifndef I2C_H_
#define I2C_H_

#include "../../../dts_hal_i2c.h"

#define i2c_t dts_hal_i2c_t
#define i2c_init dts_hal_i2c_init
#define i2c_config dts_hal_i2c_config
#define i2c_busy dts_hal_i2c_busy
#define i2c_start dts_hal_i2c_start
#define i2c_7b_addr dts_hal_i2c_7b_addr
#define i2c_10b_addr dts_hal_i2c_10b_addr
#define i2c_write dts_hal_i2c_write
#define i2c_read dts_hal_i2c_read
#define i2c_stop dts_hal_i2c_stop

#endif // I2C_H_
