#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/lib/delay.h>
#include <at24cxx.h>

extern int at24cxx_test(at24cxx_t *at24cxx);
void at24c02_test(at24cxx_t *at24c02, gpio_t *led)
{
    delay_ms(2000);
    
    if (at24cxx_test(at24c02)) {
        gpio_toggle(led);
    }
    else {
        gpio_toggle(led);
        delay_ms(500);
        gpio_toggle(led);
    }
    
    while (1);
}
