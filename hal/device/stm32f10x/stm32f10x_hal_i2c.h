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

#ifndef STM32F10X_HAL_I2C_H_
#define STM32F10X_HAL_I2C_H_

#include <stm32f10x.h>
#include <stddef.h>

typedef struct
{
    I2C_TypeDef *i2c;
    size_t timeout;
    size_t frequency;
} stm32f10x_hal_i2c_t;

void stm32f10x_hal_i2c_init(stm32f10x_hal_i2c_t *i2c);
int stm32f10x_hal_i2c_config(stm32f10x_hal_i2c_t *i2c, int param, uint8_t *ret, ...);
int stm32f10x_hal_i2c_write(stm32f10x_hal_i2c_t *i2c, uint8_t data); 
uint8_t stm32f10x_hal_i2c_read(stm32f10x_hal_i2c_t *i2c, int ack);
void stm32f10x_hal_i2c_start(stm32f10x_hal_i2c_t *i2c);
int stm32f10x_hal_i2c_7b_addr(stm32f10x_hal_i2c_t *i2c, uint8_t addr, int rw); 
int stm32f10x_hal_i2c_10b_addr(stm32f10x_hal_i2c_t *i2c, uint16_t addr, int rw);
void stm32f10x_hal_i2c_stop(stm32f10x_hal_i2c_t *i2c);
int stm32f10x_hal_i2c_busy(stm32f10x_hal_i2c_t *i2c);

#endif // STM32F10X_HAL_I2C_H_
