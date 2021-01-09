#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/lib/delay.h>
#include <w25qxx.h>

extern int w25qxx_test(w25qxx_t *w25qxx);
void w25q128_test(w25qxx_t *w25q128, gpio_t *led)
{
    delay_ms(2000);
    
    if (w25qxx_test(w25q128)) {
        gpio_toggle(led);
    }
    else {
        gpio_toggle(led);
        delay_ms(500);
        gpio_toggle(led);
    }
    
    while (1);
}
