#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/lib/delay.h>
#include <enc28j60.h>

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

#include <mem/tlsf.h>
void *dts_net_malloc(size_t size)
{
	return tlsf_malloc(size);
}
void dts_net_free(void *ptr)
{
	tlsf_free(ptr);
}

void dts_hal_default_handler(void *_, ...) {}

extern gpio_t led;
extern spi_t spi2;
extern spi_t sw_spi2;
extern enc28j60_t enc28j60;
extern gpio_t enc28j60_int;

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

int pg6_intr;
void pg6_intr_handler(void *data)
{
	pg6_intr = 1;
}

void enc28j60_intr_handlr(enc28j60_t *enc28j60, uint8_t eir)
{
	if (eir & ENC28J60_REG_EIR_TXIF) {
		enc28j60_bit_field_clear(enc28j60, 
			ENC28J60_REG_EIR, ENC28J60_REG_EIR_TXIF);
	}
	if (eir & ENC28J60_REG_EIR_PKTIF) {
		static uint8_t buff[2048];
		ether_frame_t frame;
		
		memset(buff, 0, 2048);
		frame.data = buff;
		frame.data_size = 2048;
		ether_ll_recv(&eth0, &frame);
		
		enc28j60_bit_field_clear(enc28j60, 
			ENC28J60_REG_EIR, ENC28J60_REG_EIR_PKTIF);
	}
}

void isr_poll(void) 
{
	if (pg6_intr) {
		pg6_intr = 0;
		enc28j60_isr(&enc28j60, enc28j60_intr_handlr);
	}
}

#include <dts/embedded/lib/timer.h>
#include <dts/embedded/lib/tick.h>
extern int udp_server_test(void);
extern int udp_client_test(void);
extern int tcp_client_test(void);
extern int tcp_server_test(void);
extern int dts_bsd_socket_tcp_client_test(void);
extern int dts_bsd_socket_tcp_server_test(void);

#include <dts_hal_timer.h>
extern dts_hal_timer_t tim1;
void dts_hal_tick_init(dts_hal_timer_t *t);
int main(void)
{
	gpio_init(&led);
	
	dts_hal_timer_config(&tim1, STM32F10X_HAL_TIMER_CMD_CLK_ON);
	dts_hal_tick_init(&tim1);
	
	spi_init(&spi2);
    enc28j60.spi = &spi2;
	
	gpio_init(&enc28j60_int);
	enc28j60_init(&enc28j60);
	enc28j60_enable_interrupt(&enc28j60, ENC28J60_REG_EIE_TXIE);
	enc28j60_enable_interrupt(&enc28j60, ENC28J60_REG_EIE_PKTIE);
	enc28j60_enable_global_interrupt(&enc28j60);
	
	gpio_intr_set_handler(&enc28j60_int, DTS_HAL_GPIO_TRIGGER_FALLING,
		pg6_intr_handler, &enc28j60);
	gpio_intr_enable(&enc28j60_int);
	
	interrupt_enable(EXTI9_5_IRQn);
	interrupt_global_enable();

	#define MEM_POOL_SIZE 10240
	static uint8_t mem[MEM_POOL_SIZE];
	init_memory_pool(MEM_POOL_SIZE, mem);
	dts_net_mem_init();
	
	ether_arp_resgiter_pair(&ip0, &eth0);
	
	#define COROUTINE_CNT 2
	int (*coroutines[COROUTINE_CNT])(void) = {
		//tcp_client_test,
		udp_client_test,
		//udp_server_test,
		//tcp_server_test,
		//dts_bsd_socket_tcp_client_test,
		dts_bsd_socket_tcp_server_test,
	};
	int c = 0;
	while (1) {
		isr_poll();
		coroutines[c++%COROUTINE_CNT]();
		
		tcp_loop();
		
		static timer_t tmr = { .tick = tick_ms, .interval = 1000, };
		if (timer_expired(&tmr)) {
			timer_restart(&tmr);
			dts_net_sys_heartbeat();
		}
	}
	
	return 0;
}
