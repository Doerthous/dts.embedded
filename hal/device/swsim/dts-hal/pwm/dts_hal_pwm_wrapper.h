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

#ifndef DTS_HAL_PWM_WRAPPER_H_
#define DTS_HAL_PWM_WRAPPER_H_

#define dts_hal_pwm_stop(pwm) \
    dts_hal_timer_stop((pwm)->tmr)
#define dts_hal_pwm_set_frequency(pwm, _freq) \
    ((pwm)->freq = _freq, 1)  
#define dts_hal_pwm_set_callback(pwm, _callback) \
    ((pwm)->callback = _callback, 1)
#define dts_hal_pwm_set_channel(pwm, _pin, _level) \
    ((pwm)->pin = _pin, (pwm)->level = _level, 1)

#endif // DTS_HAL_PWM_WRAPPER_H_
