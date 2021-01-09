#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/i2c.h>
#include <swsim_hal_i2c.h>
#include <stm32f10x_hal_i2c.h>


static void set_pin_mode(swsim_hal_i2c_t *i2c, gpio_t *gpio, int output)
{
    if (output) {
        gpio_config(gpio, STM32F10X_HAL_GPIO_CMD_SET_MODE, GPIO_Mode_Out_PP);        
    }
    else {
        gpio_config(gpio, STM32F10X_HAL_GPIO_CMD_SET_MODE, GPIO_Mode_IN_FLOATING);
    }
}
static DTS_HAL_I2C_METHOD_DEFINE(swsim, \
    swsim_hal_i2c_init, \
    swsim_hal_i2c_config, \
    swsim_hal_i2c_write, \
    swsim_hal_i2c_read, \
    swsim_hal_i2c_start, \
    swsim_hal_i2c_stop, \
    swsim_hal_i2c_busy, \
    swsim_hal_i2c_7b_addr, \
    swsim_hal_i2c_10b_addr \
)
static swsim_hal_i2c_t swsim_i2c1 = 
{
    .scl = { .port = GPIOB, .pin = GPIO_Pin_6, }, 
    .sda = { .port = GPIOB, .pin = GPIO_Pin_7, },
    .period = 5,
    .set_pin_mode = set_pin_mode,
};
i2c_t sw_i2c1 =
{
    .obj = (void *)&swsim_i2c1,
    .method = &DTS_HAL_I2C_METHOD(swsim),
};


static DTS_HAL_I2C_METHOD_DEFINE(stm32f10x, \
    stm32f10x_hal_i2c_init, \
    stm32f10x_hal_i2c_config, \
    stm32f10x_hal_i2c_write, \
    stm32f10x_hal_i2c_read, \
    stm32f10x_hal_i2c_start, \
    stm32f10x_hal_i2c_stop, \
    stm32f10x_hal_i2c_busy, \
    stm32f10x_hal_i2c_7b_addr, \
    stm32f10x_hal_i2c_10b_addr \
)
static stm32f10x_hal_i2c_t stm32f10x_i2c1 = 
{
    .i2c = I2C1, 
    .timeout = 10,
    .frequency = 100000,
};
i2c_t i2c1 =
{
    .obj = (void *)&stm32f10x_i2c1,
    .method = &DTS_HAL_I2C_METHOD(stm32f10x),
};
