#include <dts/embedded/hal/spi.h>
#include <w25qxx.h>

w25qxx_t w25q128 = 
{ 
	.cs.port = GPIOB,
	.cs.pin = GPIO_Pin_12,
	.cs.mode = GPIO_Mode_Out_PP,
};
