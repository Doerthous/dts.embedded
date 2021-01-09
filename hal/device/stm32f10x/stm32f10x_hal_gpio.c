/*
   The MIT License (MIT)

   Copyright (c) 2019 Doerthous

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   Authour: Doerthous <doerthous@gmail.com>
*/

#include <dts_hal_gpio.h>

#ifndef DTS_STM32F10X_HAL_GPIO
# include "stm32f10x_hal_i2c.h"
# define gpio_t stm32f10x_hal_gpio_t
# define gpio_init stm32f10x_hal_gpio_init
# define gpio_config stm32f10x_hal_gpio_config
# define gpio_write stm32f10x_hal_gpio_write
# define gpio_read stm32f10x_hal_gpio_read
#else
# include <dts/embedded/hal/gpio.h>
#endif // DTS_STM32F10X_HAL_GPIO


//# Debug
//#define DEBUG

static void gpio_clock_config(gpio_t *gpio, int enable)
{
  #if defined(USE_STDPERIPH_DRIVER)
    FunctionalState state = enable ? ENABLE : DISABLE;
    
    
    if (gpio->port == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, state);
        return;
    }
    if (gpio->port == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, state);
        return;
    }
    if (gpio->port == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, state);
        return;
    }
    if (gpio->port == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, state);
        return;
    }
    if (gpio->port == GPIOE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, state);
        return;
    }
    if (gpio->port == GPIOF) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, state);
        return;
    }
    if (gpio->port == GPIOG) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, state);
        return;
    }
  #else
    #error "gpio stm32f10x not support register operation yet!"
  #endif
}

void gpio_init(gpio_t *gpio)
{       
    // we first check whether other param of gpio is valid
    if (!IS_GPIO_SPEED(gpio->speed)) {
        gpio->speed = GPIO_Speed_2MHz; // default value
    }
    if (!IS_GPIO_MODE(gpio->mode)) {
        gpio->mode = GPIO_Mode_IPU; // default value
    }
    
    gpio_clock_config(gpio, 1);

    gpio_config(gpio, STM32F10X_HAL_GPIO_CMD_SET_MODE, gpio->mode);
    gpio_config(gpio, STM32F10X_HAL_GPIO_CMD_SET_SPEED, gpio->speed);    
}

#include <stdarg.h>
int gpio_config(gpio_t *gpio, int param, ...)
{
    int ret = -1;

  #if defined(USE_STDPERIPH_DRIVER)
    GPIO_InitTypeDef it;
    va_list args;
    
    
    it.GPIO_Mode = gpio->mode;
    it.GPIO_Pin = gpio->pin;
    it.GPIO_Speed = gpio->speed;
    
    va_start(args, param);  
    // set value
    switch (param) {
        case STM32F10X_HAL_GPIO_CMD_SET_MODE:
            gpio->mode = (GPIOMode_TypeDef)va_arg(args, int);
            it.GPIO_Mode = gpio->mode;
            GPIO_Init(gpio->port, &it);
            ret = 0;
            break;
        
        case STM32F10X_HAL_GPIO_CMD_SET_SPEED:
            gpio->speed = (GPIOSpeed_TypeDef)va_arg(args, int);
            it.GPIO_Speed = gpio->speed ;
            GPIO_Init(gpio->port, &it);
            ret = 0;
            break;
        case STM32F10X_HAL_GPIO_CMD_CLK_ON:
            gpio_clock_config(gpio, 1);
            ret = 0;
            break;
        case STM32F10X_HAL_GPIO_CMD_CLK_OFF:
            gpio_clock_config(gpio, 0);
            ret = 0;
            break;
    }
    va_end(args);

  #else
    #error "gpio stm32f10x not support register operation yet!"
  #endif

    return ret;
}

void gpio_write(gpio_t *gpio, int val)
{
    if (val) {
        gpio->port->ODR |= gpio->pin;
    }
    else {
        gpio->port->ODR &= ~gpio->pin;
    }
}
int gpio_read(gpio_t *gpio)
{
    return (gpio->port->IDR & gpio->pin) != 0;
}

void dts_hal_gpio_set(dts_hal_gpio_t *gpio)
{
    gpio->port->ODR |= gpio->pin;
}
void dts_hal_gpio_reset(dts_hal_gpio_t *gpio)
{
    gpio->port->ODR &= ~gpio->pin;
}
void dts_hal_gpio_clear(dts_hal_gpio_t *gpio)
{

    gpio->port->ODR &= ~gpio->pin;
}
void dts_hal_gpio_toggle(dts_hal_gpio_t *gpio)
{

    gpio_read(gpio) == 0 ? dts_hal_gpio_set(gpio) : dts_hal_gpio_reset(gpio);
}



// -----------------------------------------------------------------------------
//# GPIO Interrupt
#if STM32F10X_HAL_GPIO_INTERRUPT_SUPPORT == 1

#include <dts/embedded/hal/interrupt.h>

#define gpio_intr_enable dts_hal_gpio_intr_enable
#define gpio_intr_disable dts_hal_gpio_intr_disable
#define gpio_intr_set_handler dts_hal_gpio_intr_set_handler

#ifdef DEBUG
  extern gpio_t h;
  #define DEBUG_IO1 h
  #define GPIO_SET(g) gpio_set(&g)
  #define GPIO_RESET(g) gpio_reset(&g)
  #define GPIO_TOGGLE(g) gpio_toggle(&g)
#else
  #define GPIO_SET(g)
  #define GPIO_RESET(g)
  #define GPIO_TOGGLE(g)
#endif

#define EXTI_COUNT 11 // EXTI5-16
#define gpio_intr_hdlr_t void (*)(void *)


static void * extis_data[EXTI_COUNT];
static void (*extis_handler[EXTI_COUNT])(void *) =
{
    [0 ... EXTI_COUNT-1] = (gpio_intr_hdlr_t)dts_hal_default_handler,
};

// for dispatch exti 5~16
static void gpio_isr(void *data)
{
    GPIO_SET(DEBUG_IO1);

    #define EXTI_CASE(x) \
        if (EXTI->PR & (1 << x)) { \
            extis_handler[x-5](extis_data[x-5]); \
            EXTI->PR |= (1 << x); \
            GPIO_RESET(DEBUG_IO1); \
            return; \
        }
    if (EXTI->PR & 0x3E0) {
        EXTI_CASE(5);
        EXTI_CASE(6);
        EXTI_CASE(7);
        EXTI_CASE(8);
        EXTI_CASE(9);
    }
    else {
        EXTI_CASE(10);
        EXTI_CASE(11);
        EXTI_CASE(12);
        EXTI_CASE(13);
        EXTI_CASE(14);
        EXTI_CASE(15);        
    }
    #undef EXTI_CASE


    GPIO_RESET(DEBUG_IO1);
}


static dts_hal_irq_t gpio_irq(gpio_t *gpio)
{
    int j;
    
    
    j = 0;
    while (!(gpio->pin & (1 << j))) {
        j += 1;
    }
    
    while (1) {
        #define CASE(c) \
            if (j == c) { \
                return EXTI##c##_IRQn; \
            }
        #define RANGE_CASE(a,b) \
            if (a <= j && j <= b) { \
                return EXTI##b##_##a##_IRQn; \
            }
        CASE(0);
        CASE(1);
        CASE(2);
        CASE(3);
        CASE(4);
        RANGE_CASE(5, 9);
        RANGE_CASE(10, 15);
        #undef CASE
        #undef RANGE_CASE
    }
} 

void gpio_intr_enable(gpio_t *gpio)
{
    EXTI->PR |= gpio->pin;
    EXTI->IMR |= (gpio->pin);
}

void gpio_intr_disable(gpio_t *gpio)
{
    EXTI->IMR &= ~(gpio->pin);
}

void gpio_intr_set_handler(gpio_t *gpio,
    int trigger, void (*handler)(void *), void *data)
{
    int i, j;
    dts_hal_irq_t irq;
    

    i = gpio->pin;
    j = 0;
    while (!(i & 0x01)) {
        i >>= 1;
        j += 1;
    }

    //c enable clock first
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //c set trigger type
    if (trigger == DTS_HAL_GPIO_TRIGGER_FALLING) {
        EXTI->FTSR |= (1 << j);
        EXTI->RTSR &= ~(1 << j);
    }
    if (trigger == DTS_HAL_GPIO_TRIGGER_RISING) {
        EXTI->FTSR &= ~(1 << j);
        EXTI->RTSR |= (1 << j);
    }
    if (trigger == DTS_HAL_GPIO_TRIGGER_BOTH) {
        EXTI->FTSR |= (1 << j);
        EXTI->RTSR |= (1 << j);
    }    
    
    //c exti other configs
    GPIO_EXTILineConfig(((uint32_t)gpio->port-(uint32_t)(GPIOA))/0x400, j);

    //c interrupt
    irq = gpio_irq(gpio);
    
    if (irq == EXTI9_5_IRQn
        || irq == EXTI15_10_IRQn) {
        extis_data[j-5] = data;
        extis_handler[j-5] = handler == ((void *)0) ? 
            (gpio_intr_hdlr_t)dts_hal_default_handler : handler;
        interrupt_set_handler(irq, gpio_isr);
    }
    else {
        interrupt_set_handler(irq, handler);
        interrupt_set_handler_data(irq, data);
    }

    if (handler) {
        interrupt_enable(irq);
    }
    else {
        interrupt_disable(irq);
    }
}

#endif // DTS_HAL_GPIO_INTERRUPT
