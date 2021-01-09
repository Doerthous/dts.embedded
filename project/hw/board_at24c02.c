#include <dts/embedded/hal/i2c.h>
#include <at24cxx.h>

at24cxx_t at24c02 = 
{ 
    .page_size = 8, 
    .address = 0xA0, 
    .capacity = 256, 
};
