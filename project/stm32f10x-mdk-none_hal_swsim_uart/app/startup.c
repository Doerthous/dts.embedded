#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/timer.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/rtc.h>
#include <dts/embedded/hal/interrupt.h>

void dts_hal_default_handler(void *_, ...) {}

uint8_t buf[512];
	
extern timer_t tim1, tim2, tim3;
extern uart_t uart1;
extern rtc_t rtc1;
void uart_rtc(uart_t *uart, rtc_t *rtc);
void uart_echo(uart_t *uart);
void sw_uart1_low_level_init(uart_t *uart)
{
	static gpio_t a = {
		.port = GPIOA,
		.pin = GPIO_Pin_9,
		.speed = GPIO_Speed_2MHz,
		.mode = GPIO_Mode_Out_PP,
	};

	static gpio_t b = {
		.port = GPIOA,
		.pin = GPIO_Pin_10,
		.speed = GPIO_Speed_2MHz,
		.mode = GPIO_Mode_IPU,
	};
	uart->tx.pin = &a;
	uart->tx.tmr = &tim1;
	
	uart->rx.pin = &b;
	uart->rx.tmr = &tim2;
	
	gpio_init(&a);
	gpio_init(&b);

	timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	timer_config(&tim2, STM32F10X_HAL_TIMER_CMD_CLK_ON);
}
int main(void)
{	
	uart1.low_level_init = sw_uart1_low_level_init;
	uart_init(&uart1);
	uart_config(&uart1, SWSIM_HAL_UART_CMD_SET_RX_TIMEOUT, 4);
	uart_config(&uart1, SWSIM_HAL_UART_CMD_SET_RX_BUFFER, buf, 256);
	uart_config(&uart1, SWSIM_HAL_UART_CMD_SET_RX_MODE, SWSIM_HAL_UART_RX_INTR_MODE);
	uart_config(&uart1, SWSIM_HAL_UART_CMD_SET_BAUDRATE, 4800);
	
	timer_config(&tim3, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	
	interrupt_global_enable();
	
	rtc1.tmr = &tim3;
	rtc_init(&rtc1);
	
	interrupt_global_enable();
	
	//uart_rtc(&uart1, &rtc1);
	uart_echo(&uart1);
	
	return 0;
}
