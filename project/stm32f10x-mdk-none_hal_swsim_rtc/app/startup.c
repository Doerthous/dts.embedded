#include <dts/embedded/hal/timer.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/rtc.h>
#include <dts/embedded/hal/interrupt.h>

void dts_hal_default_handler(void *_, ...) {}

uint8_t buf[512];
	
extern timer_t tim1;
extern uart_t uart1;
extern rtc_t rtc1;
void uart_rtc(uart_t *uart, rtc_t *rtc);
int main(void)
{	
	uart_init(&uart1);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT, 4);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_BUFFER, buf, 256);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_MODE, STM32F10X_HAL_UART_RX_INTR_MODE);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_BAUDRATE, 115200);
	
	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	
	interrupt_global_enable();
	
	rtc1.tmr = &tim1;
	rtc_init(&rtc1);
	
	interrupt_global_enable();
	
	uart_rtc(&uart1, &rtc1);
	
	while (1);
	
	return 0;
}
