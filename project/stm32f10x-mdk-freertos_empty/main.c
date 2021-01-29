#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>

static TaskHandle_t AppTask_Handle = NULL;

static void AppTask(void *param)
{
    while (1) {
        vTaskDelay(500);
        vTaskDelay(500);        
    }
}

int main(int argc, char const *argv[])
{
    SysTick_Config(SystemCoreClock / 1000);

    if (xTaskCreate((TaskFunction_t)AppTask, (const char *)"AppTask",
        (uint16_t)512, (void *)NULL, (UBaseType_t)1, 
        (TaskHandle_t *)&AppTask_Handle) != pdPASS) {
        while (1);
    }

    vTaskStartScheduler();

    while (1);

    return 0;
}
