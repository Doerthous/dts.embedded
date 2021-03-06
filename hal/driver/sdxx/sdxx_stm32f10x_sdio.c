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
#include <stm32f10x.h>
#include <dts/embedded/lib/delay.h>


//# Debug
//#define DEBUG
#ifdef DEBUG
  #include <dts/embedded/hal/lib/uart_printf.h>
  #define PRINTF printf
#else
  #define PRINTF(...)
#endif


//#
#define SDIO_INIT_CLK_DIV                   ((uint8_t)0xB2) // 400K
#define SDIO_TRANSFER_CLK_DIV               ((uint8_t)0x01) // 72M/(1+2)=24M
//
#define SDIO_STATIC_FLAGS                   ((uint32_t)0x000005FF)
#define SD_DATATIMEOUT                      ((uint32_t)0xFFFFFFFF)
#define SDIO_FIFO_ADDRESS                   ((uint32_t)0x40018080)


//#
void sdio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    SDIO_InitTypeDef SDIO_InitStructure;


    // GPIOC and GPIOD Periph clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    // Configure PC.08, PC.09, PC.10, PC.11, PC.12 
    // pin: D0, D1, D2, D3, CLK pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
        GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Configure PD.02 CMD line
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    // Enable the SDIO AHB Clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
    
    
    /*!< Disable SDIO Clock */
    SDIO_ClockCmd(DISABLE);
    /*!< Set Power State to OFF */
    SDIO_SetPowerState(SDIO_PowerState_OFF);
    /*!< DeInitializes the SDIO peripheral */
    SDIO_DeInit();

    SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
    SDIO_InitStructure.SDIO_HardwareFlowControl = 
        SDIO_HardwareFlowControl_Disable;
    SDIO_Init(&SDIO_InitStructure);

    //Set Power State to ON
    SDIO_SetPowerState(SDIO_PowerState_ON);

    // Enable SDIO Clock
    SDIO_ClockCmd(ENABLE);
}

static uint32_t resp_type(uint8_t cmd)
{
    switch (cmd) {
        case 2:
        case 9:
            return SDIO_Response_Long;
        case 0:
            return SDIO_Response_No;
        default:
            return SDIO_Response_Short;
    }
}

static int config(sdxx_t *sdxx)
{
    SDIO_InitTypeDef SDIO_InitStructure;


    switch (sdxx->bus_width) {
        case 1:
            SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
            break;
        case 4:
            SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;
            break;
    }

    if (sdxx->frequency <= 400000) {
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;
    }
    else {
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
        sdxx->frequency = 24000000;
    }

    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_HardwareFlowControl = 
        SDIO_HardwareFlowControl_Disable;
    SDIO_Init(&SDIO_InitStructure);

    return SDXX_OK;
}

static int ask(struct sdxx *sdxx, 
    uint8_t cmd, uint32_t arg, uint8_t *res, uint32_t size)
{
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;  
    int i = 5;
    uint32_t *pb = (uint32_t *)res;    


    // send command
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);
    
    SDIO_CmdInitStructure.SDIO_Argument = arg;
    SDIO_CmdInitStructure.SDIO_CmdIndex = cmd;
    SDIO_CmdInitStructure.SDIO_Response = resp_type(cmd);
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    if (SDIO_CmdInitStructure.SDIO_Response == SDIO_Response_No) {
        i = 2000;
        while (i && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET)) {
            delay_us(1);
            i -= 1;
            if (i == 0) {
                PRINTF("%s: %d\n", __func__, SDXX_TIMEOUT);
                return SDXX_TIMEOUT;
            }
        }
        SDIO_ClearFlag(SDIO_FLAG_CMDSENT);

        return SDXX_OK;
    }


    // wait for response
    i = 2000;
    while (!(SDIO->STA & 
        (SDIO_FLAG_CCRCFAIL 
            | SDIO_FLAG_CMDREND 
            | SDIO_FLAG_CTIMEOUT)) && i) {
        delay_us(1);
        --i;
    }

    switch (size/4) {
        case 4:
            *pb++ = SDIO_GetResponse(SDIO_RESP4);
        case 3:
            *pb++ = SDIO_GetResponse(SDIO_RESP3);
        case 2:
            *pb++ = SDIO_GetResponse(SDIO_RESP2);
        case 1:
            *pb++ = SDIO_GetResponse(SDIO_RESP1);
    }

    if (!i || (SDIO->STA & SDIO_FLAG_CTIMEOUT)) {
        PRINTF("%s: %d\n", __func__, SDXX_TIMEOUT);
        return SDXX_TIMEOUT;
    }

    if (SDIO->STA & SDIO_FLAG_CCRCFAIL) {
        PRINTF("%s: %d\n", __func__, SDXX_CRC_ERROR);
        return SDXX_CRC_ERROR;
    }

    return SDXX_OK;
}

static void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); 
    
    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 
        | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

    /*!< DMA2 Channel4 disable */
    DMA_Cmd(DMA2_Channel4, DISABLE);

    /*!< DMA2 Channel4 Config */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /*!< DMA2 Channel4 enable */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

static void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); 

    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 
        | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

    /*!< DMA2 Channel4 disable */
    DMA_Cmd(DMA2_Channel4, DISABLE);

    /*!< DMA2 Channel4 Config */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
    /*!< DMA2 Channel4 enable */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

static void recv(sdxx_t *_sdxx, uint8_t *buff, uint64_t size)
{
    SDIO_DataInitTypeDef SDIO_DataInitStructure;
    int blk_sz;


    if (size == 8) {
        blk_sz = 3;
    }
    else {
        blk_sz = 9;
    }

	//SDIO_DMACmd(DISABLE);
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = size;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) (blk_sz << 4);
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    SDIO_DMACmd(ENABLE);
    SD_LowLevel_DMA_RxConfig((uint32_t *)buff, size);
}

static void send(sdxx_t *_sdxx, uint8_t *buff, uint64_t size)
{
	SDIO_DataInitTypeDef SDIO_DataInitStructure;
	

    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = size;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    SD_LowLevel_DMA_TxConfig((uint32_t *)buff, size);
    SDIO_DMACmd(ENABLE);
}

static int transfer_end(sdxx_t *sdxx)
{
    if (SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) == SET) {
        return SDXX_OK;
    }

    return SDXX_ERROR;
}

void sdxx_stm32f10x_sdio_init(sdxx_t *sdxx)
{
    sdio_init();
    sdxx->config = config;
    sdxx->recv = recv;
    sdxx->send = send;
    sdxx->ask = ask;
    sdxx->transfer_end = transfer_end;
}
