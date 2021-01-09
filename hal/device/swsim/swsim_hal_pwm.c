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

#ifndef DTS_SWSIM_HAL_PWM
# include "swsim_hal_pwm.h"
# define pwm_t swsim_hal_pwm_t
# define pwm_init swsim_hal_pwm_init
# define pwm_config swsim_hal_pwm_config
# define pwm_write swsim_hal_pwm_write
# define pwm_read swsim_hal_pwm_read
# define pwm_transfer swsim_hal_pwm_transfer
#else
# include <dts/embedded/hal/pwm.h>
#endif // DTS_SWSIM_HAL_PWM

#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/timer.h>


static void pwm_tmr_cb(timer_t *t, void *da)
{
    pwm_t *p;
  

    p = (pwm_t *)da;

    p->p = (p->p + 1) % p->f2;
	
	if (p->p == 0 && p->callback) {
		p->callback(p);
	}
    if (p->p < p->f1) {
        gpio_write(p->pin, p->level);
    }
    else {
        gpio_write(p->pin, !p->level);
    }
}


static int gcd(int a,int b)
{
	while (a != b) {
		if (a > b) {
			a=a-b;
		}
		else {
			b=b-a;
		}
	}
	
	return a;
}




int pwm_set_duty(pwm_t *pwm, int duty)
{
    int duty_gcd;
	
	if (duty < 0 || duty > 100) {
        return 0;
    }

    pwm->duty = duty;

	if (duty != 0 && duty != 100) {
		duty_gcd = gcd(duty, 100 - duty);
	}
	else {
		duty_gcd = 100;
	}
	
    timer_set_frequency(pwm->tmr, pwm->freq * 100 / duty_gcd);
	
	pwm->f1 = duty / duty_gcd;
	pwm->f2 = 100 / duty_gcd;

    return 1;
}

void pwm_start(pwm_t *pwm) {
    if (pwm_set_duty(pwm, pwm->duty)) {
        timer_set_callback(pwm->tmr, pwm_tmr_cb, (void *)pwm);
        timer_start(pwm->tmr);        
    }
}

int pwm_config(pwm_t *pwm, int cmd, ...)
{
    return -1;
}
