/*
   The MIT License (MIT)

   Copyright (c) 2021 Doerthous

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

#ifndef STM32F10X_HAL_SPI_H_
#define STM32F10X_HAL_SPI_H_

#include <stm32f10x.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
	SPI_TypeDef *spi;
} stm32f10x_hal_spi_t;

void stm32f10x_hal_spi_init(stm32f10x_hal_spi_t *spi);
int stm32f10x_hal_spi_config(stm32f10x_hal_spi_t *spi, int param, uint8_t *ret, ...);
size_t stm32f10x_hal_spi_write(stm32f10x_hal_spi_t *spi, uint8_t *data, size_t size);
size_t stm32f10x_hal_spi_read(stm32f10x_hal_spi_t *spi, uint8_t *buff, size_t size);
size_t stm32f10x_hal_spi_transfer(stm32f10x_hal_spi_t *spi, uint8_t *buff, size_t size);

#endif // STM32F10X_HAL_SPI_H_
