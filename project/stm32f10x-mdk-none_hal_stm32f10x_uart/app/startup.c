#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/interrupt.h>

extern gpio_t led;
extern uart_t uart1;
extern void uart_echo(uart_t *);
	
void dts_hal_default_handler(void *p, ...)
{
}

uint8_t buf[512];

int main(void)
{
	gpio_init(&led);
	
	uart_init(&uart1);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT, 4);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_BUFFER, buf, 256);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_MODE, STM32F10X_HAL_UART_RX_INTR_MODE);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_BAUDRATE, 115200);
	
	interrupt_global_enable();
	
	uart_echo(&uart1);
	
	return 0;
}
