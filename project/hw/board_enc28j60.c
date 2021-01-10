#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <enc28j60.h>

enc28j60_t enc28j60 = 
{ 
	.cs.port = GPIOG,
	.cs.pin = GPIO_Pin_7,
	.cs.mode = GPIO_Mode_Out_PP,
	.cs.speed = GPIO_Speed_50MHz,
};

gpio_t enc28j60_int = {
	.port = GPIOG,
	.pin = GPIO_Pin_6,
	.mode = GPIO_Mode_IN_FLOATING,
	.speed = GPIO_Speed_50MHz,
};
