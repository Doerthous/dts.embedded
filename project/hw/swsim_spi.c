#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>

spi_t spi2 =
{
    .i.port= GPIOB,
    .i.pin = GPIO_Pin_14,
    .i.mode = GPIO_Mode_IN_FLOATING,
    .o.port= GPIOB,
    .o.pin = GPIO_Pin_15,
    .o.mode = GPIO_Mode_Out_PP,
    .c.port= GPIOB,
    .c.pin = GPIO_Pin_13,
    .c.mode = GPIO_Mode_Out_PP,
};
