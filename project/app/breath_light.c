#include <dts/embedded/hal/pwm.h>
#include <dts/embedded/hal/gpio.h>

#include <math.h>
void pcb(pwm_t *p)
{
    #define step_cnt 200
    static int step = 0;

    pwm_set_duty(p, 100*sin(3.1415926*step/step_cnt));
    step = (step + 1) % step_cnt;
}


void breath_light(pwm_t *pwm, gpio_t *ch)
{
    pwm_set_frequency(pwm, 100);
    pwm_set_duty(pwm, 10);
    pwm_set_channel(pwm, ch, 1);
    pwm_set_callback(pwm, pcb);
    pwm_start(pwm);

    while (1);
}
