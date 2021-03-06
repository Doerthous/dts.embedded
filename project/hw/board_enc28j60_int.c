#include <dts/embedded/hal/gpio.h>
#include <dts/embedded/hal/spi.h>
#include <dts/embedded/hal/interrupt.h>
#include <dts/embedded/lib/delay.h>
#include <enc28j60.h>

extern enc28j60_t enc28j60;
extern gpio_t enc28j60_int;
extern void enc28j60_recv_isr(enc28j60_t *e);

#ifndef EVENT_ENC28J60_INT
# define EVENT_SET_ENC28J60_INT pg6_intr = 1;
# define EVENT_CHECK_ENC28J60_INT pg6_intr
# define EVENT_RESET_ENC28J60_INT pg6_intr = 0;
static int pg6_intr;
#endif // EVENT_ENC28J60_INT
void pg6_intr_handler(void *data)
{
	EVENT_SET_ENC28J60_INT;
}

#define handle(enc28j60, eir) do \
{ \
	if (eir & ENC28J60_REG_EIR_TXIF) { \
		enc28j60_bit_field_clear(enc28j60,  \
			ENC28J60_REG_EIR, ENC28J60_REG_EIR_TXIF); \
	} \
	if (eir & ENC28J60_REG_EIR_PKTIF) { \
		enc28j60_recv_isr(enc28j60); \
		enc28j60_bit_field_clear(enc28j60,  \
			ENC28J60_REG_EIR, ENC28J60_REG_EIR_PKTIF); \
	} \
} while (0)

void enc28j60_int_poll(void) 
{
	if (EVENT_CHECK_ENC28J60_INT) {
		EVENT_RESET_ENC28J60_INT;
		enc28j60_isr(&enc28j60, handle);
	}
}

void enc28j60_int_init(enc28j60_t *enc28j60)
{
	gpio_init(&enc28j60_int);
	enc28j60_enable_interrupt(enc28j60, ENC28J60_REG_EIE_TXIE);
	enc28j60_enable_interrupt(enc28j60, ENC28J60_REG_EIE_PKTIE);
	enc28j60_enable_global_interrupt(enc28j60);
	
	gpio_intr_set_handler(&enc28j60_int, DTS_HAL_GPIO_TRIGGER_FALLING,
		pg6_intr_handler, enc28j60);
	gpio_intr_enable(&enc28j60_int);
	
	interrupt_enable(EXTI9_5_IRQn);
}
