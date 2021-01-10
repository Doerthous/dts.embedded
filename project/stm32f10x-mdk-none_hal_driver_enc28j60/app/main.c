#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/lib/delay.h>
#include <enc28j60.h>

extern gpio_t led;
extern spi_t spi2;
extern spi_t sw_spi2;
extern enc28j60_t enc28j60;
int enc28j60_test(enc28j60_t *enc28j60);
int main(void)
{
	gpio_init(&led);
	
	spi_init(&spi2);
    enc28j60.spi = &spi2;

	if (enc28j60_test(&enc28j60)) {
		gpio_toggle(&led);
	}
	else {
		gpio_toggle(&led);
		delay_ms(500);
		gpio_toggle(&led);
	}
	
	while (1);
	
	return 0;
}
