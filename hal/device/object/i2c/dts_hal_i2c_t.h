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

#ifndef DTS_HAL_I2C_T_H_
#define DTS_HAL_I2C_T_H_

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    void (*init)(void *i2c);
    int (*config)(void *i2c, int param, uint8_t *ret, ...);
    int (*write)(void *i2c, uint8_t data);
    uint8_t (*read)(void *i2c, int ack);

    void (*start)(void *i2c);
    int (*set_7b_addr)(void *i2c, uint8_t addr, int rw); 
    int (*set_10b_addr)(void *i2c, uint16_t addr, int rw);
    void (*stop)(void *i2c);
    int (*busy)(void *i2c);
} dts_hal_i2c_method_t;

typedef struct {
    void *obj;
    dts_hal_i2c_method_t *method;
} dts_hal_i2c_t;


#define DTS_HAL_I2C_METHOD(type) dts_hal_##type##_i2c_method
#define DTS_HAL_I2C_METHOD_DEFINE(type, \
    _init, _config, _write, _read, \
    _start, _stop, _busy, \
    _set_7b_addr, _set_10b_addr) \
dts_hal_i2c_method_t DTS_HAL_I2C_METHOD(type) = \
{ \
    .init = (void (*)(void *))_init, \
    .config = (int (*)(void *,int,uint8_t *,...))_config, \
    .write = (int (*)(void *,uint8_t))_write, \
    .read = (uint8_t (*)(void *,int))_read, \
    .start = (void (*)(void *))_start, \
    .stop = (void (*)(void *))_stop, \
    .busy = (int (*)(void *))_busy, \
    .set_7b_addr = (int (*)(void *,uint8_t,int))_set_7b_addr, \
    .set_10b_addr = (int (*)(void *,uint16_t,int))_set_10b_addr, \
};
#define DTS_HAL_I2C_METHOD_DECLARE(type) \
    extern dts_hal_i2c_method_t DTS_HAL_I2C_METHOD(type);

#define DTS_HAL_I2C_WRAPPER

#endif // DTS_HAL_I2C_T_H_
