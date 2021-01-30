#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/lib/delay.h>
#include <dts/embedded/lib/timer.h>
#include <dts/embedded/lib/tick.h>
#include <enc28j60.h>
#include <dts_hal_timer.h>
extern dts_hal_timer_t tim1;
extern gpio_t led;
extern spi_t spi2;
extern uart_t uart1;
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

#include <dts/eos.h>

void enc28j60_recv_isr(enc28j60_t *e)
{
	uint8_t *buff = mem_alloc(2048);
	ether_frame_t frame;
	
	memset(buff, 0, 2048);
	frame.data = buff;
	frame.data_size = 2048;
	ether_ll_recv(&eth0, &frame);
	mem_free(buff);
}

void dts_hal_default_handler(void *_, ...) {}

thrd_t *init_thread;

#include <dts/embedded/hal/lib/uart_printf.h>
void init_thread_entry(void* parameter)
{
	gpio_init(&led);
	
	uart_init(&uart1);
	
	spi_init(&spi2);
    enc28j60.spi = &spi2;
	enc28j60_init(&enc28j60);
	isr_thrd_init();
	interrupt_global_enable();

	dts_net_mem_init();
	
	ether_arp_resgiter_pair(&ip0, &eth0);
	
	while (1) {
		thrd_sleep(tick_from_second(1));
		dts_net_sys_heartbeat();
		uart_printf(&uart1, "World!\n");
	}
}
thrd_t *t_thread;
void t_thrd_entry(void *_)
{
	while (1) {
		thrd_sleep(tick_from_second(1));
		uart_printf(&uart1, "Hello!\n");
	}
}
void init_thrd_init(void)
{
	thrd_new(&init_thread, init_thread_entry, NULL, 1024);
	thrd_new(&t_thread, t_thrd_entry, NULL, 1024);
}
