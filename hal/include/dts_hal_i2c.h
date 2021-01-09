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

#ifndef DTS_HAL_I2C_H_
#define DTS_HAL_I2C_H_

#include <dts_hal_conf.h>
#include "dts_hal_def.h"
#include <dts_hal_i2c_t.h>

void dts_hal_i2c_init(dts_hal_i2c_t *i2c);
int dts_hal_i2c_config(dts_hal_i2c_t *i2c, int param, uint8_t *ret, ...);
// return: 1: ack, 0: nack
int dts_hal_i2c_write(dts_hal_i2c_t *i2c, uint8_t data); 
uint8_t dts_hal_i2c_read(dts_hal_i2c_t *i2c, int ack);
void dts_hal_i2c_start(dts_hal_i2c_t *i2c);
// return: 1: ack, 0: nack
int dts_hal_i2c_7b_addr(dts_hal_i2c_t *i2c, uint8_t addr, int rw); 
// return: 1: ack, 0: nack
int dts_hal_i2c_10b_addr(dts_hal_i2c_t *i2c, uint16_t addr, int rw);
void dts_hal_i2c_stop(dts_hal_i2c_t *i2c);
int dts_hal_i2c_busy(dts_hal_i2c_t *i2c);

#ifdef DTS_HAL_I2C_WRAPPER
# include <dts_hal_i2c_wrapper.h>
#endif // DTS_HAL_I2C_WRAPPER

#endif // DTS_HAL_I2C_H_
