/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: MMU header file.
 */

#ifndef __MMU_H__
#define __MMU_H__

#ifndef __ASSEMBLER__
#include <stdint.h>	/* for uint32_t */
#endif /* __ASSEMBLER__ */

#ifdef RTL
    #define tlb_miss_counter        (*((volatile int *)(SRAM_BASE)))
    #define test_buf            ((volatile int *)(SRAM_BASE + sizeof (int)))
#else /* RTL */
    extern volatile unsigned int tlb_miss_counter;
    extern volatile unsigned int test_buf[4];
#endif /* RTL */

typedef struct {
	uint64_t paddr_start;
	uint32_t vaddr_start;
	uint32_t size;
	int flags;
} llenv_memory_map_t;

typedef struct {
	uint32_t unmap_addr;
	uint32_t map_addr;
	uint32_t size;
} test_regions_t;

/* Prototypes */
int test_rw(int *raddr, int *waddr, int size);
void llenv_flush_tlb(void);

#define DUMMY_ENTRY	0xFFFFFFFF
#define DUMMY_ENTRY_P	0xFFFFFFFFFFFFFFFF

#endif /* __MMU_H__ */
