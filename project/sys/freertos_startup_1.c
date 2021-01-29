#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <dts/eos.h>

extern thrd_t *init_thread;
void init_thread_entry(void* parameter);
	
int main(int argc, char const *argv[])
{
    SysTick_Config(SystemCoreClock / 1000);

    thrd_new(&init_thread, init_thread_entry, NULL, 1024);

    vTaskStartScheduler();

    while (1);

    return 0;
}
