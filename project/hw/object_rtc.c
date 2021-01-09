#include <dts/embedded/hal/rtc.h>
#include <swsim_hal_rtc.h>
#include <stm32f10x_hal_rtc.h>

static DTS_HAL_RTC_METHOD_DEFINE(stm32f10x, \
    stm32f10x_hal_rtc_init, \
    stm32f10x_hal_rtc_config, \
    stm32f10x_hal_rtc_get, \
    stm32f10x_hal_rtc_set \
)
static stm32f10x_hal_rtc_t stm32f10x_rtc;
rtc_t rtc = 
{
    .obj = (void *)&stm32f10x_rtc,
    .method = &DTS_HAL_RTC_METHOD(stm32f10x),
};

static DTS_HAL_RTC_METHOD_DEFINE(swsim, \
    swsim_hal_rtc_init, \
    swsim_hal_rtc_config, \
    swsim_hal_rtc_get, \
    swsim_hal_rtc_set \
)
static swsim_hal_rtc_t swsim_rtc;
rtc_t sw_rtc = 
{
    .obj = (void *)&swsim_rtc,
    .method = &DTS_HAL_RTC_METHOD(swsim),
};
