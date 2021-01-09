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

#ifndef AT24CXX_H_
#define AT24CXX_H_

#include <dts_hal_i2c.h>
#include <stddef.h>
#include <stdint.h>

/*
    at24c01/02/04/08/16
    capacity: 128x8(1K)/256x8(2K)/512x8(4K)/1024x8(8K)/2048x8(16K)
*/

typedef struct 
{
    dts_hal_i2c_t *i2c;
    size_t page_size; // Byte
    size_t address;
    size_t capacity; // Byte
} at24cxx_t;

size_t at24cxx_read(at24cxx_t *at24cxx,
    uint32_t addr, uint8_t *buff, size_t size);
size_t at24cxx_write(at24cxx_t *at24cxx,
    uint32_t addr, uint8_t *data, size_t size);

#endif // AT24CXX_H_
