## DTS HAL

### 目录结构

```
device：不同硬件/芯片的HAL接口实现

driver：通过HAL接口实现的一些硬件驱动

include：HAL接口

lib：供HAL内部使用的库，可以在HAL外部使用，但是不建议。
```

### 如何使用

假设使用stm32f10x芯片，使用一个硬件SPI、一个硬件UART、一个硬件I2C和一个软件I2C。

1. 指定include文件夹为项目的include目录。

2. 指定device/stm32f10x，device/swsim为项目的include目录。

3. 指定device/stm32f10x/dts-hal/spi，device/stm32f10x/dts-hal/uart为项目的包含目录，用device/stm32f10x/stm32f10x_hal_spi.c和device/stm32f10x/stm32f10x_hal_uart.c作为费别作为SPI和UART模块的实现。

4. 指定device/object/i2c为项目的包含目录，用device/stm32f10x/stm32f10x_hal_i2c.c和device/swsim/swsim_hal_i2c.c作为I2C模块的实现。

5. 引入HAL实现所依赖的其他BSP源码。

### 添加新硬件接口

假设硬件模块命名为`xxx`

1. 在include目录中加`dts_hal_xxx.h`

2. 在include/dts/embedded/hal目录中加`xxx.h`

3. 在`dts_hal_xxx.h`定义模块接口，在`xxx.h`进行局部空间转换（具体内容可参考其他模块）


### 添加新实现

1. 如果是新硬件（芯片），则在device文件夹中以芯片型号新建文件夹。

2. 如果仅是新模块（假设为gpio），则到device下对应的子目录（假设为st8990）中，新建`st8990_hal_gpio.c`，`st8990_hal_gpio.h`，`dts-hal\gpio\dts_hal_gpio_t.h`和`dts-hal\gpio\dts_hal_gpio.c`

3. 填充上述新建的文件，具体内容参考已有模块。


### 模块接口与模块实现间的配置交互

**模块实现（implement）**通过尾缀为`_SUPPORT`的宏定义告知**模块接口（interface）**是否支持某些功能。**模块接口**通过配置文件中尾缀为`_ENABLE`的宏定义告知**模块实现（interface）**是否启用某些功能。

#### 实现向接口声明其所支持的功能

所有实现`DTS HAL`接口的模块都有dts_hal_xxx_t.h文件，在该文件中声明支持的功能。如stm32f10x gpio的`dts_hal_gpio_t.h`中定义`DTS_HAL_GPIO_INTERRUPT_SUPPORT`为`1`，表示支持GPIO中断功能。

#### 接口告知实现启（禁）用某些功能

所有实现`DTS HAL`接口的模块都有dts_hal_xxx.c，在该文件中包含dts_hal_conf.h，转换一次。如stm32f10x gpio的`dts_hal_gpio.c`

```C
...
#include <dts_hal_conf.h>
#if DTS_HAL_GPIO_INTERRUPT_ENABLE == 1 //< dts_hal_conf.h中定义
# define STM32F10X_HAL_GPIO_INTERRUPT_SUPPORT 1 //< 启（禁）用stm32f10x_hal_gpio.c中和GPIO中断相关的功能
#endif
#include <stm32f10x_hal_gpio.c>
...
```

### 单实现及多实现

通常来说，每个HAL模块都对应设备的某个硬件模块，这种情况只需要**单个实现**即可。但是某些某款可能有多种实现的方法，例如I2C、SPI等硬件设备，可以通过硬件自身提供的功能实现，也可以通过IO模拟的方式实现，特殊情况下，多种实现还需要能够并存于HAL中，对于这种**多实现**的情况，HAL采用了面向对象的方式进行设计。

当只需要单实现时，如只需要stm32f10x GPIO模块，则

1. 指定`device/stm32f10x`和`device/stm32f10x/dts-hal/gpio`为include路径

2. 使用`device/stm32f10x/gpio/dts_hal_gpio.c`作为GPIO模块源码。

当需要多实现时，如需要多种I2C模块（stm32f10x和swsim），则

1. 指定`device/object/i2c`，`device/stm32f10x`和`device/swsim`为include路径

2. 使用`device/stm32f10x/stm32f10x_hal_gpio.c`和`device/stm32f10x/swsim_hal_gpio.c`作为GPIO模块源码


### 全局空间和局部空间

全局空间：

```C
#include <dts_hal_gpio.h>
dts_hal_gpio_xxx(...);
```

局部空间：

```C
#include <dts/embedded/hal/gpio.h>
gpio_xxx(...);
```

注：实现某个模块时，暴露给HAL用户的.h文件中应使用全局空间，.c文件中可酌情使用局部空间。


### 配置接口接口

config接口采用变参函数实现，同一模块的不能实现可能有不同的配置参数，见`device/stm32f10x/stm32f10x_hal_uart_cmd.h`与`device/swsim/swsim_hal_uart_cmd.h`


### 软件模拟模块

某些模块通过利用HAL自身提供的接口模拟硬件功能，如swsim中的I2C，SPI，UART，PWM等。这些软件模块会依赖某些接口（如timer_set_frequency），但是某些硬件设备上可能不具备这种接口，所以需要对这些可能不存在的接口通过宏定义进行检验（如`DTS_HAL_TIMER_SET_CALLBACK_SUPPORT`），当依赖的接口不存在时，不能使用这些软件模块。