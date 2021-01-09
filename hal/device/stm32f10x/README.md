stm32f10x_hal

stm32f10x的hal, 与dts hal无关，可单独抽取成库。

spi文件夹内适配了dts hal spi接口。

使用时，将本目录设为包含目录。

## 添加新硬件

1. 在本目录中加`stm32f10x_hal_xxx.h`和`stm32f10x_hal_xxx.c`
2. 按照dts hal对应的接口，在`stm32f10x_hal_xxx.h`中声明xxx_t和stm32f10x_hal_xxx接口