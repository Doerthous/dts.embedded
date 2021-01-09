#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/lib/uart_printf.h>
#include <dts/embedded/lib/delay.h>

void uart_echo(uart_t *uart)
{
	uint8_t buff[16];
	size_t cnt;
	
	while (1) {
		cnt = uart_read(uart, buff, 16);
		if (cnt) {
			uart_write(uart, (uint8_t *)buff, cnt);
		}
	}
}
