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

#include "blist.h"
#include <string.h>


#define blist_pos_add(bl, pos, acc) \
    ((pos + acc) % bl->__size)

#define blist_pos_sub(bl, pos, acc) \
    ((pos - acc + bl->__size) % bl->__size)


#if defined(OOP)
uint32_t blist_length(blist_t bl)
{
    return bl->__length;
}
uint32_t blist_size(blist_t bl)
{
    return bl->__size;
}
int blist_empty(blist_t bl)
{
    return bl->__length == 0;
}
int blist_full(blist_t bl)
{
    return bl->__length == bl->__size;
}
void blist_clear(blist_t bl)
{
    bl->__head = 0;
    bl->__tail = 0;
    bl->__length = 0;
}
#endif


blist_t blist_init(blist_t bl, uint8_t *buff, uint32_t size, uint32_t len)
{
    bl->__buff = buff;
    bl->__size = size;
    bl->__length = len;    
    bl->__head = len % size;
    bl->__tail = 0;
    
#if defined(OOP)
    bl->self = bl;
    bl->length = blist_length;
    bl->size = blist_size;
    bl->empty = blist_empty;
    bl->full = blist_full;
    bl->clear = blist_clear;
    
#if defined(STACK)
    bl->push = blist_push;
    bl->pop = blist_pop;
#endif

#if defined(QUEUE)
    bl->enqueue = blist_enqueue;
    bl->dequeue = blist_dequeue;
#endif
#endif

	return bl;
}


#if defined(QUEUE) || defined(STACK)
void blist_insert(blist_t bl, uint32_t pos, uint8_t b)
{
    // only support put at head now.
    if (pos == bl->__head) {
        if (bl->__length < bl->__size) {
            bl->__buff[bl->__head] = b;
            bl->__head = blist_pos_add(bl, bl->__head, 1);
            bl->__length += 1;
        }
        return;
    }
    
    // no support now
}
#endif



#if defined(QUEUE)
void blist_enqueue(blist_t bl, uint8_t b)
{
    blist_insert(bl, bl->__head, b);
}

uint8_t blist_dequeue(blist_t bl)
{
    uint8_t b = 0xFF;

    
    if (bl->__length > 0) {
        b = bl->__buff[bl->__tail];
        bl->__tail = blist_pos_add(bl, bl->__tail, 1);
        bl->__length -= 1;
	}

    return b;
}
#endif


#if defined(STACK)
void blist_push(blist_t bl, uint8_t b)
{
    blist_insert(bl, bl->__head, b);
}

uint8_t blist_pop(blist_t bl)
{
    uint8_t b = 0xFF;

    
    if (bl->__length > 0) {
        bl->__head = blist_pos_sub(bl, bl->__head, 1);
        b = bl->__buff[bl->__head];
        bl->__length -= 1;
    }

    return b;
}
#endif

/* uint8_t blist_get(blist_t *bl, uint32_t pos) */
/* { */
/*     uint8_t b = 0xFF; */


/*     if (bl->__length > 0) { */
/*         b = bl->__buff[blist_pos_add(bl, bl->__tail, pos)]; */
/*     } */
    
/*     return b; */
/* } */
/* uint32_t blist_pop_all(blist_t *bl, uint8_t *buff, uint32_t size) */
/* { */
/*     uint32_t head; */
/*     uint32_t i; */

/*     head = bl->__tail; */
/*     size = size > bl->__length ? bl->__length : size; */
    
/*     for (i = 0; i < size; ++i) { */
/*         buff[i] = bl->__buff[head]; */
/*         head = blist_pos_add(bl, head, 1); */
/*         bl->__length -= 1; */
/*     } */

/*     return size; */
/* } */
/* uint8_t blist_remove(blist_t *bl, uint32_t pos); */
/* uint8_t blist_get(blist_t *bl, uint32_t pos); */
uint32_t blist_insert_all(blist_t bl, uint8_t *buff, uint32_t size)
{
	int pos;
	int i;


    i = bl->__size - bl->__length;
    size = i > size ? size : i;

    for (i = 0, pos = bl->__head; i < size; pos = blist_pos_add(bl, pos, 1)) {
        bl->__buff[pos] = buff[i++];
    }

    bl->__head = pos;
    bl->__length += size;
    
    return size;
}

uint32_t blist_remove_all(blist_t bl, uint8_t *buff, uint32_t size)
{
    int pos = 0;
    int i = 0;


    size = bl->__length > size ? size : bl->__length;

    for (pos = bl->__tail; i < size; pos = blist_pos_add(bl, pos, 1)) {
        buff[i++] = bl->__buff[pos];
    }

    bl->__tail = pos;
    bl->__length -= size;

    return size;
}

static void memrev(uint8_t *mem, size_t size)
{
    int i;
    

    for (i = 0, --size; i < size; ++i, --size) {
        if (mem[i] != mem[size]) {
            mem[i] ^= mem[size];
            mem[size] ^= mem[i];
            mem[i] ^= mem[size];
        }
    }
}

void blist_mem_adjust(blist_t bl)
{
    if (bl->__tail > bl->__head) {
        memrev(bl->__buff, bl->__head);
        memrev(bl->__buff + bl->__head, bl->__size - bl->__head);
        memrev(bl->__buff, bl->__size);
        memmove(bl->__buff, bl->__buff + bl->__tail - bl->__head,
            bl->__length);
        bl->__head = bl->__length;
    }
}
