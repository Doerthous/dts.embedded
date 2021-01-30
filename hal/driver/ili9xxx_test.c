#include <ili9xxx.h>
#include <dts/embedded/hal/lib/uart_printf.h>
#include <dts/embedded/lib/tick.h>

extern void ili9xxx_stm32f10x_io_init(ili9xxx_t *);
extern void ili9xxx_stm32f10x_fsmc_init(ili9xxx_t *);

void ili9xxx_test(void) {
    int i = 0, j = 0;
    int t;
    
    ili9xxx_t ili = { .md_init = ili9xxx_stm32f10x_fsmc_init, 
        .width = 240, .height = 320, };
    
    ili9xxx_init(&ili);

    ili9xxx_scan_order(&ili, 0, 1);

    t = tick_us();
    ili9xxx_fill(&ili, 0, 0, ili.width-1, 
                ili.height-1, 0xf0f0);
    printf("fill screen take time: %dus\n", tick_us()-t);
    
    for (i = 0; i < 100; ++i) {
        ili9xxx_set_pixel(&ili, i, 2*i, 0);
    }
    for (i = 100; i < 200; ++i) {
        ili9xxx_set_pixel(&ili, i, 2*i, 
            ili9xxx_get_pixel(&ili, i-100, 2*(i-100))-0x100);
    }
    for (i = 0; i < 200; ++i) {
        for (j = 400; j < 450; ++j) {
            ili9xxx_set_pixel(&ili, i, j, 0);
        }
    }
    for (i = 0; i < 200; ++i) {
        for (j = 0; j < 200; ++j) {
            ili9xxx_set_pixel(&ili, i, j, 0);
        }
    }
    ili9xxx_fill(&ili, 300,300,302,302,0);
    
    while (1);
}
