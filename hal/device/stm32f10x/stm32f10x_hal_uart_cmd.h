/*
   The MIT License (MIT)

   Copyright (c) 2021 Doerthous

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

#ifndef STM32F10X_HAL_UART_CMD_H_
#define STM32F10X_HAL_UART_CMD_H_

enum 
{
    // (size_t timeout)
    STM32F10X_HAL_UART_CMD_SET_RX_TIMEOUT = 2,

    // (int)
    // [STM32F10X_HAL_UART_RX_POLL_MODE|STM32F10X_HAL_UART_RX_INTR_MODE]
    #define STM32F10X_HAL_UART_RX_POLL_MODE   1
    #define STM32F10X_HAL_UART_RX_INTR_MODE   2
    STM32F10X_HAL_UART_CMD_SET_RX_MODE,

    // (uint8_t *buff, size_t size)
    STM32F10X_HAL_UART_CMD_SET_RX_BUFFER,

    // (int)
    STM32F10X_HAL_UART_CMD_SET_BAUDRATE,
};

#endif // STM32F10X_HAL_UART_CMD_H_
