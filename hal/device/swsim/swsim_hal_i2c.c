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

#ifndef DTS_SWSIM_HAL_I2C
# include "swsim_hal_i2c.h"
# define i2c_t swsim_hal_i2c_t
# define i2c_init swsim_hal_i2c_init
# define i2c_config swsim_hal_i2c_config
# define i2c_write swsim_hal_i2c_write
# define i2c_read swsim_hal_i2c_read
# define i2c_busy swsim_hal_i2c_busy
# define i2c_start swsim_hal_i2c_start
# define i2c_7b_addr swsim_hal_i2c_7b_addr
# define i2c_10b_addr swsim_hal_i2c_10b_addr
# define i2c_stop swsim_hal_i2c_stop
#else
# include <dts/embedded/hal/i2c.h>
#endif // DTS_SWSIM_HAL_I2C

#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/lib/delay.h>
#include <stdarg.h>

static int i2c_wait_ack(i2c_t *i2c) {
	int ack = 1;
	
	
    gpio_clear(&i2c->scl);
    delay_us(i2c->period);
    gpio_set(&i2c->sda);
    i2c->set_pin_mode(i2c, &i2c->sda, 0);
    gpio_set(&i2c->scl);
    delay_us(i2c->period);
    ack = !gpio_read(&i2c->sda);
	gpio_clear(&i2c->scl);
    i2c->set_pin_mode(i2c, &i2c->sda, 1);
    delay_us(i2c->period);

    return ack;
}

static void i2c_send_ack(i2c_t *i2c, int ack) {
    if (ack) {
        gpio_clear(&i2c->scl);
        delay_us(i2c->period);
        gpio_clear(&i2c->sda);
        gpio_set(&i2c->scl);
        delay_us(i2c->period);
        gpio_clear(&i2c->scl);
        delay_us(i2c->period);
        gpio_set(&i2c->sda);
    }
    else {
        gpio_clear(&i2c->scl);
        delay_us(i2c->period);
        gpio_set(&i2c->sda);
        gpio_set(&i2c->scl);
        delay_us(i2c->period);
        gpio_clear(&i2c->scl);
        delay_us(i2c->period);
    }
}

void i2c_init(i2c_t *i2c)
{
    gpio_init(&i2c->scl);
    gpio_init(&i2c->sda);
    i2c->set_pin_mode(i2c, &i2c->scl, 1);
    i2c->set_pin_mode(i2c, &i2c->sda, 1);
    gpio_set(&i2c->scl);
    gpio_set(&i2c->sda);
}

int i2c_config(i2c_t *i2c, int param, uint8_t *ret, ...)
{
    int status = 0;
	va_list args;
	
	
	va_start(args, ret);	
	switch (param) {
		case I2C_PARAM_FREQ:
			i2c->period = 1000000 / va_arg(args, uint32_t); // us
            status = 1;
			break;
	}
	va_end(args);

    return status;
}

// both scl and sda are high level when i2c bus idle.
int i2c_busy(i2c_t *i2c)
{
    int idle = 1;
    

    i2c->set_pin_mode(i2c, &i2c->sda, 0);
    i2c->set_pin_mode(i2c, &i2c->scl, 0);

    idle &= (gpio_read(&i2c->sda) & gpio_read(&i2c->scl));
    delay_us(2);
    idle &= (gpio_read(&i2c->sda) & gpio_read(&i2c->scl));
    delay_us(2);
    idle &= (gpio_read(&i2c->sda) & gpio_read(&i2c->scl));

    i2c->set_pin_mode(i2c, &i2c->sda, 1);
    i2c->set_pin_mode(i2c, &i2c->scl, 1);

    return !idle;
}

void i2c_start(i2c_t *i2c)
{
    gpio_clear(&i2c->scl);
    delay_us(i2c->period);
    gpio_set(&i2c->sda);
    gpio_set(&i2c->scl);
    delay_us(i2c->period);
    gpio_clear(&i2c->sda);
}

int i2c_7b_addr(i2c_t *i2c, uint8_t addr, int rw)
{
    return i2c_write(i2c, addr | rw);
}

int i2c_10b_addr(i2c_t *i2c, uint16_t addr, int rw)
{
    return 0;
}

int i2c_write(i2c_t *i2c, uint8_t data)
{
    int i = 0;
    gpio_clear(&i2c->scl); 
    delay_us(i2c->period);
    for (i = 8; i; --i) {
        if (data & 0x80) { 
            gpio_set(&i2c->sda); 
        } 
        else { 
            gpio_clear(&i2c->sda); 
        } 
        gpio_set(&i2c->scl); 
        delay_us(i2c->period); /* wait for slave receive, cannot be omitted */
        data <<= 1; 
        gpio_clear(&i2c->scl); 
        delay_us(i2c->period);
    }

    return i2c_wait_ack(i2c);
}

uint8_t i2c_read(i2c_t *i2c, int ack)
{
    int i = 0;
    int data = 0;
    i2c->set_pin_mode(i2c, &i2c->sda, 0);
    gpio_clear(&i2c->scl);
    delay_us(i2c->period);
    data = 0;
    for (i = 8; i; --i) {
        data <<= 1;
        data |= gpio_read(&i2c->sda);
        gpio_set(&i2c->scl);
        delay_us(i2c->period);
        gpio_clear(&i2c->scl);
        delay_us(i2c->period);
    }
    i2c->set_pin_mode(i2c, &i2c->sda, 1);
    i2c_send_ack(i2c, ack);
    return data;
}

void i2c_stop(i2c_t *i2c)
{
    gpio_clear(&i2c->scl);
    delay_us(i2c->period);
    gpio_clear(&i2c->sda);
    gpio_set(&i2c->scl);
    delay_us(i2c->period);
    gpio_set(&i2c->sda);
}
