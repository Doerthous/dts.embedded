#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/lib/delay.h>
#include <enc28j60.h>

#include <lwip/init.h>
#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/timeouts.h>
#include <lwip/etharp.h>
#include <netif/ethernet.h>
static struct netif netif;
	
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

extern void enc28j60_int_init(enc28j60_t *enc28j60);
extern void enc28j60_int_poll(void);
void enc28j60_recv_isr(enc28j60_t *e)
{
	struct pbuf *buff = pbuf_alloc(PBUF_RAW, 2048, PBUF_RAM);
	buff->len = enc28j60_recv(e, buff->payload, 2048);
	if (buff->len) {
		netif.input(buff, &netif);
	}
}

static uint32_t now;
uint32_t sys_now()
{
	return now;
}

sys_prot_t sys_arch_protect(void)
{
	return 0;
}
void sys_arch_unprotect(sys_prot_t pval)
{
}
extern err_t ethernetif_init(struct netif *netif);

#include <dts/embedded/lib/timer.h>
#include <dts/embedded/lib/tick.h>

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
	enc28j60_init(&enc28j60);
	enc28j60_int_init(&enc28j60);
	interrupt_global_enable();

	
	/* Set network address variables */
	ip_addr_t ipaddr, netmask, gw;
	IP4_ADDR(&gw, 10,0,0,1);
	IP4_ADDR(&ipaddr, 10,0,0,2);
	IP4_ADDR(&netmask, 255,255,255,0);

	/* The lwIP single-threaded core: initialize the network stack */ 
	lwip_init();

	/* Bring up the network interface */
	/* Hint: netif_init(); was already called by lwip_init(); above */
	netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);
	/* ethhw_init() is user-defined */
	/* use ip_input instead of ethernet_input for non-ethernet hardware */
	/* (this function is assigned to netif.input and should be called by the hardware driver) */
	netif_set_default(&netif);
	netif_set_up(&netif);
	
	static timer_t tmr = { .tick = tick_ms, .interval = 1000, };
	while (1) {
		enc28j60_int_poll();
//		poll_driver(&netif);
//		(struct pbuf *p, struct netif *inp)
//		netif.input();

		if (timer_expired(&tmr)) {
			timer_restart(&tmr);
			now++;
		}
		
		sys_check_timeouts();
	}
	
	return 0;
}
