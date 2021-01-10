#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/timer.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/lib/tick.h>
#include <dts/embedded/lib/delay.h>

void dts_hal_default_handler(void *_, ...) {}
void dts_hal_tick_init(timer_t *t);

extern gpio_t led;
extern timer_t tim1;

void toggle_led(void)
{
	gpio_toggle(&led);
}
int main(void)
{
	gpio_init(&led);
	
	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	
	interrupt_global_enable();
	dts_hal_tick_init(&tim1);

	void dts_elib_delay_test(void (*do_something)(void), int delay_sec, int count);
	dts_elib_delay_test(toggle_led, 1, 2);
	
	void dts_elib_timer_test(void (*do_something)(void), int interval, int count);
	dts_elib_timer_test(toggle_led, 2000, 2);
		
	while (1);
	
	return 0;
}
