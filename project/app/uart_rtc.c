#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/rtc.h>
#include <dts/embedded/hal/lib/uart_printf.h>
#include <dts/embedded/lib/delay.h>

void uart_rtc(uart_t *uart, rtc_t *rtc)
{
    rtc_time_t tm;
    while (1) {
        delay_ms(1000);
        rtc_get(rtc, &tm);
        uart_printf(uart, "%04d-%02d-%02d %02d:%02d:%02d\n", 
            tm.tm_year, tm.tm_mon, tm.tm_mday, 
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}
