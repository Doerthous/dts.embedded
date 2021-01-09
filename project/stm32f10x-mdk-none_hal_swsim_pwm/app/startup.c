#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/timer.h>
#include <dts/embedded/hal/pwm.h>
#include <dts/embedded/hal/interrupt.h>

void dts_hal_default_handler(void *_, ...) {}

extern gpio_t led;
extern timer_t tim1;
extern pwm_t pwm1;
extern void breath_light(pwm_t *pwm, gpio_t *ch);
int main(void)
{
	gpio_init(&led);
	
	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);

	interrupt_global_enable();
	
	pwm1.tmr = &tim1;
	breath_light(&pwm1, &led);
	
	return 0;
}
