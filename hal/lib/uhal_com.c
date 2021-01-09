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

#include "uhal_com.h"
#include <dts/embedded/lib/delay.h>

#define SELF ((uhal_com_t)com)


uint32_t uhal_com_p_send(void *com, uint8_t *data, uint32_t size)
{
    uint32_t wc;


    for (wc = 0; wc < size; ++wc) {
        while (!SELF->sent(com));
        SELF->send(com, data[wc]);
    }

    return wc;
}

uint32_t uhal_com_p_recv(void *com, uint8_t *buff, uint32_t size)
{
    uint32_t rc;


    for (rc = 0; rc < size; ++rc) {
        while (!SELF->received(com));
        buff[rc] = (uint8_t)SELF->recv(com);
    }

    return rc;
}


#if UHAL_COM_INTERRUPT_ENABLE == 1

void uhal_com_sent(void *com)
{
    if (!blist_empty(&SELF->tx.buff)) {
        SELF->tx.running = 1;        
        SELF->send(com, blist_dequeue(&SELF->tx.buff));
    }
    else {
        SELF->tx.running = 0;
    }
}

void uhal_com_received(void *com)
{
    blist_enqueue(&SELF->rx.buff, (uint8_t)SELF->recv(com));
}

uint32_t uhal_com_i_send(void *com, uint8_t *data, uint32_t size)
{
    blist_init(&SELF->tx.buff, data, size, size);
    
    while (!blist_empty(&SELF->tx.buff)) {
        if (!SELF->tx.running) {
            SELF->tx.running = 1;
            SELF->send(com, blist_dequeue(&SELF->tx.buff));
        }
    }

    return size;
}

uint32_t uhal_com_i_recv(void *com, uint8_t *buff, uint32_t size)
{
    blist_init(&SELF->rx.buff, buff, size, 0);

    do {;} while (blist_length(&SELF->rx.buff) < size);
        
    blist_mem_adjust(&SELF->rx.buff);
    
    return size;
}

#endif


#if (UHAL_COM_INTERRUPT_ENABLE == 1) && (UHAL_COM_BUFFER_ENABLE == 1)

uint32_t uhal_com_i_b_send(void *com, uint8_t *data, uint32_t size)
{
    int wc = 0;
    int twc = 0;

   
    while (size > 0) {
        while (blist_size(&SELF->tx.buff) - blist_length(&SELF->tx.buff) < size
            && !blist_empty(&SELF->tx.buff));
        
        wc = blist_insert_all(&SELF->tx.buff, data, size);
        data += wc;
        size -= wc;
        twc += wc;
        
        if (!SELF->tx.running) {
            SELF->tx.running = 1;
            SELF->send(com, blist_dequeue(&SELF->tx.buff));
        }
    }
    
    return twc;
}

uint32_t uhal_com_i_b_recv(void *com, uint8_t *buff, uint32_t size)
{
    uint32_t rc = 0;
    
    
    while (rc < size) {
        while (!blist_full(&SELF->rx.buff) 
            && (blist_length(&SELF->rx.buff) < size));
        rc += blist_remove_all(&SELF->rx.buff, buff+rc, size-rc);
    }
        
    return rc;
}

#endif


#if UHAL_COM_TICK_TIMEOUT_ENABLE == 1

uint32_t uhal_com_p_t_recv(void *com, uint8_t *buff, uint32_t size)
{
    int i;
    uint32_t tick;

    
    i = 0;
    tick = uhal_com_tick();
    
    while (i < size && (uhal_com_tick() - tick < SELF->rx.timeout)) {
        if (SELF->received(com)) {
            buff[i++] = (uint8_t)SELF->recv(com);
            tick = uhal_com_tick();
        }
    }
        
    return i;
}

#endif

#if (UHAL_COM_TICK_TIMEOUT_ENABLE == 1) && (UHAL_COM_BUFFER_ENABLE == 1)

#endif


#if (UHAL_COM_INTERRUPT_ENABLE == 1) && (UHAL_COM_DELAY_TIMEOUT_ENABLE == 1)

uint32_t uhal_com_i_d_recv(void *com, uint8_t *buff, uint32_t size)
{
    uint32_t c;
    uint32_t to = 1;
    
    
    blist_init(&SELF->rx.buff, buff, size, 0);
    
    while (to > 0) {
        to = SELF->rx.timeout;
        c = blist_length(&SELF->rx.buff);
        
        while (to > 0 && c == blist_length(&SELF->rx.buff)) {
            delay_ms(1);
            --to;
        }
    }
    
    blist_mem_adjust(&SELF->rx.buff);
        
    return c;
}

#endif

#if (UHAL_COM_INTERRUPT_ENABLE == 1) \
    && (UHAL_COM_DELAY_TIMEOUT_ENABLE == 1) \
    && (UHAL_COM_BUFFER_ENABLE == 1)

uint32_t uhal_com_i_d_b_recv(void *com, uint8_t *buff, uint32_t size)
{
    uint32_t c = 0;
    uint32_t l = 0;
    uint32_t to = 1;
    
    
    while (to > 0 && c < size) {
        to = SELF->rx.timeout;
        
        while (((l = blist_length(&SELF->rx.buff)) == 0) && to > 0) {
            delay_ms(1);
            --to;
        }
        
        // wait for interrupt idle
        while (to > 0) {
            delay_ms(1); 
            if (l == blist_length(&SELF->rx.buff)) {
                break;
            }
            l = blist_length(&SELF->rx.buff);
        };
        
        while (l--) {
            c += blist_remove_all(&SELF->rx.buff, buff+c, size-c);
            if (c == size) {
                break;
            }
        }
    }
        
    return c;
}

#endif
