#include <dts/embedded/hal/spi.h>
#include <stm32f10x_hal_spi.h>
#include <swsim_hal_spi.h>

static DTS_HAL_SPI_METHOD_DEFINE(stm32f10x, \
	stm32f10x_hal_spi_init, \
	stm32f10x_hal_spi_config, \
	stm32f10x_hal_spi_write, \
	stm32f10x_hal_spi_read, \
	stm32f10x_hal_spi_transfer \
)
static stm32f10x_hal_spi_t stm32f10x_spi2 = 
{
    .spi = SPI2,
};
spi_t spi2 =
{
    .obj = (void *)&stm32f10x_spi2,
	.method = &DTS_HAL_SPI_METHOD(stm32f10x),
};

static DTS_HAL_SPI_METHOD_DEFINE(swsim, \
	swsim_hal_spi_init, \
	swsim_hal_spi_config, \
	swsim_hal_spi_write, \
	swsim_hal_spi_read, \
	swsim_hal_spi_transfer \
)
static stm32f10x_hal_spi_t swsim_spi2 = 
{
    .spi = SPI2,
};
spi_t sw_spi2 =
{
    .obj = (void *)&swsim_spi2,
	.method = &DTS_HAL_SPI_METHOD(swsim),
};
