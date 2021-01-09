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

#ifndef DTS_HAL_GPIO_H_
#define DTS_HAL_GPIO_H_

#include <dts_hal_conf.h>
#include "dts_hal_def.h"
#include "dts_hal_gpio_t.h"

// interfaces
void dts_hal_gpio_init(dts_hal_gpio_t *gpio);
int dts_hal_gpio_config(dts_hal_gpio_t *gpio, int param, ...);
void dts_hal_gpio_write(dts_hal_gpio_t *gpio, int val);
int dts_hal_gpio_read(dts_hal_gpio_t *gpio);

void dts_hal_gpio_set(dts_hal_gpio_t *gpio);
void dts_hal_gpio_reset(dts_hal_gpio_t *gpio);
void dts_hal_gpio_clear(dts_hal_gpio_t *gpio);
void dts_hal_gpio_toggle(dts_hal_gpio_t *gpio);

#if (DTS_HAL_GPIO_INTERRUPT_SUPPORT == 1) \
    && (DTS_HAL_GPIO_INTERRUPT_ENABLE == 1)

enum // Trigger type
{
    DTS_HAL_GPIO_TRIGGER_FALLING,
    DTS_HAL_GPIO_TRIGGER_RISING,
    DTS_HAL_GPIO_TRIGGER_BOTH,
};

void dts_hal_gpio_intr_enable(dts_hal_gpio_t *gpio);
void dts_hal_gpio_intr_disable(dts_hal_gpio_t *gpio);
void 
dts_hal_gpio_intr_set_handler
(
    dts_hal_gpio_t *gpio,
    int trigger, 
    void (*handler)(void *),
    void *data
);

#endif // DTS_HAL_GPIO_INTERRUPT_SUPPORT

#ifdef DTS_HAL_GPIO_WRAPPER
# include "dts_hal_gpio_wrapper.h"
#endif // DTS_HAL_GPIO_WRAPPER

#endif // DTS_HAL_GPIO_H_
