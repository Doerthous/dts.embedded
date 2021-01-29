#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/lib/delay.h>
#include <dts/embedded/lib/timer.h>
#include <dts/embedded/lib/tick.h>
#include <dts_hal_timer.h>

extern gpio_t led;
extern spi_t spi2;
extern uart_t uart1;

#include <dts/eos.h>

void dts_hal_default_handler(void *_, ...) {}

thrd_t *init_thread;

#include <dts/embedded/hal/lib/uart_printf.h>
void init_thread_entry(void* parameter)
{
	gpio_init(&led);
	
	uart_init(&uart1);
	
	interrupt_global_enable();
	
	while (1) {
		thrd_sleep(tick_from_second(1));
		uart_printf(&uart1, "World!\n");
	}
}
thrd_t *t_thread;
void t_thrd_entry(void *_)
{
	while (1) {
		thrd_sleep(tick_from_second(1));
		uart_printf(&uart1, "Hello!\n");
	}
}
void init_thrd_init(void)
{
	thrd_new(&init_thread, init_thread_entry, NULL, 1024);
	thrd_new(&t_thread, t_thrd_entry, NULL, 1024);
}
