/*
   The MIT License (MIT)

   Copyright (c) 2020 Doerthous

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   Authour: Doerthous <doerthous@gmail.com>
*/

#ifndef DTS_SWSIM_HAL_SPI
# include "swsim_hal_spi.h"
# define spi_t swsim_hal_spi_t
# define spi_init swsim_hal_spi_init
# define spi_config swsim_hal_spi_config
# define spi_write swsim_hal_spi_write
# define spi_read swsim_hal_spi_read
# define spi_transfer swsim_hal_spi_transfer
#else
# include <dts/embedded/hal/spi.h>
#endif // DTS_SWSIM_HAL_SPI

#include <dts/embedded/lib/delay.h>

void spi_init(spi_t *spi)
{
    dts_hal_gpio_init(&spi->i);
    dts_hal_gpio_init(&spi->o);
    dts_hal_gpio_init(&spi->c);

    dts_hal_gpio_clear(&spi->c);
}

int spi_config(spi_t *spi, int param, uint8_t *ret, ...)
{
	return -1;
}

size_t spi_write(spi_t *spi, uint8_t *data, size_t size)
{
    for (int i = 0; i < size; ++i) {
        uint8_t byte = data[i];
        for (int j = 0; j < 8; ++j) {
            if (byte & 0x80) {
                dts_hal_gpio_set(&spi->o);
            }
            else {
                dts_hal_gpio_clear(&spi->o);
            }
            dts_hal_gpio_toggle(&spi->c);
            for(int k=1000;--k;);
            byte <<= 1;
            dts_hal_gpio_toggle(&spi->c);
            for(int k=1000;--k;);
        }
    }
    return size;
}

size_t spi_read(spi_t *spi, uint8_t *buff, size_t size)
{
    for (int i = 0; i < size; ++i) {
        uint8_t byte = 0;
        for (int j = 0; j < 8; ++j) {
            byte <<= 1;
            dts_hal_gpio_toggle(&spi->c);
            for(int k=1000;--k;);
            if (dts_hal_gpio_read(&spi->i)) {
                byte |= 0x01;
            }
            dts_hal_gpio_toggle(&spi->c);
            for(int k=1000;--k;);
        }
        buff[i] = byte;
    }
	return size;
}

size_t spi_transfer(spi_t *spi, uint8_t *buff, size_t size)
{
    return 0;
}
