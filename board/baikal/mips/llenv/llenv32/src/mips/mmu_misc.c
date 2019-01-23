/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Miscellaneous helper functions.
 */

#include "llenv.h"
#include "gic.h"
#include "sysreg.h"
#include "mmu.h"
//#define TLB_DEBUG
#ifdef MMU_CACHEABLE
#ifdef COHERENCE_ENABLE
#define MMU_FLAGS	TLB_MASK_CACHED_COH
#else /* COHERENCE_ENABLE */
#define MMU_FLAGS	TLB_MASK_CACHED_NONCOH
#endif /* COHERENCE_ENABLE */
#else /* MMU_CACHEABLE */
#define MMU_FLAGS	TLB_MASK_UNCACHED
#endif /* MMU_CACHEABLE */
/* Define the memory map. */
#ifdef BROM
const llenv_memory_map_t llenv_memory_map[4] = {
#else
llenv_memory_map_t llenv_memory_map[4] = {
#endif
	/* paddr,            vaddr,             size,             flags */
	{MEMORY_LOW_PADDR,   MEMORY_LOW_VADDR,  MEMORY_LOW_SIZE,  MMU_FLAGS},
	{MEMORY_HIGH_PADDR,  MEMORY_HIGH_VADDR, MEMORY_HIGH_SIZE, MMU_FLAGS},
	{0x20000000,	0x20000000,	0x80000000,	MMU_FLAGS},
	{DUMMY_ENTRY_P, 0, 0, 0}
};

/* MIPS miscellaneous functions. */

int msafpe_handler();

void llenv32_mem_map_info(void)
{
	int i;
	const llenv_memory_map_t *mm = llenv_memory_map;

	debug_msg("\n\tVirtual Address Memory Map:\n");
	debug_msg("\tPhysical   Virtual    Size\n");

	for (i = 0; mm[i].paddr_start != DUMMY_ENTRY_P; i++) {
		debug_msg("\t0x%llx 0x%08X 0x%08X\n", mm[i].paddr_start, mm[i].vaddr_start, mm[i].size);
	}
}

void llenv_init_irq(void)
{
	/* Interrupt enable. */
	MIPS_EI;

	/* Disable, SPI and I2C IRQ. */
#ifndef RTL
	GIC_DISABLE_IRQ(SPI_IRQ_0);
        GIC_DISABLE_IRQ(SPI_IRQ_1);
	GIC_DISABLE_IRQ(I2C_IRQ_0);
	GIC_DISABLE_IRQ(I2C_IRQ_1);
#endif /* RTL */
}

void llenv_disable_irq(void)
{
	/* Disable IRQ */
	MIPS_DI;
}

static void tlb_handler(void)
{
	uint32_t vaddr = DUMMY_ENTRY;
	uint64_t paddr = DUMMY_ENTRY_P;
	uint64_t pfn, pfn_odd, pfn_even; 
	uint32_t vfn, index, reg;
	int i;
	const llenv_memory_map_t *mm = llenv_memory_map;

	INIT2ZERO(vaddr); INIT2ZERO(reg); INIT2ZERO(pfn); INIT2ZERO(index); INIT2ZERO(i);

	/* Get the mapping virtual address. */
	vaddr = READ_SYSREG(MIPS_CP0_BAD_VADDR);

	for (i = 0; mm[i].paddr_start != DUMMY_ENTRY_P; i++) {
		if ((mm[i].vaddr_start <= vaddr) &&
				(vaddr < (mm[i].vaddr_start + mm[i].size))) {
			paddr = mm[i].paddr_start + (vaddr - mm[i].vaddr_start);
#ifdef TLB_DEBUG
			debug_msg("%s: vaddr=0x%x, paddr=0x%llx\n", __FUNCTION__, vaddr, paddr);
#endif
			break;
		}
	}

	if (paddr == DUMMY_ENTRY) {
		debug_msg("%s: physical address is not found for 0x%x\n", __FUNCTION__, vaddr);
		return;
	}

	/* Set the page size. */
	WRITE_SYSREG(CP0_TLB_PG_MASK, MIPS_CP0_TLB_PG_MASK);

	/* Set the virtual address match information. */
	reg = (vaddr & ((~0) << CP0_TLB_ENTRY_HI_VPN2_SHIFT));
#ifdef TLB_DEBUG
	debug_msg("%s: entry_hi = 0x%x\n", __FUNCTION__, reg);
#endif
	WRITE_SYSREG(reg, MIPS_CP0_TLB_ENTRY_HI);

	/* Calculate page frame number. */
	pfn = paddr;

	/* LOL. It works, and I have no bloody idea how... */
	pfn_odd = (((pfn | PAGE_SIZE) | (CP0_TLB_PG_MASK >> 1)) >> CP0_TLB_PFN_SHIFT);
	pfn_even = (((pfn & (~PAGE_SIZE)) | (CP0_TLB_PG_MASK >> 1)) >> CP0_TLB_PFN_SHIFT);

	/* Even page. */
	WRITE_SYSREG((((pfn_even) << CP0_TLB_LO_PFN_SHIFT) | mm[i].flags), MIPS_CP0_TLB_LO0);

	/* Odd page. */
	WRITE_SYSREG((((pfn_odd) << CP0_TLB_LO_PFN_SHIFT) | mm[i].flags), MIPS_CP0_TLB_LO1);

	/* Find the random TLB. */
	index = READ_SYSREG(MIPS_CP0_TLB_RANDOM);
	WRITE_SYSREG(index, MIPS_CP0_TLB_INDEX);
#ifdef TLB_DEBUG
	debug_msg("%s: index=%d lo0=0x%x, lo1=0x%x\n", __FUNCTION__, READ_SYSREG(MIPS_CP0_TLB_INDEX),
		READ_SYSREG(MIPS_CP0_TLB_LO0), READ_SYSREG(MIPS_CP0_TLB_LO1));
#endif
	/* Write the TLB. */
	MIPS_EHB;
	MIPS_TLBWI;
	MIPS_EHB;
}

void llenv_baikal_t_pre_init(void)
{
	/* Initialize the DDR memory space for the stack. */
	llenv32_memset((void *)(STACK_TOP - STACK_CORE_SIZE), STACK_CORE_SIZE / sizeof (int), 0xABABABAB);

	MIPS_EHB;
}

#ifndef RTL
/*
 * llenv32_install_hdl() is defined in trap_irq.S for RTL.
 */

void llenv32_install_hdl(void *addr_dst, void *addr_src, int size)
{
	unsigned char *s = (char *) addr_src;
	unsigned char *d = (char *) addr_dst;

	while (size--) {

		*d++ = *s++;
	}
}
#endif /* RTL */

void llenv_flush_tlb(void)
{
	int ftlb_size, vtlb_size, tlb_size;
	uint32_t config4;
#ifdef USE_FTLB
	/* Count FTLB entries if present */
	config4 = READ_SYSREG(MIPS_CP0_CONFIG4);
	if (((config4 >> CP0_CONFIG4_MMU_EXTDEF_POS) & \
		CP0_CONFIG4_MMU_EXTDEF_MASK) == CP0_CONFIG4_MMU_EXTDEF_SIZE) {
		/* Config4[7:0] used as MMUSizeExt. */
		ftlb_size = config4 & CP0_CONFIG4_MMU_SIZE_EXT_MASK;
	} else {
		/* Config4[3:0] - FTLB ways Config4[7:4] - FTLB sets */
		ftlb_size = CP0_CONFIG4_P5600_FTLB_SIZE;
		/* FTLB is organized as 128 sets and 4-ways
		   and no other options for P5600*/
	}
#else
	ftlb_size = 0;
#endif /* USE_FTLB */

	/* Count VTLB entries */
	vtlb_size = (READ_SYSREG(MIPS_CP0_CONFIG1) >> CP0_CONFIG1_MMUSIZE_SHIFT) &\
			CP0_CONFIG1_MMUSIZE_MASK;

	/* Flush entries */
	tlb_size = vtlb_size + ftlb_size;

	WRITE_SYSREG(0, MIPS_CP0_TLB_LO0);
	WRITE_SYSREG_HIGH(0, MIPS_CP0_TLB_LO0);
	WRITE_SYSREG(0, MIPS_CP0_TLB_LO1);
	WRITE_SYSREG_HIGH(0, MIPS_CP0_TLB_LO1);
	WRITE_SYSREG(CP0_TLB_PG_MASK, MIPS_CP0_TLB_PG_MASK);
	WRITE_SYSREG(CP0_TLB_ENTRY_HI_EHINV, MIPS_CP0_TLB_ENTRY_HI);
#ifdef TLB_DEBUG
	debug_msg("%s Flush %d entries of TLB\n",__FUNCTION__, tlb_size);
#endif
	do {
		WRITE_SYSREG(tlb_size--, MIPS_CP0_TLB_INDEX);
		MIPS_EHB;
		MIPS_TLBWI;

	} while (tlb_size);

	WRITE_SYSREG(0, MIPS_CP0_TLB_INDEX);
	WRITE_SYSREG(0, MIPS_CP0_TLB_ENTRY_HI);
}


void llenv_general_exc_handler(unsigned int iar, unsigned int excode)
{
        int reg;
#ifdef EXC_DEBUG
        debug_msg("excpode = 0x%x\n", excode);
#endif
        switch (excode) {
                case EXCODE_TLBM:
                case EXCODE_TLBS:
                case EXCODE_TLBL:
#ifdef TLB_DEBUG
                        debug_msg("EXC: EPC=0x%x ErrorEPC=0x%x\n",
                        READ_SYSREG(MIPS_CP0_EPC),
                        READ_SYSREG(MIPS_CP0_ERROR_EPC));
#endif
                        /* Update TLBs. */
                        tlb_handler();
                        break;
                case EXCODE_RI:
                        debug_msg("Reserved Instruction Exception.\n");
                        debug_msg("EXC: EPC=0x%x ErrorEPC=0x%x\n",
                                READ_SYSREG(MIPS_CP0_EPC),
                                READ_SYSREG(MIPS_CP0_ERROR_EPC));
                        debug_msg("status 0x%x\n", READ_SYSREG(MIPS_CP0_STATUS));
#ifdef SIMD_ENABLE
                                /* Enable SIMD : Enable FR bit in STATUS */
                        reg = READ_SYSREG(MIPS_CP0_STATUS);
                        WRITE_SYSREG(reg | (1 << 26), MIPS_CP0_STATUS);
#endif
                        MIPS_EHB;
                        break;
                case EXCODE_CPU:
                        debug_msg("Coprocessor Unusable exception.\n");
#ifdef SIMD_ENABLE
                        reg  = READ_SYSREG(MIPS_CP0_STATUS);
                        WRITE_SYSREG(reg | 0x30000000, MIPS_CP0_STATUS);
#endif
                        debug_msg("EXC: EPC=0x%x ErrorEPC=0x%x\n",
                                READ_SYSREG(MIPS_CP0_EPC),
                                READ_SYSREG(MIPS_CP0_ERROR_EPC));
                        break;
                case EXCODE_MSADIS:
                        debug_msg("Coprocessor MSA disable. Restore CONFIG5\n");
                        reg  = READ_SYSREG(MIPS_CP0_CONFIG5);
                        WRITE_SYSREG(reg | 0x8000000, MIPS_CP0_CONFIG5);
                        debug_msg("EXC: EPC=0x%x ErrorEPC=0x%x\n",
                                READ_SYSREG(MIPS_CP0_EPC),
                                READ_SYSREG(MIPS_CP0_ERROR_EPC));
                        break;
                case EXCODE_MSAFPE:
                        debug_msg("Coprocessor MSA Floating Point exception. Disable trapping\n");
                        msafpe_handler();
                        debug_msg("EXC: EPC=0x%x ErrorEPC=0x%x\n",
                                READ_SYSREG(MIPS_CP0_EPC),
                                READ_SYSREG(MIPS_CP0_ERROR_EPC));
                        break;
                default:
                        debug_msg("DEF_EXC: EPC=0x%x ErrorEPC=0x%x\n",
                                READ_SYSREG(MIPS_CP0_EPC),
                                READ_SYSREG(MIPS_CP0_ERROR_EPC));
                        debug_msg("DEF_EXC: BadVAddr=0x%x\n",
                                READ_SYSREG(MIPS_CP0_BAD_VADDR));
                        debug_msg("excode 0x%x\n", excode);
                        debug_msg("status 0x%x\n", READ_SYSREG(MIPS_CP0_STATUS));
        }
}
