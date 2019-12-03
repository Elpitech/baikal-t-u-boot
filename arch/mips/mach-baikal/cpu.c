// SPDX-License-Identifier: GPL-2.0+
/*
 *
 */

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

#define USE_FTLB

#define P5600_FTLB_SIZE		512	/* default for p5600 */
#define MIPS_CONF1_TLBS_MASK	0x3f
#define TLB_PG_MASK		0x1000	/* for 4K page */

void flush_tlb(void)
{
	int ftlb_size, vtlb_size, tlb_size;
	uint32_t config4, reg;

#ifdef USE_FTLB
	/* enable FTLB */
	reg = read_c0_config6();
	reg &= ~(7 << 15);
	reg |= (5 << 15);
	write_c0_config6(reg);
	asm("ehb\n");

	/* Count FTLB entries if present */
	config4 = read_c0_config4();
	if ((config4 & MIPS_CONF4_MMUEXTDEF) == MIPS_CONF4_MMUEXTDEF_MMUSIZEEXT) {
		/* Config4[7:0] used as MMUSizeExt. */
		ftlb_size = config4 & MIPS_CONF4_MMUSIZEEXT;
	} else {
		/* Config4[3:0] - FTLB ways Config4[7:4] - FTLB sets */
		ftlb_size = P5600_FTLB_SIZE;
		/* FTLB is organized as 128 sets and 4-ways
		   and no other options for P5600*/
	}
#else
	ftlb_size = 0;
#endif /* USE_FTLB */

	/* Count VTLB entries */
	vtlb_size = (read_c0_config1() >> MIPS_CONF1_TLBS_SHIFT) & MIPS_CONF1_TLBS_MASK;

	/* Flush entries */
	tlb_size = vtlb_size + ftlb_size;

	write_c0_entrylo0(0);
	writex_c0_entrylo0(0);
	write_c0_entrylo1(0);
	writex_c0_entrylo1(0);
	write_c0_pagemask(TLB_PG_MASK);
	write_c0_entryhi(MIPS_ENTRYHI_EHINV);
#ifdef TLB_DEBUG
	debug_msg("%s Flush %d entries of TLB\n",__FUNCTION__, tlb_size);
#endif
	do {
		write_c0_index(tlb_size--);
		execution_hazard_barrier();
		tlb_write_indexed();

	} while (tlb_size >= 0);

	write_c0_entryhi(0);
}

int mach_cpu_init()
{
	flush_tlb();

	return 0;
}
