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

#ifndef UHAL_COM_H_
#define UHAL_COM_H_

// -----------------------------------------------------------------------------
//# Configuration
//

#include <dts_hal_conf.h>
#include <dts_hal_def.h>

#define UHAL_COM_INTERRUPT_ENABLE                                              1
#define UHAL_COM_BUFFER_ENABLE                                                 1
#define UHAL_COM_TICK_TIMEOUT_ENABLE                                           0
#define UHAL_COM_DELAY_TIMEOUT_ENABLE                                          1

#if UHAL_COM_INTERRUPT_ENABLE == 1
  #include "blist.h"
#endif


// -----------------------------------------------------------------------------
//# Definition

typedef struct uhal_com
{
    void (*send)(void *com, uint16_t data);
    uint16_t (*recv)(void *com);
    int (*sent)(void *com);
    int (*received)(void *com);


    struct {
      #if UHAL_COM_INTERRUPT_ENABLE == 1
        struct blist buff;
        uint8_t running;
      #endif

      #if UHAL_COM_TICK_TIMEOUT_ENABLE == 1 \
        || UHAL_COM_DELAY_TIMEOUT_ENABLE == 1
        uint32_t timeout;
      #endif
    } tx;
    struct {
      #if UHAL_COM_INTERRUPT_ENABLE == 1
        struct blist buff;
      #endif

      #if UHAL_COM_TICK_TIMEOUT_ENABLE == 1 \
        || UHAL_COM_DELAY_TIMEOUT_ENABLE == 1
        uint32_t timeout;
      #endif        
    } rx;
} * uhal_com_t;


// -----------------------------------------------------------------------------
// Components

/**
 * @brief      communication unit send callback. when send complete call this
 *             function.
 *
 * @param      com   communication unit.
 *
 * @note       using only in interrupt mode.
 */
void uhal_com_sent(void *com);

/**
 * @brief      uhal communication unit receive callback. when receive complete
 *             call this function.
 *
 * @param      com   communication unit.
 *
 * @note       using only in interrupt mode.
 */
void uhal_com_received(void *com);


void uhal_com_set_tx_timeout(void *com, uint32_t timeout);
void uhal_com_set_rx_timeout(void *com, uint32_t timeout);
size_t uhal_com_get_rx_timeout(void *com);

void uhal_com_set_tx_buffer(void *com, uint8_t *buff, uint32_t size);
void uhal_com_set_rx_buffer(void *com, uint8_t *buff, uint32_t size);
uint8_t *uhal_com_get_rx_buffer(void *com, uint8_t *buff);

// -----------------------------------------------------------------------------
// Interfaces
//
// p: poll, i: interrupt
// b: buffer
// d: delay, t: tick
// 
// buffer is only using in interrupt mode.
// 

uint32_t uhal_com_p_send(void *com, uint8_t *data, uint32_t size);
uint32_t uhal_com_p_recv(void *com, uint8_t *buff, uint32_t size);
//uint32_t uhal_com_p_t_send(void *com, uint8_t *buff, uint32_t size); // todo
uint32_t uhal_com_p_t_recv(void *com, uint8_t *buff, uint32_t size);
//uint32_t uhal_com_p_d_send(void *com, uint8_t *buff, uint32_t size); // todo
uint32_t uhal_com_p_d_recv(void *com, uint8_t *buff, uint32_t size);

uint32_t uhal_com_i_send(void *com, uint8_t *data, uint32_t size);
uint32_t uhal_com_i_recv(void *com, uint8_t *buff, uint32_t size);
//uint32_t uhal_com_i_t_send(void *com, uint8_t *buff, uint32_t size);
//uint32_t uhal_com_i_t_recv(void *com, uint8_t *buff, uint32_t size); // todo
//uint32_t uhal_com_i_d_send(void *com, uint8_t *buff, uint32_t size);
uint32_t uhal_com_i_d_recv(void *com, uint8_t *buff, uint32_t size);

uint32_t uhal_com_i_b_send(void *com, uint8_t *data, uint32_t size);
uint32_t uhal_com_i_b_recv(void *com, uint8_t *buff, uint32_t size);

//uint32_t uhal_com_i_t_b_send(void *com, uint8_t *buff, uint32_t size); // todo
//uint32_t uhal_com_i_t_b_recv(void *com, uint8_t *buff, uint32_t size); // todo
//uint32_t uhal_com_i_d_b_send(void *com, uint8_t *buff, uint32_t size); // todo
uint32_t uhal_com_i_d_b_recv(void *com, uint8_t *buff, uint32_t size);


// -----------------------------------------------------------------------------
// Wrapper
//
#if UHAL_COM_BUFFER_ENABLE == 1

#define uhal_com_set_tx_buffer(C, B, S) \
    blist_init(&((uhal_com_t)(C))->tx.buff, B, S, 0);
#define uhal_com_set_rx_buffer(C, B, S) \
    blist_init(&((uhal_com_t)(C))->rx.buff, B, S, 0);    
#define uhal_com_get_rx_buffer(C) \
    blist_buff(&(((uhal_com_t)(C))->rx.buff))
#endif


#if (UHAL_COM_TICK_TIMEOUT_ENABLE == 1) || (UHAL_COM_DELAY_TIMEOUT_ENABLE == 1)

#define uhal_com_set_tx_timeout(C, T) \
    ((uhal_com_t)(C))->tx.timeout = (T)
#define uhal_com_set_rx_timeout(C, T) \
    ((uhal_com_t)(C))->rx.timeout = (T)
#define uhal_com_get_rx_timeout(C) \
    ((uhal_com_t)(C))->rx.timeout
#endif

#endif // UHAL_COM_H_
