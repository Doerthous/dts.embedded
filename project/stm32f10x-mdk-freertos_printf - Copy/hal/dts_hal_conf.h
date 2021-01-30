#ifndef DTS_HAL_CONF_H_
#define DTS_HAL_CONF_H_

#define DTS_HAL_GPIO_INTERRUPT_ENABLE 1

#define DTS_HAL_LOCK_ENABLE
#ifdef DTS_HAL_LOCK_ENABLE
# include <dts_eos.h>
# define DTS_HAL_LOCK void *lock
# define dts_hal_lock_new(hal_obj) hal_obj->lock = dts_eos_mutex_new()
# define dts_hal_lock_delete(hal_obj) dts_eos_mutex_delete(hal_obj->lock)
# define dts_hal_lock_lock(hal_obj) dts_eos_mutex_lock(hal_obj->lock)
# define dts_hal_lock_unlock(hal_obj) dts_eos_mutex_unlock(hal_obj->lock)
# define DTS_HAL_UART_LOCK_ENABLE
#else
# define DTS_HAL_LOCK
# define dts_hal_lock_new(hal_obj)
# define dts_hal_lock_delete(hal_obj)
# define dts_hal_lock_lock(hal_obj)
# define dts_hal_lock_unlock(hal_obj)
#endif // DTS_HAL_LOCK_ENABLE

#endif // DTS_HAL_CONF_H_
