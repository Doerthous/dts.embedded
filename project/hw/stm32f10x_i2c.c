#include <dts/embedded/hal/i2c.h>

i2c_t i2c1 =
{
    .i2c = I2C1, 
    .timeout = 10,
	.frequency = 100000,
};
