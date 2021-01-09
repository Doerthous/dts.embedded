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

#ifndef DTS_HAL_GPIO_T_H_
#define DTS_HAL_GPIO_T_H_

#include <stm32f10x.h>
#include "stm32f10x_hal_gpio_cmd.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIOMode_TypeDef mode;
    GPIOSpeed_TypeDef speed;
} stm32f10x_hal_gpio_t;

void stm32f10x_hal_gpio_init(stm32f10x_hal_gpio_t *gpio);
int stm32f10x_hal_gpio_config(stm32f10x_hal_gpio_t *gpio, int param, ...);
void stm32f10x_hal_gpio_write(stm32f10x_hal_gpio_t *gpio, int val);
int stm32f10x_hal_gpio_read(stm32f10x_hal_gpio_t *gpio);

void stm32f10x_hal_gpio_set(stm32f10x_hal_gpio_t *gpio);
void stm32f10x_hal_gpio_reset(stm32f10x_hal_gpio_t *gpio);
void stm32f10x_hal_gpio_clear(stm32f10x_hal_gpio_t *gpio);
void stm32f10x_hal_gpio_toggle(stm32f10x_hal_gpio_t *gpio);

#endif // DTS_HAL_GPIO_T_H_
