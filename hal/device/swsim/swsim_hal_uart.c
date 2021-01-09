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

#ifndef DTS_SWSIM_HAL_UART
# include "swsim_hal_uart.h"
# define uart_t swsim_hal_uart_t
# define uart_init swsim_hal_uart_init
# define uart_config swsim_hal_uart_config
# define uart_write swsim_hal_uart_write
# define uart_read swsim_hal_uart_read
#else
# include <dts/embedded/hal/uart.h>
#endif // DTS_SWSIM_HAL_UART

#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/timer.h>

enum DTS_HAL_UART_EX_DEF
{
    DTS_HAL_UART_EVE_ENUM,
    DTS_HAL_UART_EVE_TX_COMPLETE, //< data sent
    DTS_HAL_UART_EVE_RX_COMPLETE, //< received a data
    DTS_HAL_UART_EX_DEF_COUNT,
};

//# Component
static void uart_send(void *uart, uint16_t data)
{
    ((uart_t *)uart)->tx.data = data;
    ((uart_t *)uart)->tx.data <<= 1; /*< start bit */
    ((uart_t *)uart)->tx.data |= (1 << 9); /*< stop bit */
    ((uart_t *)uart)->tx.idx = 0;
    ((uart_t *)uart)->tx.dlen = 10;
    ((uart_t *)uart)->tx.complete = 0;
    timer_restart(((uart_t *)uart)->tx.tmr);
}
static uint16_t uart_recv(void *uart)
{
    ((uart_t *)uart)->rx.complete = 0;
    return ((uart_t *)uart)->rx.data;
}
static int uart_sent(void *uart)
{
    return (((uart_t *)uart)->tx.complete);
}
static int uart_received(void *uart)
{
    return (((uart_t *)uart)->rx.complete);
}

size_t uart_write(uart_t *uart, uint8_t *data, size_t size)
{
    return uart->write((void *)uart, data, size);
}

size_t uart_read(uart_t *uart, uint8_t *buff, size_t size)
{
    return uart->read((void *)uart, buff, size);
}

//# ISR
static void default_callback(uart_t *uart, int event)
{
    if (event == DTS_HAL_UART_EVE_RX_COMPLETE) {
        uhal_com_received(uart);
    }
    else {
        uhal_com_sent(uart);
    }
}


//# Emulation
//## Debug
//#define DEBUG
#ifdef DEBUG
  extern gpio_t e,f,g;
  #define DEBUG_IO1 e
  #define DEBUG_IO2 f
  #define DEBUG_IO3 g
  #define GPIO_SET(g) gpio_set(&g)
  #define GPIO_RESET(g) gpio_reset(&g)
  #define GPIO_TOGGLE(g) gpio_toggle(&g)
#else
  #define GPIO_SET(g)
  #define GPIO_RESET(g)
  #define GPIO_TOGGLE(g)
#endif

static void uart_tx_tmr_cb(timer_t * t, void *d)
{
    uart_t *u;

    GPIO_SET(DEBUG_IO3);

    u = (uart_t *)d;

    if (u->tx.idx < u->tx.dlen) {
        gpio_write(u->tx.pin, (u->tx.data >> u->tx.idx) & 0x01);
        u->tx.idx += 1;
    }
    else {
        u->tx.complete = 1;
        timer_stop(u->tx.tmr);
        if (u->tx.callback_enable) {
            u->callback(u, DTS_HAL_UART_EVE_TX_COMPLETE);
        }
    }

    GPIO_RESET(DEBUG_IO3);
}

static void uart_rx_tmr_cb(timer_t * t, void *d)
{
    uart_t *u;

    GPIO_SET(DEBUG_IO1);

    u = (uart_t *)d;

    if (u->rx.idx < u->rx.dlen) {
        u->rx.shift_reg |= ((gpio_read(u->rx.pin) & 0x0001) << u->rx.idx);
        u->rx.idx += 1;
        if (u->rx.idx == u->rx.dlen) {
            timer_stop(u->rx.tmr);
            gpio_intr_enable(u->rx.pin);
            u->rx.data = u->rx.shift_reg;
            u->rx.complete = 1;
            if (u->rx.callback_enable) {
                u->callback(u, DTS_HAL_UART_EVE_RX_COMPLETE);
            }
        }
    }

    GPIO_RESET(DEBUG_IO1);
}

static void uart_rx_io_cb(void *d)
{
    uart_t *u;


    GPIO_SET(DEBUG_IO2);
    
    u = (uart_t *)d;

    u->rx.idx = 0;
    // skip start bit, 0-7: data bits, no parity check yet, skip stop bit.
    u->rx.dlen = 8;
    u->rx.shift_reg = 0;
    gpio_intr_disable(u->rx.pin);
    // we need to may sure the first sample circle was correct! maybe we don't 
    // need restart here, because from interrupt come here takes time. 
    timer_start(u->rx.tmr);
    
    GPIO_RESET(DEBUG_IO2);
}




void uart_init(uart_t *uart)
{
    uart->low_level_init(uart);

    uart->baud_rate = 9600;
    uart->tx.complete = 1;

    timer_set_callback(uart->tx.tmr, uart_tx_tmr_cb, uart);
    timer_set_frequency(uart->tx.tmr, uart->baud_rate);

    timer_set_callback(uart->rx.tmr, uart_rx_tmr_cb, uart);
    timer_set_frequency(uart->rx.tmr, uart->baud_rate);

    gpio_write(uart->tx.pin, 1);

    gpio_intr_set_handler(uart->rx.pin, 
        DTS_HAL_GPIO_TRIGGER_FALLING, uart_rx_io_cb, uart);
    gpio_intr_enable(uart->rx.pin);

    uart->callback = default_callback;
    uart->_.send = uart_send;
    uart->_.recv = uart_recv;
    uart->_.sent = uart_sent;
    uart->_.received = uart_received;
    uart->write = uhal_com_p_send;
    uart->read = uhal_com_p_recv;
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
        case SWSIM_HAL_UART_RX_POLL_MODE:
            if (uhal_com_get_rx_timeout(&uart->_) == 0) {
                //USART_ITConfig(to_st_uart(uart), USART_IT_RXNE ,DISABLE);
                uart->read = uhal_com_p_recv;
                //uart->rx.mode.base = mode;
                return 1;
            }
            #if defined(UHAL_USING_TICK_MS)
                //USART_ITConfig(to_st_uart(uart), USART_IT_RXNE ,DISABLE);
                uart->read = (uart_read_t)uart_p_t_read;            
                //uart->rx.mode.timeout = 1;
                uart->rx.mode.base = mode;
                return UART_CONF_OK;
            #elif defined(UHAL_USING_DELAY_MS)
                //USART_ITConfig(to_st_uart(uart), USART_IT_RXNE ,DISABLE);
                uart->read = (uart_read_t)uart_p_d_read;
                //uart->rx.mode.timeout = 1;  
                uart->rx.mode.base = mode; 
                return UART_CONF_OK;            
            #else
                return 0;
            #endif


        // Set uart rx as interrupte mode
        case SWSIM_HAL_UART_RX_INTR_MODE:
            if (uhal_com_get_rx_timeout(&uart->_) == 0 
                && !uhal_com_get_rx_buffer(&(uart->_))) {
                uart->rx.callback_enable = 0;
                uart->read = uhal_com_i_recv;
            }
            if (uhal_com_get_rx_timeout(&uart->_) > 0 
                && !uhal_com_get_rx_buffer(&(uart->_))) {
                uart->rx.callback_enable = 0;
                uart->read = uhal_com_i_d_recv;
            }
            if (uhal_com_get_rx_timeout(&uart->_) == 0 
                && uhal_com_get_rx_buffer(&(uart->_))) {
                uart->rx.callback_enable = 1;
                uart->read = uhal_com_i_b_recv;
            }
            if (uhal_com_get_rx_timeout(&uart->_) > 0
                && uhal_com_get_rx_buffer(&(uart->_))) {
                uart->rx.callback_enable = 1;
                uart->read = uhal_com_i_d_b_recv;
            }
            //uart->rx.mode.base = mode;
            return 0;
    }

    return 0;
}

#include <stdarg.h>
int uart_config(uart_t *uart, int cmd, ...)
{
    va_list args;
    int ret = -1;
    uint8_t *buff;
    size_t size;
    
    va_start(args, cmd);

    switch (cmd) {
        case SWSIM_HAL_UART_CMD_SET_RX_TIMEOUT:
            uhal_com_set_rx_timeout(&uart->_, va_arg(args, size_t));
            ret = 0;
            break;
        case SWSIM_HAL_UART_CMD_SET_RX_MODE:
            if (uart_rx_mode_config(uart, va_arg(args, int))) {
                ret = 0;
            }
            break;
        case SWSIM_HAL_UART_CMD_SET_RX_BUFFER:
            buff = va_arg(args, uint8_t *);
            size = va_arg(args, size_t);
            uhal_com_set_rx_buffer(uart, buff, size);
            ret = 0;
            break;
        case SWSIM_HAL_UART_CMD_SET_BAUDRATE: {
            uart->baud_rate = va_arg(args, int);
            timer_set_frequency(uart->tx.tmr, uart->baud_rate);
            timer_set_frequency(uart->rx.tmr, uart->baud_rate);
            ret = 0;
        } break;
    }
    
    va_end(args);
    
    return ret;
}
