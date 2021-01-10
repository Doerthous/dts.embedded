#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/uart.h>
#include <w25qxx.h>

void dts_hal_default_handler(void *p, ...)
{
}

extern gpio_t led;
extern spi_t spi2;
extern spi_t sw_spi2;
extern w25qxx_t w25q128;
extern uart_t uart1;
extern int fs_test(void);
uint8_t buf[256];
int main(void)
{
	gpio_init(&led);

	uart_init(&uart1);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT, 4);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_BUFFER, buf, 256);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_RX_MODE, STM32F10X_HAL_UART_RX_INTR_MODE);
	uart_config(&uart1, STM32F10X_HAL_UART_CMD_SET_BAUDRATE, 115200);
	
	//spi_init(&sw_spi2);
    //w25q128.spi = &sw_spi2,
	
	spi_init(&spi2);
    w25q128.spi = &spi2,

	fs_test();
	
	return 0;
}
