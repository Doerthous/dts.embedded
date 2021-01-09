/*
   The MIT License (MIT)

   Copyright (c) 2019 Doerthous

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

#ifndef W25QXX_H_
#define W25QXX_H_

#include <dts_hal_gpio.h>
#include <dts_hal_spi.h>
#include <stddef.h>

typedef struct
{
    dts_hal_gpio_t cs;
    dts_hal_spi_t *spi;

    size_t capacity; // Byte
    size_t sector_size; // Byte
} w25qxx_t;


int w25qxx_init(w25qxx_t *w25qxx);
// no erase before write
size_t w25qxx_write(w25qxx_t *w25qxx, 
    uint32_t addr, uint8_t *data, size_t size);
size_t w25qxx_read(w25qxx_t *w25qxx, 
    uint32_t addr, uint8_t *buff, size_t size);
int w25qxx_erase_sector(w25qxx_t *w25qxx, uint32_t addr);
int w25qxx_erase_chip(w25qxx_t *w25qxx);

#endif // W25QXX_H_
