#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <enc28j60.h>

enc28j60_t enc28j60 = 
{ 
	.cs.port = GPIOG,
	.cs.pin = GPIO_Pin_7,
	.cs.mode = GPIO_Mode_Out_PP,
	.cs.speed = GPIO_Speed_50MHz,
	.mac_addr = { 0x68, 0x19, 0x78, 0x11, 0x12, 0x34 },
};

gpio_t enc28j60_int = {
	.port = GPIOG,
	.pin = GPIO_Pin_6,
	.mode = GPIO_Mode_IN_FLOATING,
	.speed = GPIO_Speed_50MHz,
};
