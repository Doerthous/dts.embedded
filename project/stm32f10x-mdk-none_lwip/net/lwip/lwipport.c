#include "lwip/mem.h"

void mem_init(void)
{
	#define MEM_POOL_SIZE 10240
	static uint8_t mem[MEM_POOL_SIZE];
	init_memory_pool(MEM_POOL_SIZE, mem);
}

void* mem_trim(void *mem, mem_size_t size)
{
  return mem;
}
