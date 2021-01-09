#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/lib/delay.h>

extern gpio_t led;
extern void led_blink(gpio_t *led, int second);

int main(void)
{
	gpio_init(&led);
	
	led_blink(&led, 1);
	
	return 0;
}
