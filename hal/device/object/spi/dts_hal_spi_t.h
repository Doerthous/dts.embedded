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

#ifndef DTS_HAL_SPI_T_H_
#define DTS_HAL_SPI_T_H_

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    void (*init)(void *spi);
    int (*config)(void *spi, int param, uint8_t *ret, ...);
    size_t (*write)(void *spi, uint8_t *data, size_t size);
    size_t (*read)(void *spi, uint8_t *buff, size_t size);
    size_t (*transfer)(void *spi, uint8_t *buff, size_t size);
} dts_hal_spi_method_t;

typedef struct {
    void *obj;
    dts_hal_spi_method_t *method;
} dts_hal_spi_t;


#define DTS_HAL_SPI_METHOD(type) dts_hal_##type##_spi_method
#define DTS_HAL_SPI_METHOD_DEFINE(type, _init, _config, _write, _read, _transfer) \
dts_hal_spi_method_t DTS_HAL_SPI_METHOD(type) = \
{ \
    .init = (void (*)(void *))_init, \
    .config = (int (*)(void *,int,uint8_t *,...))_config, \
    .write = (size_t (*)(void *,uint8_t *,size_t))_write, \
    .read = (size_t (*)(void *,uint8_t *,size_t))_read, \
    .transfer = (size_t (*)(void *,uint8_t *,size_t))_transfer, \
};
#define DTS_HAL_SPI_METHOD_DECLARE(type) \
    extern dts_hal_spi_method_t DTS_HAL_SPI_METHOD(type);

#define DTS_HAL_SPI_WRAPPER

#endif // DTS_HAL_SPI_T_H_
