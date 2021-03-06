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

#include <dts/embedded/hal/i2c.h>
#include <dts/embedded/lib/delay.h>
#include "at24cxx.h"

/*
    at24c01/02/04/08/16
    capacity: 128x8(1K)/256x8(2K)/512x8(4K)/1024x8(8K)/2048x8(16K)
*/

size_t at24cxx_read(at24cxx_t *at24cxx,
    uint32_t addr, uint8_t *buff, size_t size)
{
    int i = 0;
    
    
//	while (i2c_busy(at24cxx->i2c));
	
    if (size > 0) {
        i2c_start(at24cxx->i2c);
        i2c_7b_addr(at24cxx->i2c, at24cxx->address, 0);
        i2c_write(at24cxx->i2c, addr);
        i2c_start(at24cxx->i2c);
        i2c_7b_addr(at24cxx->i2c, at24cxx->address, 1);
        
        for (i = 0; i < size-1; ++i) {
            buff[i] = i2c_read(at24cxx->i2c, 1);
        }
        buff[i] = i2c_read(at24cxx->i2c, 0);
        
        i2c_stop(at24cxx->i2c); // stop condition
    }
    
    return size;
}

size_t at24cxx_write(at24cxx_t *at24cxx,
    uint32_t addr, uint8_t *data, size_t size)
{
    int poffset = 0;
    int twc = 0;
    int wc = 0;
    
    
//	while (i2c_busy(at24cxx->i2c));
	
    while (size > 0) {
        poffset = addr & (at24cxx->page_size-1);
        wc = size < at24cxx->page_size-poffset ? 
            size : at24cxx->page_size-poffset;

        i2c_start(at24cxx->i2c);
        i2c_7b_addr(at24cxx->i2c, at24cxx->address, 0);
        i2c_write(at24cxx->i2c, addr);
        
        twc += wc;
        addr += wc;
        size -= wc;
        
        while (wc-- > 0) {
            i2c_write(at24cxx->i2c, *data++);
        }
        
        i2c_stop(at24cxx->i2c);
        
		// at24cxx inner delay, see datasheet.
        delay_ms(5);
    }
    
    return twc;
}
