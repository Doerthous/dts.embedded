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

/**
  *             buff:
  *                      0                                          size-1
  *                 |    -    -    t    =    =    =    =    =    h    -    |
  *                      nil  nil  e0   e1   e2   e3   e4   e5   nil  nil
  *                                ^                        ^    ^   
  *                                |                        |    |
  *                                dequeue                  pop  push/enqueue
  *                 h: head, t: tail
  *                 nil: null
  *                 ex: element x
  */

#ifndef BLIST_H_
#define BLIST_H_

#include <stdint.h>


//#define OOP
//#define STACK
#define QUEUE


typedef struct blist
{
#if defined(OOP)
    struct blist *self;

    uint32_t (*length)(struct blist *self);
    uint32_t (*size)(struct blist *self);
    int (*empty)(struct blist *self);
    int (*full)(struct blist *self);
    void (*clear)(struct blist *self);
    
#if defined(STACK)
    void (*push)(struct blist *self, uint8_t b);
    uint8_t (*pop)(struct blist *self);
#endif

#if defined(QUEUE)
    void (*enqueue)(struct blist *self, uint8_t b);
    uint8_t (*dequeue)(struct blist *self);
#endif
#endif

    uint8_t *__buff;
    uint32_t __size;
    uint32_t __head;
    uint32_t __tail; // for safe thread.
    uint32_t __length;
} * blist_t;


blist_t blist_init(blist_t bl, uint8_t *buff, uint32_t size, uint32_t len);


// getter and checker
uint8_t *blist_buff(blist_t bl);
uint32_t blist_length(blist_t bl);
uint32_t blist_size(blist_t bl);
int blist_empty(blist_t bl);
int blist_full(blist_t bl);
void blist_clear(blist_t bl);


// queue operations
#if defined(QUEUE)
void blist_enqueue(blist_t bl, uint8_t b);
uint8_t blist_dequeue(blist_t bl);
#endif


// stack operations
#if defined(STACK)
void blist_push(blist_t bl, uint8_t b);
uint8_t blist_pop(blist_t bl);
#endif


// list operations
/* void blist_insert(blist_t bl, uint32_t pos, uint8_t b); */
/* uint8_t blist_remove(blist_t bl, uint32_t pos); */
/* uint8_t blist_get(blist_t bl, uint32_t pos); */

uint32_t blist_insert_all(blist_t bl, uint8_t *buff, uint32_t size); 
uint32_t blist_remove_all(blist_t bl, uint8_t *buff, uint32_t size); 

// other
void blist_mem_adjust(blist_t bl);


#if !defined(OOP)
// wrapper
#define blist_buff(bl) \
    (bl)->__buff
#define blist_length(bl) \
    (bl)->__length
#define blist_size(bl) \
    (bl)->__size
#define blist_empty(bl) \
    (blist_length(bl) == 0)
#define blist_full(bl) \
    (blist_length(bl) == blist_size(bl))
#define blist_clear(bl) \
    (bl)->__head = 0; \
    (bl)->__length = 0
#endif

#endif // BLIST_H_
