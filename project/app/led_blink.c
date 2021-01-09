#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/lib/delay.h>

void led_blink(gpio_t *led, int second)
{	
	while (1) {
		delay_ms(second*1000);
		gpio_toggle(led);
	}
}
