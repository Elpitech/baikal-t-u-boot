
#ifndef __GIC_MIPS_H__
#define __GIC_MIPS_H__

#include "sysreg.h"
#include "mem_map.h"

#ifdef USE_GIC
#define READ_GIC_REG(r)       READ_MEMORY_REG(r)
#define WRITE_GIC_REG(r, v)   WRITE_MEMORY_REG(r, v)
#else
#define READ_GIC_REG(r)         (0xBADCAFE)
#define WRITE_GIC_REG(r, v)
#endif /* USE_GIC */

#ifndef __ASSEMBLER__

#ifdef USE_GIC
/* 
 * TEST_GIC_REG_ID macro returns 0 if the reg register is to equal to
 * its reg_val default value.
 */
#define TEST_GIC_REG_ID TEST_MEM_REG_ID

/* 
 * TEST_GIC_READ macro returns 0 if the reg register can be read.
 */
#define TEST_GIC_READ   TEST_READ_MEM_REG
#else
#define TEST_GIC_REG_ID
#define TEST_GIC_READ
#endif /* USE_GIC */

typedef void (*llenv_interrupt_handler_t) (unsigned int iar, int unsigned aiar);
extern llenv_interrupt_handler_t llenv_test_irq_trap;
extern llenv_interrupt_handler_t llenv_test_exc_trap;

void llenv32_gic_handler(unsigned int hw_irq, unsigned int sw_irq);

void llenv32_read_gic_cnt(uint32_t *hi, uint32_t *lo);
void llenv32_write_gic_cnt(uint32_t hi, uint32_t lo);
void llenv32_set_gic_comp(uint32_t off);

#ifndef BROM
#define LLENV32_SET_IRQ_HANDLER(x)      (llenv_test_irq_trap = x)
#define LLENV32_SET_EXC_HANDLER(x)      (llenv_test_exc_trap = x)
#else /* BROM */
#define LLENV32_SET_IRQ_HANDLER(x)
#define LLENV32_SET_EXC_HANDLER(x)
#endif /* BROM */
#endif /* __ASSEMBLER__ */


#define NUM_GIC_SGI     2	/* Quantity of SW interrupts. */
#define GROUP1_IRQ      0	/* Identify whether SW happens. */

/* Enable/Disable interrupts. */
#define MIPS_EI		asm volatile("ei; nop; ehb; nop")
#define MIPS_DI		asm volatile("di; nop; ehb; nop")

#define GIC_SHARE_OFFSET	0x00000	/* Shared Section Offset. */
#define GIC_LOCAL_OFFSET	0x08000 /* Core-Local Section Offset. */
#define GIC_OTHER_OFFSET	0x0C000 /* Core-Other Section Offset. */
#define GIC_USER_OFFSET		0x10000 /* User-Mode Visible Section Offset. */

#define GIC_SHARE_BASE		(GIC_BASE + GIC_SHARE_OFFSET)
#define GIC_LOCAL_BASE		(GIC_BASE + GIC_LOCAL_OFFSET)
#define GIC_OTHER_BASE		(GIC_BASE + GIC_OTHER_OFFSET)
#define GIC_USER_BASE		(GIC_BASE + GIC_USER_OFFSET)

/* Shared Section Register Map. */
#define GIC_SH_CONFIG		(GIC_SHARE_BASE + 0x0000)	/* GIC Config Register */
#define GIC_SH_CounterLo	(GIC_SHARE_BASE + 0x0010)	/* GIC CounterLo Shared Global Counter. */
#define GIC_SH_CounterHi	(GIC_SHARE_BASE + 0x0014)	/* GIC CounterHi */
#define GIC_RevisionID		(GIC_SHARE_BASE + 0x0020)	/* GIC Revision Register RevisionID of the GIC hardware. */
#define GIC_SH_COUNTER_LO	GIC_SH_CounterLo
#define GIC_SH_COUNTER_HI	GIC_SH_CounterHi
#define GIC_REVISION_ID		GIC_RevisionID
#define GIC_SH_POL31_0		(GIC_SHARE_BASE + 0x0100)	/* Global Interrupt Polarity Register0 Polarity of the interrupt. */
#define GIC_SH_POL63_32		(GIC_SHARE_BASE + 0x0104)	/* Global Interrupt Polarity Register1 */
#define GIC_SH_POL95_64		(GIC_SHARE_BASE + 0x0108)	/* Global Interrupt Polarity Register2 */
#define GIC_SH_POL127_96	(GIC_SHARE_BASE + 0x010c)	/* Global Interrupt Polarity Register3 */
#define GIC_SH_POL159_128	(GIC_SHARE_BASE + 0x0110)	/* Global Interrupt Polarity Register4 */
#define GIC_SH_POL191_160	(GIC_SHARE_BASE + 0x0114)	/* Global Interrupt Polarity Register5 */
#define GIC_SH_POL223_192	(GIC_SHARE_BASE + 0x0118)	/* Global Interrupt Polarity Register6 */
#define GIC_SH_POL255_224	(GIC_SHARE_BASE + 0x011c)	/* Global Interrupt Polarity Register7 */
#define GIC_SH_TRIG31_0		(GIC_SHARE_BASE + 0x0180)	/* Global Interrupt Trigger Type Register0.*/ 
#define GIC_SH_TRIG63_32	(GIC_SHARE_BASE + 0x0184)	/* Global Interrupt Trigger Type Register1 */
#define GIC_SH_TRIG95_64	(GIC_SHARE_BASE + 0x0188)	/* Global Interrupt Trigger Type Register2 */
#define GIC_SH_TRIG127_96	(GIC_SHARE_BASE + 0x018c)	/* Global Interrupt Trigger Type Register3 */
#define GIC_SH_TRIG159_128	(GIC_SHARE_BASE + 0x0190)	/* Global Interrupt Trigger Type Register4 */
#define GIC_SH_TRIG191_160	(GIC_SHARE_BASE + 0x0194)	/* Global Interrupt Trigger Type Register5 */
#define GIC_SH_TRIG223_192	(GIC_SHARE_BASE + 0x0198)	/* Global Interrupt Trigger Type Register6 */
#define GIC_SH_TRIG255_224	(GIC_SHARE_BASE + 0x019c)	/* Global Interrupt Trigger Type Register7 */
#define GIC_SH_DUAL31_0		(GIC_SHARE_BASE + 0x0200)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL63_32	(GIC_SHARE_BASE + 0x0204)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL95_64	(GIC_SHARE_BASE + 0x0208)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL127_96	(GIC_SHARE_BASE + 0x020c)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL159_128	(GIC_SHARE_BASE + 0x0210)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL191_160	(GIC_SHARE_BASE + 0x0214)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL223_192	(GIC_SHARE_BASE + 0x0218)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_DUAL255_224	(GIC_SHARE_BASE + 0x021c)	/* Global Interrupt Dual Edge Register */
#define GIC_SH_WEDGE		(GIC_SHARE_BASE + 0x0280)	/* Global Interrupt Write Edge Register */
#define GIC_SH_RMASK31_00	(GIC_SHARE_BASE + 0x0300)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK63_32	(GIC_SHARE_BASE + 0x0304)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK95_64	(GIC_SHARE_BASE + 0x0308)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK127_96	(GIC_SHARE_BASE + 0x030c)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK159_128	(GIC_SHARE_BASE + 0x0310)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK191_160	(GIC_SHARE_BASE + 0x0314)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK223_192	(GIC_SHARE_BASE + 0x0318)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_RMASK255_224	(GIC_SHARE_BASE + 0x031c)	/* Global Interrupt Reset Mask Register */
#define GIC_SH_SMASK31_00	(GIC_SHARE_BASE + 0x0380)	/* Global Interrupt Set Mask */
#define GIC_SH_SMASK63_32	(GIC_SHARE_BASE + 0x0384)	/* Global Interrupt Set Mask Register */
#define GIC_SH_SMASK95_64	(GIC_SHARE_BASE + 0x0388)	/* Global Interrupt Set Mask Register */
#define GIC_SH_SMASK127_96	(GIC_SHARE_BASE + 0x038c)	/* Global Interrupt Set Mask Register */
#define GIC_SH_SMASK159_128	(GIC_SHARE_BASE + 0x0390)	/* Global Interrupt Set Mask Register */
#define GIC_SH_SMASK191_160	(GIC_SHARE_BASE + 0x0394)	/* Global Interrupt Set Mask Register */
#define GIC_SH_SMASK223_192	(GIC_SHARE_BASE + 0x0398)	/* Global Interrupt Set Mask Register */
#define GIC_SH_SMASK255_224	(GIC_SHARE_BASE + 0x039c)	/* Global Interrupt Set Mask Register */
#define GIC_SH_MASK31_00	(GIC_SHARE_BASE + 0x0400)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK63_32	(GIC_SHARE_BASE + 0x0404)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK95_64	(GIC_SHARE_BASE + 0x0408)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK127_96	(GIC_SHARE_BASE + 0x040c)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK159_128	(GIC_SHARE_BASE + 0x0410)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK191_160	(GIC_SHARE_BASE + 0x0414)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK223_192	(GIC_SHARE_BASE + 0x0418)	/* Global Interrupt Mask Register */
#define GIC_SH_MASK255_224	(GIC_SHARE_BASE + 0x041c)	/* Global Interrupt Mask Register */
#define GIC_SH_PEND31_00	(GIC_SHARE_BASE + 0x0480)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND63_32	(GIC_SHARE_BASE + 0x0484)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND95_64	(GIC_SHARE_BASE + 0x0488)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND127_96	(GIC_SHARE_BASE + 0x048c)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND159_128	(GIC_SHARE_BASE + 0x0490)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND191_160	(GIC_SHARE_BASE + 0x0494)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND223_192	(GIC_SHARE_BASE + 0x0498)	/* Global Interrupt Pending Register */
#define GIC_SH_PEND255_224	(GIC_SHARE_BASE + 0x049c)	/* Global Interrupt Pending Register */
#define GIC_SH_MAP0_PIN		(GIC_SHARE_BASE + 0x0500)	/* Global Interrupt Map Src0 to Pin Register */
#define GIC_SH_MAP1_PIN		(GIC_SHARE_BASE + 0x0504)	/* Global Interrupt Map Src1 to Pin Register */
#define GIC_SH_MAP2_PIN		(GIC_SHARE_BASE + 0x0508)	/* Global Interrupt Map Src2 to Pin Register */
#define GIC_SH_MAP255_PIN	(GIC_SHARE_BASE + 0x08fc)	/* Global Interrupt Map Src255 to Pin Register */
#define GIC_SH_MAP0_CORE31_0	(GIC_SHARE_BASE + 0x2000)	/* Global Interrupt Map Src0 to Core Register */
#define GIC_SH_MAP1_CORE31_0	(GIC_SHARE_BASE + 0x2020)	/* Global Interrupt Map Src1 to Core Register */
#define GIC_SH_MAP2_CORE31_0	(GIC_SHARE_BASE + 0x2040)	/* Global Interrupt Map Src2 to Core Register */
#define GIC_SH_MAP255_CORE31_0	(GIC_SHARE_BASE + 0x3fe0)	/* Global Interrupt Map Src255 to Core Register */
#define GIC_VB_DINT_SEND	(GIC_SHARE_BASE + 0x6000)	/* DINT Send to Group Register */
#define GIC_SH_MAP_SPACER	0x20	/* Spacing between map registers. */

/* Core-Local Section Register Map. */

#define GIC_COREi_CTL_LOCAL		(GIC_LOCAL_BASE + 0x0000)	/* Local Interrupt Control Register */
#define GIC_COREi_PEND_LOCAL		(GIC_LOCAL_BASE + 0x0004)	/* Local Interrupt Pending Register */
#define GIC_COREi_MASK_LOCAL		(GIC_LOCAL_BASE + 0x0008)	/* Local Mask Register */
#define GIC_COREi_RMASK_LOCAL		(GIC_LOCAL_BASE + 0x000c)	/* Local Reset Mask Register */
#define GIC_COREi_SMASK_LOCAL		(GIC_LOCAL_BASE + 0x0010)	/* Local Set Mask Register */
#define GIC_COREi_WD_MAP_LOCAL		(GIC_LOCAL_BASE + 0x0040)	/* Local WatchDog Map-to-Pin Register */
#define GIC_COREi_COMPARE_MAP_LOCAL	(GIC_LOCAL_BASE + 0x0044)	/* Local GIC Counter/Compare Map-to-Pin Register */
#define GIC_COREi_TIMER_MAP_LOCAL	(GIC_LOCAL_BASE + 0x0048)	/* Local CPU Timer Map-to-Pin Register */
#define GIC_COREi_FDC_MAP_LOCAL		(GIC_LOCAL_BASE + 0x004c)	/* Local CPU Fast Debug Channel Map-to-Pin Register */
#define GIC_COREi_PERFCTR_MAP_LOCAL	(GIC_LOCAL_BASE + 0x0050)	/* Local Perf Counter Map-to-Pin Register */
#define GIC_COREi_SWInt0_MAP_LOCAL	(GIC_LOCAL_BASE + 0x0054)	/* Local SWInt0 Map-to-Pin Register */
#define GIC_COREi_SWInt1_MAP_LOCAL	(GIC_LOCAL_BASE + 0x0058)	/* Local SWInt1 Map-to-Pin Register */
#define GIC_COREi_OTHER_ADDR_LOCAL	(GIC_LOCAL_BASE + 0x0080)	/* Core-Other Addressing Register */
#define GIC_COREi_IDENT_LOCAL		(GIC_LOCAL_BASE + 0x0088)	/* Core-Local Identification Register */
#define GIC_COREi_WD_CONFIG0_LOCAL	(GIC_LOCAL_BASE + 0x0090)	/* Programmable/Watchdog Timer0 Config Register */
#define GIC_COREi_WD_COUNT0_LOCAL	(GIC_LOCAL_BASE + 0x0094)	/* Programmable/Watchdog Timer0 Count Register */
#define GIC_COREi_WD_INITIAL0_LOCAL	(GIC_LOCAL_BASE + 0x0098)	/* Programmable/Watchdog Timer0 Initial Count Register */
#define GIC_COREi_CompareLo_LOCAL	(GIC_LOCAL_BASE + 0x00A0)	/* CompareLo Register */
#define GIC_COREi_CompareHi_LOCAL	(GIC_LOCAL_BASE + 0x00A4)	/* CompareHi Register */
#define GIC_COREi_EICSS0_LOCAL		(GIC_LOCAL_BASE + 0x0100)	/* EIC Shadow Set for Interrupt Src0 */
#define GIC_COREi_EICSS1_LOCAL		(GIC_LOCAL_BASE + 0x0104)	/* EIC Shadow Set for Interrupt Src1 */
#define GIC_InterrCOREi_EICSS62_LOCAL	(GIC_LOCAL_BASE + 0x0108)	/* EIC Shadow Set for Interrupt Src2 through Src62 (0x01F8) */
#define GIC_COREi_EICSS63_LOCAL		(GIC_LOCAL_BASE + 0x01FC)	/* EIC Shadow Set for Interrupt Src63 */
#define GIC_VO_DINT_PART_LOCAL		(GIC_LOCAL_BASE + 0x3000)	/* Core-Local DINT Group Participate Register */
#define GIC_VL_DINT_PART_LOCAL		GIC_VO_DINT_PART_LOCAL
#define GIC_VO_BRK_GROUP_LOCAL		(GIC_LOCAL_BASE + 0x3080)	/* Core-Local DebugBreak Group Register */
#define GIC_VL_BRK_GROUP_LOCAL		GIC_VO_BRK_GROUP_LOCAL

/* Core-Other Section Register Map. */
#define GIC_COREi_CTL_OTHER		(GIC_OTHER_BASE + 0x0000)	/* Local Interrupt Control Register */
#define GIC_COREi_PEND_OTHER		(GIC_OTHER_BASE + 0x0004)	/* Local Interrupt Pending Register */
#define GIC_COREi_MASK_OTHER		(GIC_OTHER_BASE + 0x0008)	/* Local Mask Register */
#define GIC_COREi_RMASK_OTHER		(GIC_OTHER_BASE + 0x000c)	/* Local Reset Mask Register */
#define GIC_COREi_SMASK_OTHER		(GIC_OTHER_BASE + 0x0010)	/* Local Set Mask Register */
#define GIC_COREi_WD_MAP_OTHER		(GIC_OTHER_BASE + 0x0040)	/* Local WatchDog Map-to-Pin Register */
#define GIC_COREi_COMPARE_MAP_OTHER	(GIC_OTHER_BASE + 0x0044)	/* Local GIC Counter/Compare Map-to-Pin Register */
#define GIC_COREi_TIMER_MAP_OTHER	(GIC_OTHER_BASE + 0x0048)	/* Local CPU Timer Map-to-Pin Register */
#define GIC_COREi_FDC_MAP_OTHER		(GIC_OTHER_BASE + 0x004c)	/* Local CPU Fast Debug Channel Map-to-Pin Register */
#define GIC_COREi_PERFCTR_MAP_OTHER	(GIC_OTHER_BASE + 0x0050)	/* Local Perf Counter Map-to-Pin Register */
#define GIC_COREi_SWInt0_MAP_OTHER	(GIC_OTHER_BASE + 0x0054)	/* Local SWInt0 Map-to-Pin Register */
#define GIC_COREi_SWInt1_MAP_OTHER	(GIC_OTHER_BASE + 0x0058)	/* Local SWInt1 Map-to-Pin Register */
#define GIC_COREi_OTHER_ADDR_OTHER	(GIC_OTHER_BASE + 0x0080)	/* Core-Other Addressing Register */
#define GIC_COREi_IDENT_OTHER		(GIC_OTHER_BASE + 0x0088)	/* Core-Local Identification Register */
#define GIC_COREi_WD_CONFIG0_OTHER	(GIC_OTHER_BASE + 0x0090)	/* Programmable/Watchdog Timer0 Config Register */
#define GIC_COREi_WD_COUNT0_OTHER	(GIC_OTHER_BASE + 0x0094)	/* Programmable/Watchdog Timer0 Count Register */
#define GIC_COREi_WD_INITIAL0_OTHER	(GIC_OTHER_BASE + 0x0098)	/* Programmable/Watchdog Timer0 Initial Count Register */
#define GIC_COREi_CompareLo_OTHER	(GIC_OTHER_BASE + 0x00A0)	/* CompareLo Register */
#define GIC_COREi_CompareHi_OTHER	(GIC_OTHER_BASE + 0x00A4)	/* CompareHi Register */
#define GIC_COREi_EICSS0_OTHER		(GIC_OTHER_BASE + 0x0100)	/* EIC Shadow Set for Interrupt Src0 */
#define GIC_COREi_EICSS1_OTHER		(GIC_OTHER_BASE + 0x0104)	/* EIC Shadow Set for Interrupt Src1 */
#define GIC_InterrCOREi_EICSS62_OTHER	(GIC_OTHER_BASE + 0x0108)	/* EIC Shadow Set for Interrupt Src2 through Src62 (0x01F8) */
#define GIC_COREi_EICSS63_OTHER		(GIC_OTHER_BASE + 0x01FC)	/* EIC Shadow Set for Interrupt Src63 */
#define GIC_VO_DINT_PART_OTHER		(GIC_OTHER_BASE + 0x3000)	/* Core-Local DINT Group Participate Register */
#define GIC_VL_DINT_PART_OTHER		GIC_VO_DINT_PART_OTHER
#define GIC_VO_BRK_GROUP_OTHER		(GIC_OTHER_BASE + 0x3080)	/* Core-Local DebugBreak Group Register */
#define GIC_VL_BRK_GROUP_OTHER		GIC_VO_BRK_GROUP_OTHER

/* User-Mode Visible Section Register Map. */
#define GIC_SH_CounterLo_USER	(GIC_USER_BASE + 0x0000)	/* GIC CounterLo. Read-only alias for GIC Shared CounterLo. */
#define GIC_SH_CounterHi_USER	(GIC_USER_BASE + 0x0004)	/* GIC CounterHi. Read-only alias for GIC Shared CounterHi. */

/*Default values */
#define                  GIC_SH_CONFIG_VAL 	0x880f0001
#define                GIC_REVISION_ID_VAL 	0x00000400
#define                 GIC_SH_POL31_0_VAL 	0x00000000
#define                GIC_SH_POL63_32_VAL 	0x00000000
#define                GIC_SH_POL95_64_VAL 	0x00000000
#define               GIC_SH_POL127_96_VAL 	0x00000000
#define              GIC_SH_POL159_128_VAL 	0x00000000
#define              GIC_SH_POL191_160_VAL 	0x00000000
#define              GIC_SH_POL223_192_VAL 	0x00000000
#define              GIC_SH_POL255_224_VAL 	0x00000000
#define                GIC_SH_TRIG31_0_VAL 	0x00000000
#define               GIC_SH_TRIG63_32_VAL 	0x00000000
#define               GIC_SH_TRIG95_64_VAL 	0x00000000
#define              GIC_SH_TRIG127_96_VAL 	0x00000000
#define             GIC_SH_TRIG159_128_VAL 	0x00000000
#define             GIC_SH_TRIG191_160_VAL 	0x00000000
#define             GIC_SH_TRIG223_192_VAL 	0x00000000
#define             GIC_SH_TRIG255_224_VAL 	0x00000000
#define                GIC_SH_DUAL31_0_VAL 	0x00000000
#define               GIC_SH_DUAL63_32_VAL 	0x00000000
#define               GIC_SH_DUAL95_64_VAL 	0x00000000
#define              GIC_SH_DUAL127_96_VAL 	0x00000000
#define             GIC_SH_DUAL159_128_VAL 	0x00000000
#define             GIC_SH_DUAL191_160_VAL 	0x00000000
#define             GIC_SH_DUAL223_192_VAL 	0x00000000
#define             GIC_SH_DUAL255_224_VAL 	0x00000000
#define               GIC_SH_MASK31_00_VAL 	0x00000000
#define               GIC_SH_MASK63_32_VAL 	0x00000000
#define               GIC_SH_MASK95_64_VAL 	0x00000000
#define              GIC_SH_MASK127_96_VAL 	0x00000000
#define             GIC_SH_MASK159_128_VAL 	0x00000000
#define             GIC_SH_MASK191_160_VAL 	0x00000000
#define             GIC_SH_MASK223_192_VAL 	0x00000000
#define             GIC_SH_MASK255_224_VAL 	0x00000000
#define               GIC_SH_PEND31_00_VAL 	0xffffffff
#define               GIC_SH_PEND63_32_VAL 	0xffffffff
#define               GIC_SH_PEND95_64_VAL 	0xffffffff
#define              GIC_SH_PEND127_96_VAL 	0xffffffff
#define             GIC_SH_PEND159_128_VAL 	0x00000000
#define             GIC_SH_PEND191_160_VAL 	0x00000000
#define             GIC_SH_PEND223_192_VAL 	0x00000000
#define             GIC_SH_PEND255_224_VAL 	0x00000000
#define                GIC_SH_MAP0_PIN_VAL 	0x80000000
#define                GIC_SH_MAP1_PIN_VAL 	0x80000000
#define                GIC_SH_MAP2_PIN_VAL 	0x80000000
#define              GIC_SH_MAP255_PIN_VAL 	0x00000000
#define           GIC_SH_MAP0_CORE31_0_VAL 	0x00000000
#define           GIC_SH_MAP1_CORE31_0_VAL 	0x00000000
#define           GIC_SH_MAP2_CORE31_0_VAL 	0x00000000
#define         GIC_SH_MAP255_CORE31_0_VAL 	0x00000000
#define               GIC_VB_DINT_SEND_VAL 	0x00000000
#define            GIC_COREi_CTL_LOCAL_VAL 	0x00000001
#define           GIC_COREi_PEND_LOCAL_VAL 	0x00000000
#define           GIC_COREi_MASK_LOCAL_VAL 	0x0000007f
#define         GIC_COREi_WD_MAP_LOCAL_VAL 	0x40000000
#define    GIC_COREi_COMPARE_MAP_LOCAL_VAL 	0x80000000
#define      GIC_COREi_TIMER_MAP_LOCAL_VAL 	0x80000005
#define        GIC_COREi_FDC_MAP_LOCAL_VAL 	0x80000005
#define    GIC_COREi_PERFCTR_MAP_LOCAL_VAL 	0x80000005
#define     GIC_COREi_SWInt0_MAP_LOCAL_VAL 	0x80000000
#define     GIC_COREi_SWInt1_MAP_LOCAL_VAL 	0x80000000
#define     GIC_COREi_OTHER_ADDR_LOCAL_VAL 	0x00000000
#define          GIC_COREi_IDENT_LOCAL_VAL 	0x00000000
#define     GIC_COREi_WD_CONFIG0_LOCAL_VAL 	0x00000000
#define      GIC_COREi_WD_COUNT0_LOCAL_VAL 	0xffffffff
#define    GIC_COREi_WD_INITIAL0_LOCAL_VAL 	0xffffffff
#define      GIC_COREi_CompareLo_LOCAL_VAL 	0x00000000
#define      GIC_COREi_CompareHi_LOCAL_VAL 	0xffffffff
#define         GIC_COREi_EICSS0_LOCAL_VAL 	0x00000000
#define         GIC_COREi_EICSS1_LOCAL_VAL 	0x00000000
#define  GIC_InterrCOREi_EICSS62_LOCAL_VAL 	0x00000000
#define        GIC_COREi_EICSS63_LOCAL_VAL 	0x00000000
#define         GIC_VO_DINT_PART_LOCAL_VAL 	0x00000001
#define         GIC_VL_DINT_PART_LOCAL_VAL 	0x00000001
#define         GIC_VO_BRK_GROUP_LOCAL_VAL 	0x00000000
#define         GIC_VL_BRK_GROUP_LOCAL_VAL 	0x00000000
#define            GIC_COREi_CTL_OTHER_VAL 	0x00000001
#define           GIC_COREi_PEND_OTHER_VAL 	0x00000000
#define           GIC_COREi_MASK_OTHER_VAL 	0x0000007f
#define         GIC_COREi_WD_MAP_OTHER_VAL 	0x40000000
#define    GIC_COREi_COMPARE_MAP_OTHER_VAL 	0x80000000
#define      GIC_COREi_TIMER_MAP_OTHER_VAL 	0x80000005
#define        GIC_COREi_FDC_MAP_OTHER_VAL 	0x80000005
#define    GIC_COREi_PERFCTR_MAP_OTHER_VAL 	0x80000005
#define     GIC_COREi_SWInt0_MAP_OTHER_VAL 	0x80000000
#define     GIC_COREi_SWInt1_MAP_OTHER_VAL 	0x80000000
#define     GIC_COREi_OTHER_ADDR_OTHER_VAL 	0x00000000
#define          GIC_COREi_IDENT_OTHER_VAL 	0x00000000
#define     GIC_COREi_WD_CONFIG0_OTHER_VAL 	0x00000000
#define      GIC_COREi_WD_COUNT0_OTHER_VAL 	0xffffffff
#define    GIC_COREi_WD_INITIAL0_OTHER_VAL 	0xffffffff
#define      GIC_COREi_CompareLo_OTHER_VAL 	0x00000000
#define      GIC_COREi_CompareHi_OTHER_VAL 	0xffffffff
#define         GIC_COREi_EICSS0_OTHER_VAL 	0x00000000
#define         GIC_COREi_EICSS1_OTHER_VAL 	0x00000000
#define  GIC_InterrCOREi_EICSS62_OTHER_VAL 	0x00000000
#define        GIC_COREi_EICSS63_OTHER_VAL 	0x00000000
#define         GIC_VO_DINT_PART_OTHER_VAL 	0x00000001
#define         GIC_VL_DINT_PART_OTHER_VAL 	0x00000001
#define         GIC_VO_BRK_GROUP_OTHER_VAL 	0x00000000
#define         GIC_VL_BRK_GROUP_OTHER_VAL 	0x00000000

/* GIC_SH_CONFIG */
#define GIC_SH_CONFIG_COUNTSTOP		(1 << 28)	/* Setting this bit will stop GIC_CounterHi and GIC_CounterLo. */

#define GIC_NUM_CPUS	8	/* Number of CPUs provided by GIC. */

/* GIC_COREi_PEND bits. */
#define FDC_PEND	(1 << 6)	/* Fast Debug Channel interrupt. */
#define SWINT1_PEND	(1 << 5)	/* SW interrupt1. */
#define SWINT0_PEND	(1 << 4)	/* SW interrupt0. */
#define PERFCOUNT_PEND	(1 << 3)	/* Performance Counter. */
#define TIMER_PEND	(1 << 2)	/* CPU Timer interrupt. */
#define COMPARE_PEND	(1 << 1)	/* GIC Count/Compare interrupt. */
#define WD_PEND		(1 << 0)	/* WatchDog interrupt. */
#define SWINT_SHIFT	4

/* GIC_COREi_MASK bits. */
#define FDC_MASK	(1 << 6)	/* Fast Debug Channel interrupt. */
#define SWINT1_MASK	(1 << 5)	/* SW interrupt1. */
#define SWINT0_MASK	(1 << 4)	/* SW interrupt0. */
#define PERFCOUNT_MASK	(1 << 3)	/* Performance Counter. */
#define TIMER_MASK	(1 << 2)	/* CPU Timer interrupt. */
#define COMPARE_MASK	(1 << 1)	/* GIC Count/Compare interrupt. */
#define WD_MASK		(1 << 0)	/* WatchDog interrupt. */

/* GIC_COREi_CTL bits. */
#define EIC_ROUTABLE	(1 << 0)	/* EIC mode. */

/* GIC_COREi_*_MAP bits. */
#define MAP_TO_PIN	(1 << 31)	/* Map to a VPE interrupt pin. */
#define MAP_TO_NMI	(1 << 30)	/* Map to a VPE NMI interrupt pin. */
#define MAP_SHIFT	(0)
#define MAP_BITS	(6)

/* GIC_COREi_WD_CONFIG0_LOCAL */
#define WDRESET		(1 << 7)	/* Reset bit */
#define WDINTR		(1 << 6)	/* Interrupt status */
#define WDWAIT		(1 << 5)	/* Stop countdown if the bit is zero. */
#define WDDEBUG		(1 << 4)	/* Stop countdown if the bit iz zero. */
#define WDTYPE_SHIFT	(1)		/* Types of modes*/
#define WDTYPE_POS	WDTYPE_SHIFT
#define WDTYPE_BITS	(3)
#define WDTYPE_ONE_TRIP	0		/* WD One Trip Mode, asserts interrupts and stops. */
#define WDTYPE_SEC_CNT	1		/* WD Second Countdown Mode, asserts reset on all the cores. */
#define WDTYPE_PIT	2		/* PIT mode - programmable interval timer. */
#define WDSTART		(1 << 0)	/* WD timer stop/start. */

/* GIC_SH_TRIG */
#define GIC_SH_TRIG_LEVEL	0	/* Level trigger type. */
#define GIC_SH_TRIG_EDGE	1	/* Edge trigger type. */

/* GIC_SH_POL */
#define GIC_SH_POL_LOW		0	/* Active Low; Falling Edge */
#define GIC_SH_POL_HIGH		1	/* Active High; Rising Edge */

/* Mask/unmask the HW interrupts in the CP0_STATUS register. */
#define GIC_DISABLE_IRQ(x)	__GIC_DISABLE_IRQ(x, CP0_STATUS_IM_HW_SHIFT)
#define GIC_ENABLE_IRQ(x)	__GIC_ENABLE_IRQ(x, CP0_STATUS_IM_HW_SHIFT)

/* Mask/unmask the SW interrupts in the CP0_STATUS register. */
#define GIC_DISABLE_SW_IRQ(x)	__GIC_DISABLE_IRQ(x, CP0_STATUS_IM_SW_SHIFT)
#define GIC_ENABLE_SW_IRQ(x)	__GIC_ENABLE_IRQ(x, CP0_STATUS_IM_SW_SHIFT)

#define IS_EIC_MODE	1
//#define IS_EIC_MODE	(READ_SYSREG(MIPS_CP0_CONFIG3) & CP0_CONFIG3_VEIC)

#define GIC_EIC_MAX_IRQS	63

#define __GIC_DISABLE_IRQ(x, m) do {			\
	volatile unsigned int reg;			\
	reg = READ_SYSREG(MIPS_CP0_STATUS);		\
	if (IS_EIC_MODE) {				\
		reg |= (GIC_EIC_MAX_IRQS << (m));	\
	} else {					\
		reg &= ~((x) << (m));			\
	}						\
	WRITE_SYSREG(reg, MIPS_CP0_STATUS);		\
} while (0)

#define __GIC_ENABLE_IRQ(x, m) do {			\
	volatile unsigned int reg;			\
	reg = READ_SYSREG(MIPS_CP0_STATUS);		\
	if (IS_EIC_MODE) {				\
		reg &= ~((CP0_STATUS_IM_HW_MASK)	\
			| (CP0_STATUS_IM_SW_MASK));	\
	} else {					\
		reg |= ((x) << (m));			\
	}						\
	WRITE_SYSREG(reg, MIPS_CP0_STATUS);		\
} while (0)

#endif /* __GIC_MIPS_H__ */
