#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/hal/timer.h>

extern gpio_t led;
extern uart_t uart1;
	
void dts_hal_default_handler(void *p, ...)
{
}

uint8_t buf[512];

extern timer_t tim1;
void dts_hal_tick_init(timer_t *t);
int main(void)
{
	gpio_init(&led);
	
	uart_init(&uart1);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT, 4);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_BUFFER, buf, 256);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_MODE, STM32F10X_HAL_UART_RX_INTR_MODE);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_BAUDRATE, 115200);
	
	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	dts_hal_tick_init(&tim1);
	
	interrupt_global_enable();
	
	void ili9xxx_test(void);
	ili9xxx_test();
	
	return 0;
}
