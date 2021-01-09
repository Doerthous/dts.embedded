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

#ifndef DTS_HAL_I2C_WRAPPER_H_
#define DTS_HAL_I2C_WRAPPER_H_

// TODO: assert method not NULL

#define dts_hal_i2c_init(i2c) \
    (i2c)->method->init((i2c)->obj)
#define dts_hal_i2c_config(i2c, param, ret, ...) \
    (i2c)->method->config((i2c)->obj, param, ret, ##__VA_ARGS__)
#define dts_hal_i2c_write(i2c, data) \
    (i2c)->method->write((i2c)->obj, data)
#define dts_hal_i2c_read(i2c, buff) \
    (i2c)->method->read((i2c)->obj, buff)
#define dts_hal_i2c_start(i2c) \
    (i2c)->method->start((i2c)->obj)
#define dts_hal_i2c_7b_addr(i2c, addr, rw) \
    (i2c)->method->set_7b_addr((i2c)->obj, addr, rw) 
#define dts_hal_i2c_10b_addr(i2c, addr, rw) \
    (i2c)->method->set_10b_addr((i2c)->obj, addr, rw) 
#define dts_hal_i2c_stop(i2c) \
    (i2c)->method->stop((i2c)->obj)
#define dts_hal_i2c_busy(i2c) \
    (i2c)->method->busy((i2c)->obj)

#endif // DTS_HAL_I2C_WRAPPER_H_
