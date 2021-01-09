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

#ifndef DTS_HAL_PWM_H_
#define DTS_HAL_PWM_H_

#include <dts_hal_conf.h>
#include "dts_hal_def.h"
#include <dts_hal_pwm_t.h>

#include <dts_hal_gpio.h>

void dts_hal_pwm_start(dts_hal_pwm_t *pwm);
void dts_hal_pwm_stop(dts_hal_pwm_t *pwm);
int dts_hal_pwm_config(dts_hal_pwm_t *pwm, int cmd, ...);
int dts_hal_pwm_set_frequency(dts_hal_pwm_t *pwm, int freq);
int dts_hal_pwm_set_duty(dts_hal_pwm_t *pwm, int duty);
int dts_hal_pwm_set_channel(dts_hal_pwm_t *pwm, dts_hal_gpio_t *ch, int level);
int dts_hal_pwm_set_callback(dts_hal_pwm_t *pwm, 
    void (*callback)(dts_hal_pwm_t *));

#ifdef DTS_HAL_PWM_WRAPPER
# include "dts_hal_pwm_wrapper.h"
#endif // DTS_HAL_PWM_WRAPPER

#endif // DTS_HAL_PWM_H_
