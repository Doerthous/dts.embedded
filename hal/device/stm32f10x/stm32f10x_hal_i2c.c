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

#ifndef DTS_STM32F10X_HAL_I2C
# include "stm32f10x_hal_i2c.h"
# define i2c_t stm32f10x_hal_i2c_t
# define i2c_init stm32f10x_hal_i2c_init
# define i2c_config stm32f10x_hal_i2c_config
# define i2c_write stm32f10x_hal_i2c_write
# define i2c_read stm32f10x_hal_i2c_read
# define i2c_busy stm32f10x_hal_i2c_busy
# define i2c_start stm32f10x_hal_i2c_start
# define i2c_7b_addr stm32f10x_hal_i2c_7b_addr
# define i2c_10b_addr stm32f10x_hal_i2c_10b_addr
# define i2c_stop stm32f10x_hal_i2c_stop
#else
# include <dts/embedded/hal/i2c.h>
#endif // DTS_STM32F10X_HAL_I2C

#include <dts/embedded/lib/delay.h>
#include <stdarg.h>


void i2c_init(i2c_t *i2c)
{
#if defined(USE_STDPERIPH_DRIVER)
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef  I2c_InitStructure;
    
    
    if (i2c->i2c == I2C1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,  ENABLE);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;            
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

		
		I2C_Cmd(i2c->i2c, DISABLE);
        I2c_InitStructure.I2C_ClockSpeed = i2c->frequency;
        I2c_InitStructure.I2C_Mode =  I2C_Mode_I2C;
        I2c_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
        I2c_InitStructure.I2C_OwnAddress1 = 0xA0;                 
        I2c_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2c_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_Init(i2c->i2c, &I2c_InitStructure);
        I2C_Cmd(i2c->i2c, ENABLE);

        return;
    }
#else
    #error "i2c stm32f10x not support register operation yet!"
#endif
}

int i2c_config(i2c_t *i2c, int param, uint8_t *ret, ...)
{
    int status = 0;
	va_list args;
	//I2C_InitTypeDef  I2c_InitStructure;
	
	
	va_start(args, ret);	
	switch (param) {
		/*case I2C_PARAM_TIMEOUT:
			i2c->timeout = va_arg(args, uint32_t);
            status = 1;
			break;
        case I2C_PARAM_FREQ:
            I2C_Cmd(i2c->i2c, DISABLE);
			i2c->frequency = va_arg(args, uint32_t);
            I2c_InitStructure.I2C_ClockSpeed = i2c->frequency;
            I2c_InitStructure.I2C_Mode =  I2C_Mode_I2C;
            I2c_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
            I2c_InitStructure.I2C_OwnAddress1 = 0xA0;                 
            I2c_InitStructure.I2C_Ack = I2C_Ack_Enable;
            I2c_InitStructure.I2C_AcknowledgedAddress = 
				I2C_AcknowledgedAddress_7bit;
            I2C_Init(i2c->i2c, &I2c_InitStructure);
            I2C_Cmd(i2c->i2c, ENABLE);
            status = 1;
            break;
		*/
	}
	va_end(args);

    return status;
}

int i2c_busy(i2c_t *i2c)
{
    return I2C_GetFlagStatus(i2c->i2c, I2C_FLAG_BUSY);
}

void i2c_start(i2c_t *i2c)
{
    I2C_GenerateSTART(i2c->i2c, ENABLE);
    while (!I2C_CheckEvent(i2c->i2c, I2C_EVENT_MASTER_MODE_SELECT)); 
}

int i2c_7b_addr(i2c_t *i2c, uint8_t addr, int rw)
{
	int ack = i2c->timeout;
	
	
	if (rw) {
		I2C_Send7bitAddress(i2c->i2c, addr, I2C_Direction_Receiver);
		while (!I2C_CheckEvent(i2c->i2c, 
			I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && ack--) {
			delay_ms(1);
		}
	}
	else {
		I2C_Send7bitAddress(i2c->i2c, addr, I2C_Direction_Transmitter);
		while (!I2C_CheckEvent(i2c->i2c, 
			I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && ack--) {
			delay_ms(1);
		}
	}

    return ack;
}

int i2c_10b_addr(i2c_t *i2c, uint16_t addr, int rw)
{
    //hal_assert(0); // todo
    return 0;
}

int i2c_write(i2c_t *i2c, uint8_t data)
{
	int ack = i2c->timeout;
	
	
    I2C_SendData(i2c->i2c, data);
	
    while (!I2C_CheckEvent(i2c->i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && ack--) {
		delay_ms(1);
	}
	
    return ack;
}

uint8_t i2c_read(i2c_t *i2c, int ack)
{
    I2C_AcknowledgeConfig(I2C2, ack ? ENABLE : DISABLE);
    while (I2C_CheckEvent(i2c->i2c, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
    return I2C_ReceiveData(i2c->i2c); 
}

void i2c_stop(i2c_t *i2c)
{
    I2C_GenerateSTOP(i2c->i2c, ENABLE);
	
	// Make sure stop transfer complete, otherwise we will get a bus error.
	delay_ms(1); 
	
	// Every time after an i2c communication complete, re-init the i2c.
	// This is a trap of stm32 i2c, see 26.3.3 Master receiver. Maybe 
	// the SCL is scretched low by the i2c, so we just re-init i2c here.
	i2c_init(i2c);
}
