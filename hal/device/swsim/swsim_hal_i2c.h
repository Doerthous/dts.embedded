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

#ifndef SWSIM_HAL_I2C_H_
#define SWSIM_HAL_I2C_H_

#include <dts_hal_gpio.h>
#include <stddef.h>

#define I2C_PARAM_FREQ      1

typedef struct swsim_hal_i2c
{
    dts_hal_gpio_t scl;
    dts_hal_gpio_t sda;
    size_t period;

    void (*set_pin_mode)(struct swsim_hal_i2c *i2c, dts_hal_gpio_t *gpio, int output);
} swsim_hal_i2c_t;

void swsim_hal_i2c_init(swsim_hal_i2c_t *i2c);
int swsim_hal_i2c_config(swsim_hal_i2c_t *i2c, int param, uint8_t *ret, ...);
// return: 1: ack, 0: nack
int swsim_hal_i2c_write(swsim_hal_i2c_t *i2c, uint8_t data); 
uint8_t swsim_hal_i2c_read(swsim_hal_i2c_t *i2c, int ack);
void swsim_hal_i2c_start(swsim_hal_i2c_t *i2c);
// return: 1: ack, 0: nack
int swsim_hal_i2c_7b_addr(swsim_hal_i2c_t *i2c, uint8_t addr, int rw); 
// return: 1: ack, 0: nack
int swsim_hal_i2c_10b_addr(swsim_hal_i2c_t *i2c, uint16_t addr, int rw);
void swsim_hal_i2c_stop(swsim_hal_i2c_t *i2c);
int swsim_hal_i2c_busy(swsim_hal_i2c_t *i2c);

#endif // SWSIM_HAL_I2C_H_
