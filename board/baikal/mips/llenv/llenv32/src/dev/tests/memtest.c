/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2015-2017 Baikal Electronics.
 *
 * Author: Ekaterina Skachko <Ekaterina.Skachko@baikalelectronics.ru>
 * Contributor: Georgy Vlasov <Georgy.Vlasov@baikalelectronics.ru> {addrcheck}
 *
 * Description: Memory tests.
 */

#include "llenv.h"
#include "mem_map.h"
#include "sysreg.h"
#include "mmu.h"
#include "ddr.h"


/* Declare test suite. */
#define TSUITE memtest
DEC_TESTSUITE(TSUITE, "memtest");

/* Declare all test cases: */
DEC_TESTCASE(TSUITE, movinv_0, "movinv0");
DEC_TESTCASE(TSUITE, movinv_1, "movinv1");
DEC_TESTCASE(TSUITE, movinv_walk, "movinv_walk");
DEC_TESTCASE(TSUITE, addrcheck, "addrcheck");
#ifndef UBOOT
/*
 * Some functions are redundant in UBOOT-lib
 */
DEC_TESTCASE(TSUITE, modulo_20, "MODULO_20");
#endif
/*
 * Add test cases to run. All test cases should be specified
 * between BEGIN_TCASE_LIST and END_TCASE_LIST.
 */
BEGIN_TCASE_LIST(TSUITE)
ADD_TCASE_TO_LIST(TSUITE, movinv_0)
ADD_TCASE_TO_LIST(TSUITE, movinv_1)
ADD_TCASE_TO_LIST(TSUITE, movinv_walk)
ADD_TCASE_TO_LIST(TSUITE, addrcheck)
#ifndef UBOOT
/*
 * Some functions are redundant in UBOOT-lib
 */
ADD_TCASE_TO_LIST(TSUITE, modulo_20)
#endif
END_TCASE_LIST;

#define REPEAT8(com)	com; com; com; com; \
			com; com; com; com;

#ifdef MMU_CACHEABLE
#ifdef COHERENCE_ENABLE
#define MMU_FLAGS	TLB_MASK_CACHED_COH
#else /* COHERENCE_ENABLE */
#define MMU_FLAGS	TLB_MASK_CACHED_NONCOH
#endif /* COHERENCE_ENABLE */
#else /* MMU_CACHEABLE */
#define MMU_FLAGS	TLB_MASK_UNCACHED
#endif /* MMU_CACHEABLE */
int movinv(llenv_arch_t start_addr, int size, llenv_arch_t pattern);
int modulo(int offset, llenv_arch_t start_addr, int size, llenv_arch_t pattern);

static int writememory(llenv_arch_t start_addr, int size, uint32_t highphy_addr);
static int checkmemory(llenv_arch_t start_addr, int size, uint32_t highphy_addr);

extern llenv_memory_map_t llenv_memory_map;

int which_rank(llenv_arch_t addr)
{
	int bit_num = READ_DDRC_REG(DDRC_ADDRMAP0) + 6;


	if (addr & (1 << bit_num))
		return 1;
	else
		return 0;
}

void parse_error(llenv_arch_t data, llenv_arch_t pattern)
{

	int i;
	llenv_arch_t mask;

	debug_msg("Faulty bits : ");
	for (i = 0; i < 32; i++) {
		mask = (1 << i);
		if ((data & mask) != (pattern & mask))
			debug_msg("%d ",i);
	}
	debug_msg("\n");

}

DEC_TESTSUITE_INIT(TSUITE)
{
	return 0;
}

DEC_TESTSUITE_RUN(TSUITE)
{
	/* Use default handler. */
	return llenv_default_run_testsuite(&TSUITE_STRUCT(memtest), TSUITE_LIST(memtest), tc_name);
}

DEC_TESTSUITE_INFO(TSUITE)
{
	return;
}

DEC_TESTSUITE_TEARDOWN(TSUITE)
{
	debug_msg("\n TEARDOWN \n");
	return 0;
}

#ifdef UBOOT
unsigned long getenv_ulong(const char *name, int base, unsigned long default_val);
uint32_t get_ddr_highmem_size(void);
uint64_t ddr_high_size;
uint64_t high_phyaddr;
uint64_t high_psize;
uint32_t low_phyaddr;
uint32_t low_psize;


uint32_t check_memory_range(uint64_t phyaddr, uint64_t psize, uint64_t def_phyaddr, uint64_t def_psize){
    /* Size of high addr should be same or smaller than value from ddr spd flash */
        if (((phyaddr + psize) > (def_phyaddr + (def_psize))) && (phyaddr > def_phyaddr)){
		debug_msg("\nEntered range is bigger than available memory space.\
		\nPlease input correct value. Default start addr of high memory region is 0x20000000, \
		\ndefault start addr of low memory region is 0x00000000, \
		\nsize of high memory automatically set at the start of system, \
		\nand save in memory_high_size uboot env variable.\
		\nSize of low memory is 0x08000000, you can choose different values in 0x0 - 0x08000000. \n");
		return 1;}
	else{
		return 0;}
}

uint32_t init_vars(){
	debug_msg("\nMemory ranges:");
	ddr_high_size = get_ddr_highmem_size(); /* B */
	high_phyaddr = getenv_ulong("memory_high_paddr",16,0);
	debug_msg("\nmemory_high_paddr = %llx",high_phyaddr);
	high_psize = (uint64_t)getenv_ulong("memory_high_size",16,0); /* B */
	debug_msg("\nmemory_high_size = %llx",high_psize);
	low_phyaddr = getenv_ulong("memory_low_paddr",16,0);
	debug_msg("\nmemory_low_paddr = %x",low_phyaddr);
	low_psize = getenv_ulong("memory_low_size",16,0);
	debug_msg("\nmemory_low_size = %x\n",low_psize);
}
#endif

DEC_TESTCASE_RUN(TSUITE, movinv_0)
{
	llenv_arch_t pattern;
	int res = 0;
	uint32_t mem;
	int size;
	int full_size;
#ifdef ARCH_MIPS
	const llenv_memory_map_t tmp_map  = llenv_memory_map;
	uint32_t high_addr = MEMORY_HIGH_VADDR;
	uint32_t high_size = MEMORY_HIGH_SIZE;

#ifdef UBOOT
	init_vars();
	if (check_memory_range(high_phyaddr, high_psize, MEMORY_HIGH_PADDR, ddr_high_size)){
		return 1;}

	if (check_memory_range(low_phyaddr, low_psize,GET_PADDR(MEMORY_LOW_VADDR), MEMORY_LOW_SIZE)){
		return 1;}

	llenv_memory_map_t exc1_map = {high_phyaddr ,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#else
	llenv_memory_map_t exc1_map = {MEMORY_HIGH_PADDR,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#endif

	llenv_memory_map_t * ptr_map = &llenv_memory_map;
	mem = MEMORY_LOW_VADDR;
#ifndef UBOOT
	size = MEMORY_SIZE;
	pattern = 0xAAAAAAAA;
#else
       /* For now the lowest part u-boot relocates to DRAM is STACK
          so let's just take address of the lowest stack variable and
          check DRAM up to it (save another 4K for the next func)*/

        void *stack_pointer;
	mem = LEGACY_KSEG3 | low_phyaddr;
	size = (LEGACY_KSEG3 | (int)&stack_pointer) - mem - 0x1000;
	pattern = 0xAAAAAAAA;
#endif

	debug_msg("\n*** Test inspired by MEMTEST86 MOVING INVERSION algotithm ***\n");

	debug_msg("\nPhysycal range:         0x%x - 0x%x ", GET_PADDR(mem), GET_PADDR(mem + size - sizeof(llenv_arch_t)));
	debug_msg("rank #%d\n", which_rank(GET_PADDR(mem)));
	res = movinv(mem, size, pattern);
	full_size = size / 0x1000;
#ifdef UBOOT
	while (((exc1_map.paddr_start + MEMORY_HIGH_SIZE) < (high_phyaddr + high_psize)) && exc1_map.paddr_start) {
#else
	while (exc1_map.paddr_start){
#endif
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + high_size - sizeof(llenv_arch_t));
		debug_msg("rank #%d\n", which_rank(exc1_map.paddr_start));
		res += movinv(high_addr, high_size, pattern);
		exc1_map.paddr_start += high_size;
		full_size += high_size/0x1000;
	}
#ifdef UBOOT
	/* Check rest of memory */
	if (exc1_map.paddr_start){
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + (high_phyaddr + high_psize - exc1_map.paddr_start) - sizeof(llenv_arch_t));
		debug_msg("rank #%d\n", which_rank(exc1_map.paddr_start));
		res += movinv(high_addr, (high_phyaddr + high_psize - exc1_map.paddr_start), pattern);
	}
	/* End of checking */
#endif
	*ptr_map = tmp_map;
	llenv_flush_tlb();

#else /* ARCH_MIPS */
	mem = MEMORY_BASE;
	res = movinv(mem, MEMORY_SIZE, 0x0);
	full_size = MEMORY_SIZE / 1024;
#endif /* ARCH_MIPS */
	debug_msg("\nFully checked:		%dKB\n",full_size);
        debug_msg("\nDamaged cells:             %d\n", res);
	return res;
}

DEC_TESTCASE_RUN(TSUITE, movinv_1)
{
	int res;
	uint32_t mem;
	int size;
	int full_size;
	llenv_arch_t pattern;
#ifdef ARCH_MIPS
	const llenv_memory_map_t tmp_map  = llenv_memory_map;
	uint32_t high_addr = MEMORY_HIGH_VADDR;
	uint32_t high_size = MEMORY_HIGH_SIZE;

#ifdef UBOOT
	init_vars();
	if (check_memory_range(high_phyaddr, high_psize, MEMORY_HIGH_PADDR, ddr_high_size))
		return 1;

	if (check_memory_range(low_phyaddr, low_psize,GET_PADDR(MEMORY_LOW_VADDR), MEMORY_LOW_SIZE)){
		return 1;}

	llenv_memory_map_t exc1_map = {high_phyaddr ,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#else
	llenv_memory_map_t exc1_map = {MEMORY_HIGH_PADDR,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#endif

	llenv_memory_map_t * ptr_map = &llenv_memory_map;
        mem = MEMORY_LOW_VADDR;
#ifndef UBOOT
        size = MEMORY_SIZE;
        pattern = ~0x0;
#else
       /* For now the lowest part u-boot relocates to DRAM is STACK
          so let's just take address of the lowest stack variable and
          check DRAM up to it (save another 4K for the next func)*/
        void *stack_pointer;
	mem = LEGACY_KSEG3 | low_phyaddr;
        size = (LEGACY_KSEG3 | (int)&stack_pointer) - mem - 0x1000;
	pattern = 0x55555555;
#endif
        debug_msg("\n*** Test inspired by MEMTEST86 MOVING INVERSION algotithm ***\n");

	debug_msg("\nPhysycal range:         0x%x - 0x%x\n", GET_PADDR(mem), GET_PADDR(mem + size - sizeof(llenv_arch_t)));
	res = movinv(mem, size, pattern);
	full_size = size / 0x1000;


#ifdef UBOOT
	while (((exc1_map.paddr_start + MEMORY_HIGH_SIZE) < (high_phyaddr + high_psize)) && exc1_map.paddr_start) {
#else
	while (exc1_map.paddr_start){
#endif
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx\n", exc1_map.paddr_start, \
			exc1_map.paddr_start + high_size - sizeof(llenv_arch_t));
		res |= movinv(high_addr, high_size, 0x0);
		exc1_map.paddr_start += high_size;
		full_size += high_size/0x1000;
	}
#ifdef UBOOT
	/* Check rest of memory */
	if (exc1_map.paddr_start){
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + (high_phyaddr + high_psize - exc1_map.paddr_start) - sizeof(llenv_arch_t));
		debug_msg("rank #%d\n", which_rank(exc1_map.paddr_start));
		res += movinv(high_addr, (high_phyaddr + high_psize - exc1_map.paddr_start), pattern);
	}
	/* End of checking */
#endif
	*ptr_map = tmp_map;
	llenv_flush_tlb();
#else /* ARCH_MIPS */
	mem = MEMORY_BASE;
	res = movinv(mem, MEMORY_SIZE, ~0x0);
	full_size = MEMORY_SIZE / 0x1000;
#endif /* ARCH_MIPS */
	debug_msg("\nFully checked:		%dKB\n",full_size);
        debug_msg("\nDamaged cells:             %d\n", res);

	return res;
}

DEC_TESTCASE_RUN(TSUITE, movinv_walk)
{
	int res = 0;
	int pattern, word;
	uint32_t mem;
	int size;
#ifdef ARCH_MIPS
	const llenv_memory_map_t tmp_map  = llenv_memory_map;
	uint32_t high_addr = MEMORY_HIGH_VADDR;
	uint32_t high_size = MEMORY_HIGH_SIZE;
#ifdef UBOOT
	init_vars();
	if (check_memory_range(high_phyaddr, high_psize, MEMORY_HIGH_PADDR, ddr_high_size))
		return 1;

	if (check_memory_range(low_phyaddr, low_psize,GET_PADDR(MEMORY_LOW_VADDR), MEMORY_LOW_SIZE)){
		return 1;}

	llenv_memory_map_t exc1_map = {high_phyaddr ,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#else
	llenv_memory_map_t exc1_map = {MEMORY_HIGH_PADDR,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#endif
	llenv_memory_map_t * ptr_map = &llenv_memory_map;
        mem = MEMORY_LOW_VADDR;
#ifndef UBOOT
        size = MEMORY_SIZE;
#else
        /* For now the lowest part u-boot relocates to DRAM is STACK
           so let's just take address of the lowest stack variable and
           check DRAM up to it (save another 4K for the next func)*/
        void *stack_pointer;
	mem = LEGACY_KSEG3 | low_phyaddr;
        size = (LEGACY_KSEG3 | (int)&stack_pointer) - mem - 0x1000;
#endif
	debug_msg("\n*** Test inspired by MEMTEST86 MOVING INVERSION algotithm with a walking one***\n");

	for(pattern = 0x80; pattern; pattern >>= 1) {
		word = (pattern << 24) | (pattern << 16) |\
			(pattern << 8) | pattern;


		 debug_msg("\nPhysycal range:         0x%x - 0x%x\n",GET_PADDR(mem),GET_PADDR(mem + size - sizeof(llenv_arch_t)));

		/* Try walking ones */
		res = movinv(mem, size, word);
		/* Try walking zeroes */
		res = movinv(mem, size, ~word);


#ifdef UBOOT
	exc1_map.paddr_start = high_phyaddr;
	while (((exc1_map.paddr_start + MEMORY_HIGH_SIZE) < (high_phyaddr + high_psize)) && exc1_map.paddr_start) {
#else
	exc1_map.paddr_start = MEMORY_HIGH_PADDR;
	while (exc1_map.paddr_start){
#endif
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("Physycal range:         0x%llx - 0x%llx\n", exc1_map.paddr_start, \
			exc1_map.paddr_start + high_size - sizeof(llenv_arch_t));
		/* Try walking ones */
		res |= movinv(MEMORY_HIGH_VADDR, MEMORY_HIGH_SIZE, word);
		/* Try walking zeroes */
		res |= movinv(MEMORY_HIGH_VADDR, MEMORY_HIGH_SIZE, ~word);
		exc1_map.paddr_start += high_size;
	}
#ifdef UBOOT
	/* Check rest of memory */
	if (exc1_map.paddr_start){
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + (high_phyaddr + high_psize - exc1_map.paddr_start) - sizeof(llenv_arch_t));
		debug_msg("rank #%d\n", which_rank(exc1_map.paddr_start));
		res += movinv(high_addr, (high_phyaddr + high_psize - exc1_map.paddr_start), pattern);
	}
	/* End of checking */
#endif
	*ptr_map = tmp_map;
	llenv_flush_tlb();
	}
#else /* ARCH_MIPS */
        mem = MEMORY_BASE;
	for(pattern = 0x80; pattern; pattern >>= 1) {
                word = (pattern << 24) | (pattern << 16) |\
                        (pattern << 8) | pattern;
                /* Try walking ones */
                res |= movinv(mem, MEMORY_SIZE, word);
                /* Try walking zeroes */
                res |= movinv(mem, MEMORY_SIZE, ~word);
        }

#endif /* ARCH_MIPS */

	return res;
}

int movinv(llenv_arch_t start_addr, int size, llenv_arch_t pattern)
{
	llenv_arch_t *addr;
	short st = OK;

	debug_msg("set memory              pattern 0x%x\n", pattern);


	addr = (llenv_arch_t *)start_addr;
	while ((addr < (llenv_arch_t *)(start_addr + size))) {
		REPEAT8(*(addr++) = pattern);
	}

	debug_msg("backwards check mem for pattern 0x%x\n", pattern);
	debug_msg("set memory              pattern 0x%x\n",  ~pattern);

	for (addr = (llenv_arch_t *)(start_addr + size - sizeof(llenv_arch_t)); \
	     addr >= (llenv_arch_t *)start_addr; addr--) {
		if (*(addr) != pattern) {
			debug_msg("Error at addr 0x%x value = 0x%x\n", addr, *addr);
			parse_error(*addr, pattern);			parse_error(*addr, pattern);
			st++;
		}
		*(addr) = ~pattern;
	}

	debug_msg("straight check mem for  pattern 0x%x\n", ~pattern);

	for (addr = (llenv_arch_t *)start_addr; \
		addr <=(llenv_arch_t *)(start_addr + size - sizeof(llenv_arch_t)) && addr; \
		addr++) {
		if (*(addr) != ~pattern) {
			debug_msg("Error at addr 0x%x 0x%x\n", addr, *addr);
			parse_error(*addr, pattern);
			st++;
		}
	}

	debug_msg("Memory area has %d damaged cells\n", st);

	return st;
}

DEC_TESTCASE_RUN(TSUITE, addrcheck)
{
	int res = 0;
	uint32_t mem;
	int size;
	const llenv_memory_map_t tmp_map  = llenv_memory_map;
	uint32_t high_addr = MEMORY_HIGH_VADDR;
	uint32_t high_size = MEMORY_HIGH_SIZE;	
#ifdef UBOOT
	init_vars();
	if (check_memory_range(high_phyaddr, high_psize, MEMORY_HIGH_PADDR, ddr_high_size)){
		return 1;}

	if (check_memory_range(low_phyaddr, low_psize,GET_PADDR(MEMORY_LOW_VADDR), MEMORY_LOW_SIZE)){
		return 1;}

	llenv_memory_map_t exc1_map = {high_phyaddr ,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#else
	llenv_memory_map_t exc1_map = {MEMORY_HIGH_PADDR,   high_addr, \
		high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#endif /* UBOOT */
	llenv_memory_map_t * ptr_map = &llenv_memory_map;
	mem = MEMORY_BASE_UNMAP;
#ifndef UBOOT
	size = MEMORY_SIZE;
#else
       /* For now the lowest part u-boot relocates to DRAM is STACK
          so let's just take address of the lowest stack variable and
          check DRAM up to it (save another 4K for the next func)*/

        void *stack_pointer;
	mem = LEGACY_KSEG3 | low_phyaddr;
	size = (LEGACY_KSEG3 | (int)&stack_pointer) - mem - 0x1000;
#endif
	debug_msg("\n*** Address test of DDR ***\n");
	debug_msg("\nPhysycal range:         0x%x - 0x%x \n", GET_PADDR(mem), \
		GET_PADDR(mem + size - sizeof(llenv_arch_t)));
	res = writememory(mem, size,low_phyaddr);
	debug_msg("\nEnd of unmapped AREA\n");

#ifdef UBOOT
	while (((exc1_map.paddr_start + (uint64_t)MEMORY_HIGH_SIZE) < (high_phyaddr + high_psize)) && exc1_map.paddr_start) {
#else
	while (exc1_map.paddr_start){
#endif
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx \n", exc1_map.paddr_start, \
			exc1_map.paddr_start + high_size - sizeof(llenv_arch_t));
		res += writememory(high_addr, high_size, ((exc1_map.paddr_start << 1)>>1));
		exc1_map.paddr_start += high_size;
	}
#ifdef UBOOT
	/* Write rest of memory */
	if (exc1_map.paddr_start){
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx \n", exc1_map.paddr_start, \
			exc1_map.paddr_start + (high_phyaddr + high_psize - exc1_map.paddr_start) - sizeof(llenv_arch_t));
		res += writememory(high_addr,(uint32_t)(high_phyaddr + high_psize - exc1_map.paddr_start), ((exc1_map.paddr_start << 1)>>1));
	}
	/* End of writing */
#endif
	debug_msg("\nEnd of filling\n\n***Start Verifying***\n\nStart of unmapped AREA\n");
	llenv_flush_tlb();
	exc1_map.paddr_start = MEMORY_LOW_PADDR;
	exc1_map.vaddr_start = MEMORY_LOW_VADDR;
	exc1_map.size = MEMORY_LOW_SIZE;
	exc1_map.flags = MMU_FLAGS;
	*ptr_map = exc1_map;
	mem = LEGACY_KSEG3 | low_phyaddr;
	size = (LEGACY_KSEG3 | (int)&stack_pointer) - mem - 0x1000;
	debug_msg("\nPhysycal range:         0x%x - 0x%x\n", GET_PADDR(mem), \
		GET_PADDR(mem + size - sizeof(llenv_arch_t)));
	res  = checkmemory(mem, size, low_phyaddr);
	debug_msg("\nEnd of unmapped AREA\n\nStart of mapped AREA\n");

	high_addr = MEMORY_HIGH_VADDR;
 	high_size = MEMORY_HIGH_SIZE;
#ifdef UBOOT
	llenv_flush_tlb();
	exc1_map.paddr_start = high_phyaddr;
	exc1_map.vaddr_start = high_addr;
	exc1_map.size = high_size - sizeof(llenv_arch_t);
	exc1_map.flags = MMU_FLAGS;
	*ptr_map = exc1_map;
#else
	llenv_memory_map_t exc2_map = {MEMORY_HIGH_PADDR,   high_addr, \
		high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;
#endif /* UBOOT */
#ifdef UBOOT
	while (((exc1_map.paddr_start + (uint64_t)MEMORY_HIGH_SIZE) < (high_phyaddr + high_psize)) && exc1_map.paddr_start) {
#else
	while (exc1_map.paddr_start){
#endif
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + high_size - sizeof(llenv_arch_t));
		res += checkmemory(high_addr, high_size, ((exc1_map.paddr_start << 1)>>1));
		exc1_map.paddr_start += high_size;
	}
#ifdef UBOOT
	/* Check rest of memory */
	if (exc1_map.paddr_start){
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		debug_msg("\nPhysycal range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + (high_phyaddr + high_psize - exc1_map.paddr_start) - sizeof(llenv_arch_t));
		res += checkmemory(high_addr,(uint32_t)(high_phyaddr + high_psize - exc1_map.paddr_start), ((exc1_map.paddr_start << 1)>>1));
	}
	/* End of checking */
#endif

	*ptr_map = tmp_map;
	llenv_flush_tlb();

	debug_msg("\nEnd of mapped AREA\n");
	return res;
}

static int writememory(llenv_arch_t start_addr, int size, uint32_t highphy_addr)
{
	llenv_arch_t *addr;
	debug_msg("\n highphy_addr = %x",highphy_addr);
	int st = 0;
	addr = (llenv_arch_t *)start_addr;
	while ((addr < (llenv_arch_t *)(start_addr + size))) {
		for(int k = 0;k < 8;k++){
			*(addr) = highphy_addr;
			addr++;
			highphy_addr++;
		}
	}
	return st;
}

static int checkmemory(llenv_arch_t start_addr, int size, uint32_t highphy_addr)
{
	llenv_arch_t *addr;
	int st = 0;
	debug_msg("\n highphy_addr = %x",highphy_addr);
	addr = (llenv_arch_t *)start_addr;
	while ((addr < (llenv_arch_t *)(start_addr + size))) {
		for (int k = 0; k < 8; k++){
			if (*(addr) != highphy_addr) {
				st++;
				debug_msg("Error addr = %x val = 0x%x\n",(int) addr, *(addr));
			}
			addr++;
			highphy_addr++;
		}
	}
	debug_msg("\nMemory area has %d damaged cells\n", st);

	return st;
}

#ifndef UBOOT
/*
 * Some functions are redundant in UBOOT-lib
 */
DEC_TESTCASE_RUN(TSUITE, modulo_20)
{
	int res = 0;
	int pattern;
	int i;
	uint32_t mem;
        debug_msg("\n%s : Modulo 20 test with pattern = 0x%x.\n",__FUNCTION__, pattern);
#ifdef ARCH_MIPS
#ifdef MMU_CACHEABLE
	mem = 0x80000000;
#else /* MMU_CACHEABLE */
	mem = MEMORY_BASE_UNMAP;
#endif /* MMU_CACHEABLE */
	pattern = 0xA55AA55A;
	for(i = 0; i < 20; i++) {
		res |= modulo(i, mem, MEMORY_SIZE, pattern);
		res |= modulo(i, MEMORY_HIGH_VADDR, MEMORY_HIGH_SIZE, pattern);
	}
#else /* ARCH_MIPS */
        mem = MEMORY_BASE;
        pattern = 0xA55AA55A;
        for(i = 0; i < 20; i++) {
                res |= modulo(i, mem, MEMORY_SIZE, pattern);
        }
#endif /* ARCH_MIPS */
	return res;
}

int modulo(int offset, llenv_arch_t start_addr, int size, llenv_arch_t pattern)
{
	llenv_arch_t *addr;
	int i;
	int j;

	debug_msg("Start setting every 20th position of memory with offset %d from addr = 0x%x to 0x%x pattern = 0x%x\n", \
			offset, start_addr, start_addr + size, pattern);

	/* Fill every 20th position with pattern */
	for (addr = (llenv_arch_t *)start_addr + offset; addr < (llenv_arch_t *)(start_addr + size); addr += 20 ) {
		*addr = pattern;
	}
	debug_msg("Start setting all the rest of the memory pattern = 0x%x\n", ~pattern);
	/* Fill all the othe memory with complements */
	for (j = 0; j < 2; j++) {
		i = 0;
		for (addr = (llenv_arch_t *)start_addr; addr < (llenv_arch_t *)(start_addr + size); addr++) {

			if (i != offset) {
				*(addr) = ~pattern;
			}

			if (++i == 20)
				i = 0;
		}
	}

	debug_msg("Check every 20th position\n");
	/* Fill every 20th position with pattern */
	for (addr = (llenv_arch_t *)start_addr + offset; addr < (llenv_arch_t *)(start_addr + size); addr += 20 ) {
		if (*addr != pattern) {
			debug_msg("Error at addr = 0x%x\n", addr);
			return ERROR;
		}
	}
	debug_msg("Modulo 20 check with offset = %d OK\n", offset);

	return OK;
}
#endif /* UBOOT */
