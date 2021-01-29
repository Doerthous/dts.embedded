#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
	
int main(int argc, char const *argv[])
{
    SysTick_Config(SystemCoreClock / 1000);

    void init_thrd_init(void);
    init_thrd_init();

    vTaskStartScheduler();

    while (1);

    return 0;
}
