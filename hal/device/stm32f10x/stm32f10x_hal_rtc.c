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

#ifndef DTS_STM32F10X_HAL_RTC
# include "stm32f10x_hal_rtc.h"
# define rtc_time_t stm32f10x_hal_rtc_time_t
# define rtc_t stm32f10x_hal_rtc_t
# define rtc_init stm32f10x_hal_rtc_init
# define rtc_config stm32f10x_hal_rtc_config
# define rtc_set stm32f10x_hal_rtc_set
# define rtc_get stm32f10x_hal_rtc_get
#else
# include <dts/embedded/hal/rtc.h>
#endif // DTS_STM32F10X_HAL_RTC

#include "../../lib/datetime.h"

#include <dts/embedded/lib/delay.h>

#include <stdint.h>

#define RTC_YEAR_START      1970
#define RTC_MONTH_BASE      1
#define RTC_DAY_BASE        1

int  rtc_init(rtc_t *rtc)
{
    // 检查是不是第一次配置时钟
    int temp = 0;
    
    // 使能PWR和BKP外设时钟, 使能后备寄存器访问  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);      
    PWR_BackupAccessCmd(ENABLE);

    // 从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050) {               
        BKP_DeInit();   // 复位备份区域    
        RCC_LSEConfig(RCC_LSE_ON);  // 设置外部低速晶振(LSE),使用外设低速晶振
        //检查指定的RCC标志位设置与否,等待低速晶振就绪
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
            temp++;
            delay_ms(10);
        }
        if(temp >= 250) {
            return 0; //初始化时钟失败,晶振有问题        
        }
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
        RCC_RTCCLKCmd(ENABLE);  //使能RTC时钟  
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
        RTC_WaitForSynchro();       //等待RTC寄存器同步  
        //RTC_ITConfig(RTC_IT_SEC, ENABLE);       //使能RTC秒中断
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
        RTC_EnterConfigMode();/// 允许配置  
        RTC_SetPrescaler(32767); //设置RTC预分频的值
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成  
        RTC_ExitConfigMode(); //退出配置模式  
        BKP_WriteBackupRegister(BKP_DR1, 0x5050);   //向指定的后备寄存器中写入用户程序数据
    }
    else {
        RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
        //RTC_ITConfig(RTC_IT_SEC, ENABLE);   //使能RTC秒中断
        RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
    }
    //RTC_NVIC_Config();//RCT中断分组设置                                
	
	return 1;
}

int  rtc_config(rtc_t *rtc, int cmd, ...)
{
    return 1;
}


int  rtc_set(rtc_t *rtc, rtc_time_t *time)
{
    int sec;    
    datetime_t past = { RTC_YEAR_START, 0, 0, 0, 0, 0 };
    datetime_t now = { time->tm_year, 
        time->tm_mon - RTC_MONTH_BASE, time->tm_mday - RTC_DAY_BASE,
        time->tm_hour, time->tm_min, time->tm_sec };


    // setting forbid
    if((time->tm_year < RTC_YEAR_START) 
        || (time->tm_year > (RTC_YEAR_START + 135))) {
        return 0;      
    }

    sec = (int)datetime_diff(&now, &past);


    //使能PWR和BKP外设时钟  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);    
    PWR_BackupAccessCmd(ENABLE);    //使能RTC和后备寄存器访问 
	RTC_EnterConfigMode();/// 允许配置  
	RTC_SetCounter(sec);   //设置RTC计数器的值
	RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成  
	RTC_ExitConfigMode(); //退出配置模式

    return 1;
}

int  rtc_get(rtc_t *rtc, rtc_time_t *time)
{
    datetime_t past = { RTC_YEAR_START, 0, 0, 0, 0, 0 };


    datetime_add(&past, RTC_GetCounter());

    time->tm_year = past.year;
    time->tm_mon = past.mon + RTC_MONTH_BASE;
    time->tm_mday = past.day + RTC_DAY_BASE;
    time->tm_hour = past.hour;
    time->tm_min = past.min;
    time->tm_sec = past.sec;
    time->tm_wday = datetime_week(time->tm_year,time->tm_mon,time->tm_mday);
    time->tm_wday = (time->tm_wday+6) % 7;
	
    return 1;
}
