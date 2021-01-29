#include <dts/eos.h>

static thrd_t *isr_thrd;

#define bit(x) (1<<(x))
#define EVENT_ENC28J60_INT bit(0)
static uint32_t event;

#define EVENT_SET_ENC28J60_INT event|=EVENT_ENC28J60_INT; thrd_resume(isr_thrd);
#define EVENT_CHECK_ENC28J60_INT (event&EVENT_ENC28J60_INT)
#define EVENT_RESET_ENC28J60_INT event&=~EVENT_ENC28J60_INT
#include "../hw/board_enc28j60_int.c"


static void isr_thrd_entry(void *_)
{
	while (1) {
		thrd_suspend(isr_thrd);
		while (EVENT_CHECK_ENC28J60_INT) {
			EVENT_RESET_ENC28J60_INT;
			enc28j60_isr(&enc28j60, handle);
		}
	}
};

void isr_thrd_init(void)
{
	thrd_new(&isr_thrd, isr_thrd_entry, NULL, 1024);
	enc28j60_int_init(&enc28j60);
}	
