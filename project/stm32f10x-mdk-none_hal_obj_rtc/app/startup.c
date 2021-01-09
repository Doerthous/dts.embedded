#include <dts/embedded/hal/timer.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/hal/rtc.h>
#include <swsim_hal_rtc.h>

extern timer_t tim1;
extern uart_t uart1;
extern rtc_t rtc, sw_rtc;
extern void uart_rtc(uart_t *, rtc_t *);
	
void dts_hal_default_handler(void *p, ...)
{
}

uint8_t buf[512];

int main(void)
{
	int using_sw_rtc = 0;
	
	uart_init(&uart1);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT, 4);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_BUFFER, buf, 256);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_MODE, STM32F10X_HAL_UART_RX_INTR_MODE);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_BAUDRATE, 115200);
	
	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	
	rtc_init(&rtc);
	((swsim_hal_rtc_t *)sw_rtc.obj)->tmr = &tim1;
	rtc_init(&sw_rtc);
	
	interrupt_global_enable();
	
	if (using_sw_rtc) {
		uart_rtc(&uart1, &sw_rtc);
	}
	else {
		uart_rtc(&uart1, &rtc);
	}
	
	return 0;
}
