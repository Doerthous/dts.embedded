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

#ifndef GPIO_H_
#define GPIO_H_

#include "../../../dts_hal_gpio.h"

#define gpio_t dts_hal_gpio_t
#define gpio_init dts_hal_gpio_init
#define gpio_config dts_hal_gpio_config
#define gpio_write dts_hal_gpio_write
#define gpio_read dts_hal_gpio_read
#define gpio_set dts_hal_gpio_set
#define gpio_reset dts_hal_gpio_reset
#define gpio_clear dts_hal_gpio_clear
#define gpio_toggle dts_hal_gpio_toggle

#if (DTS_HAL_GPIO_INTERRUPT_SUPPORT == 1) \
    && (DTS_HAL_GPIO_INTERRUPT_ENABLE == 1)
# define gpio_intr_enable dts_hal_gpio_intr_enable
# define gpio_intr_disable dts_hal_gpio_intr_disable
# define gpio_intr_set_handler dts_hal_gpio_intr_set_handler
#endif // DTS_HAL_GPIO_INTERRUPT_SUPPORT

#endif // GPIO_H_
