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

#ifndef DTS_STM32F10X_HAL_UART
# include "stm32f10x_hal_uart.h"
# define uart_t stm32f10x_hal_uart_t
# define uart_init stm32f10x_hal_uart_init
# define uart_config stm32f10x_hal_uart_config
# define uart_write stm32f10x_hal_uart_write
# define uart_read stm32f10x_hal_uart_read
#else
# include <dts/embedded/hal/uart.h>
#endif // DTS_STM32F10X_HAL_UART

#include <dts/embedded/hal/interrupt.h>

#ifndef DTS_HAL_UART_DEFAULT_BAUD_RATE
#define DTS_HAL_UART_DEFAULT_BAUD_RATE 9600
#endif // DTS_HAL_UART_DEFAULT_BAUD_RATE

#ifndef DTS_HAL_UART_LOCK_ENABLE
# undef dts_hal_lock_new
# undef dts_hal_lock_delete
# undef dts_hal_lock_lock
# undef dts_hal_lock_unlock
# define dts_hal_lock_new(hal_obj)
# define dts_hal_lock_delete(hal_obj)
# define dts_hal_lock_lock(hal_obj)
# define dts_hal_lock_unlock(hal_obj)
#endif // DTS_HAL_UART_LOCK_ENABLE

#define to_st_uart(uart) ((uart_t *)(uart))->st_uart

static void uart_send(void *uart, uint16_t data)
{
    USART_SendData(to_st_uart(uart), data); 
}
static uint16_t uart_recv(void *uart)
{
    return USART_ReceiveData(to_st_uart(uart));
}
static int uart_sent(void *uart)
{
    return USART_GetFlagStatus(to_st_uart(uart), USART_FLAG_TXE) == SET;
}
static int uart_received(void *uart)
{
    return USART_GetFlagStatus(to_st_uart(uart), USART_FLAG_RXNE) == SET;
}

size_t uart_write(uart_t *uart, uint8_t *data, size_t size)
{
    dts_hal_lock_lock(uart);

    size = uart->write(uart, data, size);

    dts_hal_lock_unlock(uart);

    return size;
}

size_t uart_read(uart_t *uart, uint8_t *buff, size_t size)
{
    dts_hal_lock_lock(uart);

    size = uart->read(uart, buff, size);

    dts_hal_lock_unlock(uart);

    return size;
}

/**
 * @brief      Uart Interrupt Service Routine
 *
 * @param[in]  irq   The interrupt request number
 */
static void uart_isr(void *u)
{
    uart_t *uart = (uart_t *)u;

    if (USART_GetITStatus(to_st_uart(uart), USART_IT_RXNE)) {
        uhal_com_received(uart);
        USART_ClearITPendingBit(to_st_uart(uart), USART_IT_RXNE);
    }
    
    if (USART_GetITStatus(to_st_uart(uart), USART_IT_TC)) {
        uhal_com_sent(uart);
        USART_ClearITPendingBit(to_st_uart(uart), USART_IT_TC);
    }
}



void uart_init(uart_t *uart)
{
    uart->read = uhal_com_p_recv;
    uart->write = uhal_com_p_send;
    uart->_.send = uart_send;
    uart->_.recv = uart_recv;
    uart->_.sent = uart_sent;
    uart->_.received = uart_received;
    dts_hal_lock_new(uart);
    
    #if defined(USE_STDPERIPH_DRIVER)
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    if (to_st_uart(uart) == USART1) {
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
          
        //USART1_TX   GPIOA.9
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        //USART1_RX      GPIOA.10
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
     
        //USART init
        USART_InitStructure.USART_BaudRate = DTS_HAL_UART_DEFAULT_BAUD_RATE;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

        USART_Init(USART1, &USART_InitStructure);
        USART_Cmd(USART1, ENABLE);
        
        interrupt_enable(USART1_IRQn);
        interrupt_set_handler(USART1_IRQn, uart_isr);
        interrupt_set_handler_data(USART1_IRQn, (void *)uart);
        
        USART_ClearFlag(USART1, USART_FLAG_RXNE); // in case of some bad state.
        
        return;
    }
    #endif
    
}

/**
 * @brief      Uart Rx Behavier Configuration.
 * 
 * @param      uart  The uart
 * @param[in]  mode  Uart rx base mode [poll|interrupt]
 *
 * @return     UART_CONF_STATE
 * 
 * @note       **Only** timeout will cause an UART_CONF_NOT_TIMEOUT_SUPPORT
 *             error.
 */
static int uart_rx_mode_config(uart_t *uart, int mode)
{    
    switch (mode) {
        // Set uart rx as poll mode
        //  We don't delete buff here so that we can directly change to buff
        //  mode. We only check timeout here.
        //  Only when we set rx poll mode successed can we disable the interrupt
        //  of rx.
        //  
        // 1. if timeout == 0
        //      use uart_p_read
        //      return UART_CONF_OK
        // 2. if timeout != 0 && defined(UHAL_USING_TICK_MS)
        //      use uart_p_t_read
        //      return UART_CONF_OK
        // 3. if timeout != 0 && defined(UHAL_USING_DELAY_MS)
        //      use uart_p_d_read
        //      return UART_CONF_OK
        // 4. return UART_CONF_NOT_TIMEOUT_SUPPORT
        // 
        case STM32F10X_HAL_UART_RX_POLL_MODE:
            if (uhal_com_get_rx_timeout(&uart->_) == 0) {
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE ,DISABLE);
                uart->read = uhal_com_p_recv;
                //uart->rx.mode.base = mode;
                return 0;
            }
            #if defined(UHAL_USING_TICK_MS)
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE ,DISABLE);
                uart->read = (uart_read_t)uart_p_t_read;            
                //uart->rx.mode.timeout = 1;
                uart->rx.mode.base = mode;
                return UART_CONF_OK;
            #elif defined(UHAL_USING_DELAY_MS)
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE ,DISABLE);
                uart->read = (uart_read_t)uart_p_d_read;
                //uart->rx.mode.timeout = 1;  
                uart->rx.mode.base = mode; 
                return UART_CONF_OK;            
            #else
                return -1;
            #endif


        // Set uart rx as interrupte mode
        case STM32F10X_HAL_UART_RX_INTR_MODE:
            if (uhal_com_get_rx_timeout(&uart->_) == 0 
                && !uhal_com_get_rx_buffer(&(uart->_))) {
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE, DISABLE);
                uart->read = uhal_com_i_recv;
            }
            if (uhal_com_get_rx_timeout(&uart->_) > 0 
                && !uhal_com_get_rx_buffer(&(uart->_))) {
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE, DISABLE);
                uart->read = uhal_com_i_d_recv;
            }
            if (uhal_com_get_rx_timeout(&uart->_) == 0 
                && uhal_com_get_rx_buffer(&(uart->_))) {
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE, ENABLE);
                uart->read = uhal_com_i_b_recv;
            }
            if (uhal_com_get_rx_timeout(&uart->_) > 0
                && uhal_com_get_rx_buffer(&(uart->_))) {
                USART_ITConfig(to_st_uart(uart), USART_IT_RXNE, ENABLE);
                uart->read = uhal_com_i_d_b_recv;
            }
            //uart->rx.mode.base = mode;
            return 0;
    }

    return -1;
}


#include <stdarg.h>
int uart_config(uart_t *uart, int cmd, ...)
{
    va_list args;
    int ret = 0;
    uint8_t *buff;
    size_t size;
    
    va_start(args, cmd);

    switch (cmd) {
        case STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT:
            uhal_com_set_rx_timeout(&uart->_, va_arg(args, size_t));
            break;
        case STM32F10X_HAL_UART_CMD_SET_RX_MODE:
            ret = uart_rx_mode_config(uart, va_arg(args, int));
            break;
        case STM32F10X_HAL_UART_CMD_SET_RX_BUFFER:
            buff = va_arg(args, uint8_t *);
            size = va_arg(args, size_t);
            uhal_com_set_rx_buffer(uart, buff, size);
            break;
        case STM32F10X_HAL_UART_CMD_SET_BAUDRATE: {
            #define CR1_OVER8_Set             ((u16)0x8000)  /* USART OVER8 mode Enable Mask */
            #define CR1_OVER8_Reset           ((u16)0x7FFF)  /* USART OVER8 mode Disable Mask */
			int br = va_arg(args, int);
            uint32_t tmpreg = 0x00, apbclock = 0x00;
            uint32_t integerdivider = 0x00;
            uint32_t fractionaldivider = 0x00;
            
            RCC_ClocksTypeDef RCC_ClocksStatus;
            RCC_GetClocksFreq(&RCC_ClocksStatus);
            if (((uint32_t)to_st_uart(uart)) == USART1_BASE) {
                apbclock = RCC_ClocksStatus.PCLK2_Frequency;
            }
            else {
                apbclock = RCC_ClocksStatus.PCLK1_Frequency;
            }
            /* Determine the integer part */
            if ((to_st_uart(uart)->CR1 & CR1_OVER8_Set) != 0) {
                /* Integer part computing in case Oversampling mode is 8 Samples */
                integerdivider = ((25 * apbclock) / (2 * (br)));    
            }
            else { /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
                /* Integer part computing in case Oversampling mode is 16 Samples */
                integerdivider = ((25 * apbclock) / (4 * (br)));    
            }
            tmpreg = (integerdivider / 100) << 4;
            /* Determine the fractional part */
            fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
            /* Implement the fractional part in the register */
            if ((to_st_uart(uart)->CR1 & CR1_OVER8_Set) != 0) {
                tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
            }
            else { /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
                tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
            }
            /* Write to USART BRR */
            to_st_uart(uart)->BRR = (uint16_t)tmpreg;
        } break;
        default:
            ret = -1;
    }
    
    va_end(args);
    
    return ret;
}
