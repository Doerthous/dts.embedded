#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/lib/delay.h>
#include <dts/embedded/lib/timer.h>
#include <dts/embedded/lib/tick.h>
#include <enc28j60.h>
#include <dts_hal_timer.h>
extern dts_hal_timer_t tim1;
extern gpio_t led;
extern spi_t spi2;
extern spi_t sw_spi2;
extern enc28j60_t enc28j60;
extern void isr_thrd_init(void);

#include <dts_net_mem.h>
#include <dts/net/tcp.h>
#include <dts/net/udp.h>
#include <dts/net/ip.h>
#include <dts/net/arp.h>
#include <dts/net/ether_arp.h>
#include <dts/net/ether.h>
#include <dts/net/dblk.h>
#include <dts/net/icmp.h>
#include <dts_net_sys.h>

ether_t eth0 = {
	.interface = (void*)&enc28j60,
	.send = (size_t(*)(void *,uint8_t*,size_t))enc28j60_send,
	.recv = (size_t(*)(void *,uint8_t*,size_t))enc28j60_recv,
	.mac_address = { 0x68, 0x19, 0x78, 0x11, 0x12, 0x34 },
};

ip_t ip0 = {
	IP_IPv4_ADDR(10,0,0,2),
	IP_IPv4_MASK(255,255,255,0),
};


void enc28j60_recv_isr(enc28j60_t *e)
{
	uint8_t *buff = rt_malloc(2048);
	ether_frame_t frame;
	
	memset(buff, 0, 2048);
	frame.data = buff;
	frame.data_size = 2048;
	ether_ll_recv(&eth0, &frame);
	rt_free(buff);
}

void dts_hal_default_handler(void *_, ...) {}


#include <rthw.h>
#include <rtthread.h>

#include "board.h"
rt_thread_t init_thread;

void rt_init_thread_entry(void* parameter)
{
	gpio_init(&led);
	
	spi_init(&spi2);
    enc28j60.spi = &spi2;
	enc28j60_init(&enc28j60);
	isr_thrd_init();
	interrupt_global_enable();

	dts_net_mem_init();
	
	ether_arp_resgiter_pair(&ip0, &eth0);
	
	timer_t tmr;
	timer_init(&tmr, rt_tick_get);
	timer_start(&tmr, 1000);
	while (1) {
		if (timer_expired(&tmr)) {
			timer_restart(&tmr);
			dts_net_sys_heartbeat();
		}
	}
}

int rt_application_init(void)
{
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 8, 20);

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    return 0;
}

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#elif __ICCARM__
#pragma section="HEAP"
#else
extern int __bss_end;
#endif

/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
    rt_kprintf("\n\r Wrong parameter value detected on\r\n");
    rt_kprintf("       file  %s\r\n", file);
    rt_kprintf("       line  %d\r\n", line);

    while (1) ;
}

/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
    /* init board */
    rt_hw_board_init();

    /* show version */
    rt_show_version();

#ifdef RT_USING_HEAP
#if STM32_EXT_SRAM
    rt_system_heap_init((void*)STM32_EXT_SRAM_BEGIN, (void*)STM32_EXT_SRAM_END);
#else
#if defined(__CC_ARM) || defined(__CLANG_ARM)
    rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)STM32_SRAM_END);
#elif __ICCARM__
    rt_system_heap_init(__segment_end("HEAP"), (void*)STM32_SRAM_END);
#else
    /* init memory system */
    rt_system_heap_init((void*)&__bss_end, (void*)STM32_SRAM_END);
#endif
#endif  /* STM32_EXT_SRAM */
#endif /* RT_USING_HEAP */

    /* init scheduler system */
    rt_system_scheduler_init();

    /* initialize timer */
    rt_system_timer_init();

    /* init timer thread */
    rt_system_timer_thread_init();

    /* init application */
    rt_application_init();

    /* init idle thread */
    rt_thread_idle_init();

    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return ;
}

int main(void)
{
    /* disable interrupt first */
    rt_hw_interrupt_disable();

    /* startup RT-Thread RTOS */
    rtthread_startup();

    return 0;
}

/*@}*/
