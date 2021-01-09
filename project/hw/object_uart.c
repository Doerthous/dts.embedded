#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/timer.h>
#include <swsim_hal_uart.h>
#include <stm32f10x_hal_uart.h>

static DTS_HAL_UART_METHOD_DEFINE(stm32f10x, \
	stm32f10x_hal_uart_init, \
	stm32f10x_hal_uart_config, \
	stm32f10x_hal_uart_write, \
	stm32f10x_hal_uart_read \
);

static stm32f10x_hal_uart_t stm32f10x_uart1 = 
{
    .st_uart = USART1,
};
uart_t uart1 =
{
    .obj = (void *)&stm32f10x_uart1,
	.method = &DTS_HAL_UART_METHOD(stm32f10x),
};


static DTS_HAL_UART_METHOD_DEFINE(swsim, \
	swsim_hal_uart_init, \
	swsim_hal_uart_config, \
	swsim_hal_uart_write, \
	swsim_hal_uart_read \
);
void sw_uart1_low_level_init(swsim_hal_uart_t *uart);
static swsim_hal_uart_t swsim_uart1 = 
{
	.low_level_init = sw_uart1_low_level_init,
};
uart_t sw_uart1 =
{
    .obj = (void *)&swsim_uart1,
	.method = &DTS_HAL_UART_METHOD(swsim),
};
