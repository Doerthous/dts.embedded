#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/timer.h>
#include <dts/embedded/hal/interrupt.h>

void dts_hal_default_handler(void *_, ...) {}

extern gpio_t led;
extern timer_t tim1;
extern void led_blink(gpio_t *led, int second);
void tim1_callback(timer_t *timer, void *data)
{
	gpio_t *led = (gpio_t *)data;
	gpio_toggle(led);
}
int main(void)
{
	gpio_init(&led);
	
	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	//timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_SET_FREQ, 1);
	//timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_SET_CALLBACK, tim1_callback, &led);
	
	timer_set_frequency(&tim1, 1);
	timer_set_callback(&tim1, tim1_callback, &led);
	
	interrupt_global_enable();

	timer_start(&tim1);
	
	while (1);
	
	return 0;
}
