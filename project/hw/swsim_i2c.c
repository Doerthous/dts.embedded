#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/i2c.h>

static void set_pin_mode(i2c_t *i2c, gpio_t *gpio, int output)
{
    if (output) {
        gpio_config(gpio, STM32F10X_HAL_GPIO_CMD_SET_MODE, GPIO_Mode_Out_PP);        
    }
    else {
        gpio_config(gpio, STM32F10X_HAL_GPIO_CMD_SET_MODE, GPIO_Mode_IN_FLOATING);
    }
}

i2c_t i2c1 = 
{ 
    .scl = { .port = GPIOB, .pin = GPIO_Pin_6, }, 
    .sda = { .port = GPIOB, .pin = GPIO_Pin_7, },
    .period = 5,
    .set_pin_mode = set_pin_mode,
};
