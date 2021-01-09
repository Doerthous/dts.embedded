## Embedded Learning

### bsp

BSP source code provided by chip manufacturer, thiry party.

### os Directory

All kinds of RTOS, thiry party.

### hal Directory

An HAL library which is os-independent.

### project Directory

Lots of embedded project named by [chip]-[toolchain]-[os]-[title].

`app`: os-independent and hal-dependent app.

`hw`: hal hardware unit.

### else

gpio->led->uart->com model->i2c->at24cxx test->spi->wq25 test->

init 和 clk_on power_on 的区分