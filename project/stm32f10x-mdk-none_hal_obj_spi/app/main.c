#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/lib/delay.h>
#include <w25qxx.h>

extern gpio_t led;
extern spi_t spi2;
extern spi_t sw_spi2;
extern w25qxx_t w25q128;
void w25q128_test(w25qxx_t *w25q128, gpio_t *led);
int main(void)
{
	gpio_init(&led);
	
	//spi_init(&sw_spi2);
    //w25q128.spi = &sw_spi2,
	
	spi_init(&spi2);
    w25q128.spi = &spi2,

	w25q128_test(&w25q128, &led);
	
	return 0;
}
