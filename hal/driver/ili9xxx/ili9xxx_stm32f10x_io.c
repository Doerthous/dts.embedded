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

#include <ili9xxx.h>
#include <dts/embedded/hal/gpio.h>


#define CS 16
#define RS 17
#define WR 18
#define RD 19
#define BL 20
static gpio_t line[] = 
{
    // LCD D0~D15
    { 
        .port = GPIOD, .pin = GPIO_Pin_14, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOD, .pin = GPIO_Pin_15, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOD, .pin = GPIO_Pin_0, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOD, .pin = GPIO_Pin_1, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_7, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_8, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_9, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_10, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_11, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_12, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_13, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOE, .pin = GPIO_Pin_14, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },   
    { 
        .port = GPIOE, .pin = GPIO_Pin_15, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOD, .pin = GPIO_Pin_8, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOD, .pin = GPIO_Pin_9, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },   
    { 
        .port = GPIOD, .pin = GPIO_Pin_10, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },


    // LCD cs,rs,wr,rd
    { 
        .port = GPIOG, .pin = GPIO_Pin_12, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOG, .pin = GPIO_Pin_0, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
    { 
        .port = GPIOD, .pin = GPIO_Pin_5, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },   
    { 
        .port = GPIOD, .pin = GPIO_Pin_4, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },


    // LCD backlight
    { 
        .port = GPIOB, .pin = GPIO_Pin_0, 
        .mode = GPIO_Mode_Out_PP, .speed = GPIO_Speed_2MHz,
    },
};
static uint8_t wr;


#define ili9xxx_cs_set()    gpio_set(&line[CS])
#define ili9xxx_cs_clr()    gpio_reset(&line[CS])
#define ili9xxx_rs_set()    gpio_set(&line[RS])
#define ili9xxx_rs_clr()    gpio_reset(&line[RS])
#define ili9xxx_wr_set()    gpio_set(&line[WR])
#define ili9xxx_wr_clr()    gpio_reset(&line[WR])
#define ili9xxx_rd_set()    gpio_set(&line[RD])
#define ili9xxx_rd_clr()    gpio_reset(&line[RD])



static void ili9xxx_gpio_init() 
{
    int i = 0;


    for (i = 0; i < 21; ++i) {
        gpio_init(&line[i]);
    }

    ili9xxx_cs_set();
    ili9xxx_rs_set();
    ili9xxx_wr_set();
    ili9xxx_rd_set();

    gpio_write(&line[BL], 1);
}


static void ili9xxx_gpio_data_out(int data) 
{
    int i;

    if (!wr) {
        wr = 1;
        for (i = 0; i < 16; ++i) {
            gpio_config(&line[i], STM32F10X_HAL_GPIO_CMD_SET_MODE, GPIO_Mode_Out_PP);
        }
    }

    GPIOD->ODR &= ~0xc703;
    GPIOE->ODR &= ~0xff80;
    GPIOD->ODR |= (((data & 0xe000) >> 5) | 
                   ((data & 0x000c) >> 2) | 
                   ((data & 0x0003) << 14));
    GPIOE->ODR |= ((data & 0x1ff0) << 3);  
}
static int  ili9xxx_gpio_data_in() 
{ 
    int i;
    int pd, pe;

    if (wr) {
        wr = 0;
        for (i = 0; i < 16; ++i) {
            gpio_config(&line[i], STM32F10X_HAL_GPIO_CMD_SET_MODE, GPIO_Mode_IN_FLOATING);
        }
    }

    pd = GPIOD->IDR;
    pe = GPIOE->IDR;
    return ((pe & 0xff80) >> 3) | 
            ((pd & 0x3) << 2) | 
            ((pd & 0xc000) >> 14) | 
            ((pd & 0x700) << 5);
}


static void write_address(ili9xxx_t *ili9xxx, uint32_t address)
{
    ili9xxx_cs_clr();
    ili9xxx_rs_clr();
    ili9xxx_wr_clr();
    ili9xxx_gpio_data_out(address); 
    ili9xxx_wr_set();
    ili9xxx_rs_set();
    ili9xxx_cs_set();
}

static void write_data(ili9xxx_t *ili9xxx, uint32_t data) 
{
    ili9xxx_cs_clr();
    ili9xxx_rs_set();
    ili9xxx_wr_clr();
    ili9xxx_gpio_data_out(data); 
    ili9xxx_wr_set();
    ili9xxx_cs_set();
}

static uint32_t read_data(ili9xxx_t *ili9xxx) 
{
    int data = 0;
    ili9xxx_cs_clr();
    ili9xxx_rs_set();
    ili9xxx_rd_clr();
    data = ili9xxx_gpio_data_in(); 
    ili9xxx_rd_set();
    ili9xxx_cs_set();
    return data;
}

void ili9xxx_io_init(ili9xxx_t *ili9xxx)
{
    ili9xxx_gpio_init();
    ili9xxx->write_address = write_address;
    ili9xxx->write_data = write_data;
    ili9xxx->read_data = read_data;
}
