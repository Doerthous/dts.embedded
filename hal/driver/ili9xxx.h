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


#ifndef ILI9XXX_H_
#define ILI9XXX_H_

#include <stdint.h>

typedef struct ili9xxx_config
{
    uint8_t vertical_scan_order: 1;
    uint8_t horizonal_scan_order: 1;
    uint8_t bgr: 1; // RGB or BGR
    uint8_t row_addr_order: 1;
    uint8_t col_addr_order: 1;
    uint8_t row_col_exchange: 1;
} ili9xxx_config_t;

typedef struct ili9xxx
{
    uint16_t id;
    uint16_t width;
    uint16_t height;

    // machine-dependent, internal-use.
    void (*md_init)(struct ili9xxx * ili9xxx);
    void (*write_address)(struct ili9xxx *ili9xxx, uint32_t address);
    void (*write_data)(struct ili9xxx *ili9xxx, uint32_t data);
    uint32_t (*read_data)(struct ili9xxx *ili9xxx);
} ili9xxx_t;


int ili9xxx_init(ili9xxx_t *ili9xxx);
void ili9xxx_set_pixel(ili9xxx_t *, int x, int y, int color);
int  ili9xxx_get_pixel(ili9xxx_t *, int x, int y);
void ili9xxx_fill(ili9xxx_t *, int xs, int ys, int xe, int ye, int color);

int ili9xxx_read_config(ili9xxx_t *ili9xxx, ili9xxx_config_t *config);
int ili9xxx_scan_order(ili9xxx_t *ili9xxx, int vso, int hso);
int ili9xxx_addr_order(ili9xxx_t *ili9xxx, int rao, int cao);

#endif // ILI9XXX_H_
