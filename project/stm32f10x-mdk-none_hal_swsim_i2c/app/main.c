#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/i2c.h>
#include <dts/embedded/lib/delay.h>
#include <at24cxx.h>

extern gpio_t led;
extern i2c_t i2c1;
extern at24cxx_t at24c02;
void at24c02_test(at24cxx_t *at24c02, gpio_t *led);
int main(void)
{
	gpio_init(&led);
	
	i2c_init(&i2c1);
	at24c02.i2c = &i2c1;
	
	at24c02_test(&at24c02, &led);

	return 0;
}
