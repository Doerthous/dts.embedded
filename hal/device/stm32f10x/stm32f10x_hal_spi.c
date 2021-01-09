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

#ifndef DTS_STM32F10X_HAL_SPI
# include "stm32f10x_hal_spi.h"
# define spi_t stm32f10x_hal_spi_t
# define spi_init stm32f10x_hal_spi_init
# define spi_config stm32f10x_hal_spi_config
# define spi_write stm32f10x_hal_spi_write
# define spi_read stm32f10x_hal_spi_read
# define spi_transfer stm32f10x_hal_spi_transfer
#else
# include <dts/embedded/hal/spi.h>
#endif // DTS_STM32F10X_HAL_SPI

void spi_init(spi_t *spi)
{
#if defined(USE_STDPERIPH_DRIVER)
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    
    if (spi->spi == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);
        
        // todo ...
        return;
    }
    if (spi->spi == SPI2) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStructure.SPI_CRCPolynomial = 7;
        SPI_Init(SPI2, &SPI_InitStructure);
     
        SPI_Cmd(SPI2, ENABLE);
        return;
    }
    if (spi->spi == SPI3) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE);
        
        // todo ...
        return;
    }
#else
    #error "spi stm32f10x not support register operation yet!"
#endif
}


int spi_config(spi_t *spi, int param, uint8_t *ret, ...)
{
    return 0;
}

static int spi_write_read(spi_t *spi, uint8_t byte)
{
    int ret = 1000;
    while ((SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_TXE) == RESET) && ret--);
    if (ret < 0) {
        return ret;
    }
    ret = 1000;
    spi->spi->DR = byte; //SPI_I2S_SendData(spi->spi, byte);
    while ((SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_RXNE) == RESET) && ret--);
    if (ret < 0) {
        return ret;
    }
    return (0x00FF & spi->spi->DR); //SPI_I2S_ReceiveData(spi->spi);
}

size_t spi_transfer(spi_t *spi, uint8_t *buff, size_t size)
{
    for (int i = 0; i < size; ++i) {
        while (SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_TXE) == RESET);
        //SPI_I2S_SendData(spi->spi, byte);
        spi->spi->DR = buff[i];
        while (SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_RXNE) == RESET);
        buff[i]= spi->spi->DR; //SPI_I2S_ReceiveData(spi->spi);
    }
    return size;
}

size_t spi_write(spi_t *spi, uint8_t *data, size_t size)
{
    int i = 0;
    
    for (i = 0; i < size; ++i) {
        if (spi_write_read(spi, data[i]) < 0) {
            return 0;
        }
    }

    return size;
}
size_t spi_read(spi_t *spi, uint8_t *buff, size_t size)
{
    int i = 0, d;
    
    for (i = 0; i < size; ++i) {
        if ((d = spi_write_read(spi, 0xFF)) < 0) {
            return 0;
        }
        buff[i] = d;
    }

    return size;	
}
