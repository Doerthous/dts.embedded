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

#include <sdxx.h>
#include <stdlib.h>
#include <string.h>

//#
#define USING_UART_PRINTF


//#
#if defined(USING_UART_PRINTF)
  #include <dts/embedded/hal/lib/uart_printf.h>
#endif

static int afc = 0;
#define ASSERT(expr) \
    if (!(expr)) { \
        afc += 1; \
        printf("assert failed: %s %s %d\n", __FILE__, __func__, __LINE__);\
    }


//#
#define BLOCK_CNT       10
#define BLOCK_SIZE      512
#define BUFF_SIZE       (BLOCK_CNT * BLOCK_SIZE)

static uint8_t buf1[BUFF_SIZE];
static uint8_t buf2[BUFF_SIZE];


//#
static void sdxx_test1(sdxx_t *sdxx)
{
    int ret, i, j;


    ret = sdxx_config(sdxx, SDXX_CMD_RX_MODE, SDXX_RX_DMA_SINGLE_BLK_ITER);
    ASSERT(ret == SDXX_OK);
    ret = sdxx_config(sdxx, SDXX_CMD_TX_MODE, SDXX_TX_DMA_SINGLE_BLK_ITER);
    ASSERT(ret == SDXX_OK);

    i = ((uint8_t)rand()) + 1;

    for (j = 0; j < BUFF_SIZE; ++j) {
        buf1[j] = (uint8_t)rand();
    }

    ret = sdxx_write_block(sdxx, i, BLOCK_CNT, buf1);
    ASSERT(ret == BLOCK_CNT);

    ret = sdxx_read_block(sdxx, i, BLOCK_CNT, buf2);
    ASSERT(ret == BLOCK_CNT);

    ASSERT(memcmp(buf1, buf2, BUFF_SIZE) == 0);
}

static void sdxx_test2(sdxx_t *sdxx)
{
    int ret, i, j;


    ret = sdxx_config(sdxx, SDXX_CMD_RX_MODE, SDXX_RX_DMA_MULTI_BLK);
    ASSERT(ret == SDXX_OK);
    ret = sdxx_config(sdxx, SDXX_CMD_TX_MODE, SDXX_TX_DMA_SINGLE_BLK_ITER);
    ASSERT(ret == SDXX_OK);


    i = ((uint8_t)rand()) + 1;

    for (j = 0; j < BUFF_SIZE; ++j) {
        buf1[j] = (uint8_t)rand();
    }

    ret = sdxx_write_block(sdxx, i, BLOCK_CNT, buf1);
    ASSERT(ret == BLOCK_CNT);

    ret = sdxx_read_block(sdxx, i, BLOCK_CNT, buf2);
    ASSERT(ret == BLOCK_CNT);

    ASSERT(memcmp(buf1, buf2, BUFF_SIZE) == 0);
}

static void sdxx_test3(sdxx_t *sdxx)
{
    int ret, i, j;


    ret = sdxx_config(sdxx, SDXX_CMD_RX_MODE, SDXX_RX_DMA_SINGLE_BLK_ITER);
    ASSERT(ret == SDXX_OK);
    ret = sdxx_config(sdxx, SDXX_CMD_TX_MODE, SDXX_TX_DMA_MULTI_BLK);
    ASSERT(ret == SDXX_OK);


    i = ((uint8_t)rand()) + 1;

    for (j = 0; j < BUFF_SIZE; ++j) {
        buf1[j] = (uint8_t)rand();
    }

    ret = sdxx_write_block(sdxx, i, BLOCK_CNT, buf1);
    ASSERT(ret == BLOCK_CNT);

    ret = sdxx_read_block(sdxx, i, BLOCK_CNT, buf2);
    ASSERT(ret == BLOCK_CNT);

    ASSERT(memcmp(buf1, buf2, BUFF_SIZE) == 0);
}

static void sdxx_test4(sdxx_t *sdxx)
{
    int ret, i, j;


    ret = sdxx_config(sdxx, SDXX_CMD_RX_MODE, SDXX_RX_DMA_MULTI_BLK);
    ASSERT(ret == SDXX_OK);
    ret = sdxx_config(sdxx, SDXX_CMD_TX_MODE, SDXX_TX_DMA_MULTI_BLK);
    ASSERT(ret == SDXX_OK);


    i = ((uint8_t)rand()) + 1;

    for (j = 0; j < BUFF_SIZE; ++j) {
        buf1[j] = (uint8_t)rand();
    }

    ret = sdxx_write_block(sdxx, i, BLOCK_CNT, buf1);
    ASSERT(ret == BLOCK_CNT);

    ret = sdxx_read_block(sdxx, i, BLOCK_CNT, buf2);
    ASSERT(ret == BLOCK_CNT);

    ASSERT(memcmp(buf1, buf2, BUFF_SIZE) == 0);
}


int sdxx_test(sdxx_t *sdxx)
{
    ASSERT(sdxx_init(sdxx) == SDXX_OK);

    sdxx_test1(sdxx);
    sdxx_test2(sdxx);
    sdxx_test3(sdxx);
    sdxx_test4(sdxx);

    printf("assert fault count: %d\n", afc);

    return afc;
}

/****************************** Copy right 2019 *******************************/
