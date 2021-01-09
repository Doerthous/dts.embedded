#include <dts/embedded/hal/gpio.h>

gpio_t led = 
{
    .port= GPIOB,
    .pin = GPIO_Pin_5,
    .mode = GPIO_Mode_Out_PP,
};
