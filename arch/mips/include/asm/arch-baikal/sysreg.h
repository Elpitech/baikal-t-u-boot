/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: CP0 registers.
 */

#ifndef __SYSREG_MIPS_H__
#define __SYSREG_MIPS_H__

/* Define Coprocessor0 registers: Name, Register, Select. */

#define MIPS_CP0_TLB_INDEX          $0, 0
#define MIPS_CP0_TLB_RANDOM         $1, 0
#define MIPS_CP0_TLB_LO0            $2, 0
#define MIPS_CP0_TLB_LO1            $3, 0
#define MIPS_CP0_TLB_CONTEXT        $4, 0
#define MIPS_CP0_TLB_CONTEXT_CFG    $4, 1
#define MIPS_CP0_TLB_USER_LOCAL     $4, 2
#define MIPS_CP0_TLB_PG_MASK        $5, 0
#define MIPS_CP0_TLB_PG_GRAIN       $5, 1
#define MIPS_CP0_SEG_CTL0           $5, 2
#define MIPS_CP0_SEG_CTL1           $5, 3
#define MIPS_CP0_SEG_CTL2           $5, 4
#define MIPS_CP0_TLB_PW_BASE        $5, 5
#define MIPS_CP0_TLB_PW_FIELD       $5, 6
#define MIPS_CP0_TLB_PW_SIZE        $5, 7
#define MIPS_CP0_TLB_WIRED          $6, 0
#define MIPS_CP0_TLB_PW_CTL         $6, 6
#define MIPS_CP0_HWRENA             $7, 0
#define MIPS_CP0_BAD_VADDR          $8, 0
#define MIPS_CP0_BAD_INSTR          $8, 1
#define MIPS_CP0_BAD_INSTRP         $8, 2
#define MIPS_CP0_COUNT              $9, 0
#define MIPS_CP0_TLB_ENTRY_HI       $10, 0
#define MIPS_CP0_GUEST_CTL1         $10, 4
#define MIPS_CP0_GUEST_CTL2         $10, 5
#define MIPS_CP0_COMPARE            $11, 0
#define MIPS_CP0_GUEST_CTL0_EXT     $11, 4
#define MIPS_CP0_STATUS             $12, 0
#define MIPS_CP0_INTCTL             $12, 1
#define MIPS_CP0_SRSCTL             $12, 2
#define MIPS_CP0_GUEST_CTL0         $12, 6
#define MIPS_CP0_GT_OFFSET          $12, 7
#define MIPS_CP0_CAUSE              $13, 0
#define MIPS_CP0_EXC_PC             $14, 0
#define MIPS_CP0_EPC                MIPS_CP0_EXC_PC
#define MIPS_CP0_PRID               $15, 0
#define MIPS_CP0_EBASE              $15, 1
#define MIPS_CP0_CDMMBASE           $15, 2
#define MIPS_CP0_CMGCRBASE          $15, 3
#define MIPS_CP0_CONFIG0            $16, 0
#define MIPS_CP0_CONFIG1            $16, 1
#define MIPS_CP0_CONFIG2            $16, 2
#define MIPS_CP0_CONFIG3            $16, 3
#define MIPS_CP0_CONFIG4            $16, 4
#define MIPS_CP0_CONFIG5            $16, 5
#define MIPS_CP0_CONFIG6            $16, 6
#define MIPS_CP0_CONFIG7            $16, 7
#define MIPS_CP0_MAAR               $17, 1
#define MIPS_CP0_MAARI              $17, 2
#define MIPS_CP0_WATCH_LO           $18, 0
#define MIPS_CP0_WATCH_LO_1         $18, 1
#define MIPS_CP0_WATCH_LO_2         $18, 2
#define MIPS_CP0_WATCH_LO_3         $18, 3
#define MIPS_CP0_WATCH_HI           $19, 0
#define MIPS_CP0_WATCH_HI_1         $19, 1
#define MIPS_CP0_WATCH_HI_2         $19, 2
#define MIPS_CP0_WATCH_HI_3         $19, 3
#define MIPS_CP0_DEBUG              $23, 0
#define MIPS_CP0_TRACE_CONTROL      $23, 1
#define MIPS_CP0_TRACE_CONTROL2     $23, 2
#define MIPS_CP0_TRACE_DATA1        $23, 3
#define MIPS_CP0_TRACE_IBPC         $23, 4
#define MIPS_CP0_TRACE_DBPC         $23, 5
#define MIPS_CP0_DEPC               $24, 0
#define MIPS_CP0_TRACE_CONTROL3     $24, 2
#define MIPS_CP0_TRACE_DATA2        $24, 3
#define MIPS_CP0_PERF0_CTL          $25, 0
#define MIPS_CP0_PERF0_CNT          $25, 1
#define MIPS_CP0_PERF1_CTL          $25, 2
#define MIPS_CP0_PERF1_CNT          $25, 3
#define MIPS_CP0_PERF2_CTL          $25, 4
#define MIPS_CP0_PERF2_CNT          $25, 5
#define MIPS_CP0_PERF3_CTL          $25, 6
#define MIPS_CP0_PERF3_CNT          $25, 7
#define MIPS_CP0_ERRCTL             $26, 0
#define MIPS_CP0_CACHE_ERR          $27, 0
#define MIPS_CP0_TAG_LO1            $28, 0
#define MIPS_CP0_TAG_LO2            $28, 2
#define MIPS_CP0_DATA_LO1           $28, 1
#define MIPS_CP0_DATA_LO2           $28, 3
#define MIPS_CP0_L23_TAG_LO         $28, 4
#define MIPS_CP0_L23_DATA_LO        $28, 5
#define MIPS_CP0_TAG_HI1            $29, 0
#define MIPS_CP0_DATA_HI1           $29, 1
#define MIPS_CP0_L23_DATA_HI        $29, 5
#define MIPS_CP0_ITAG_LO            MIPS_CP0_TAG_LO1
#define MIPS_CP0_ITAG_HI            MIPS_CP0_TAG_HI1
#define MIPS_CP0_IDATA_LO           MIPS_CP0_DATA_LO1
#define MIPS_CP0_IDATA_HI           MIPS_CP0_DATA_HI1
#define MIPS_CP0_DTAG_LO            MIPS_CP0_TAG_LO2	/* the P5600 core does not implement the DTagHi register */
#define MIPS_CP0_DDATA_LO           MIPS_CP0_DATA_LO2	/* The P5600 core does not implement the DDataHi register. */
#define MIPS_CP0_ERROR_EPC          $30, 0
#define MIPS_CP0_DESAVE             $31, 0
#define MIPS_CP0_KSCRATCH1          $31, 2
#define MIPS_CP0_KSCRATCH2          $31, 3
#define MIPS_CP0_KSCRATCH3          $31, 4

/* Reset values of CP0 registers */

#define CP0_TLB_INDEX_VAL	0x0
#define CP0_TLB_RANDOM_VAL	0x0
#define CP0_TLB_CONTEXT_CFG_VAL	0x7FFFF0
#define CP0_TLB_PG_MASK_VAL	0x0
#define CP0_TLB_PG_GRAIN_VAL	0x0
#define CP0_TLB_PW_BASE_VAL	0x0
#define CP0_TLB_PW_FIELD_VAL	0x2
#define CP0_TLB_PW_SIZE_VAL	0x0
#define CP0_TLB_WIRED_VAL	0x0
#define CP0_TLB_PW_CTL_VAL	0x0
#define CP0_HWRENA_VAL		0x0
#define CP0_BAD_INSTR_VAL	0x1
#define CP0_TLB_ENTRY_HI_VAL_H	0x0
#define CP0_TLB_ENTRY_HI_VAL_L	0x0
#define CP0_GUEST_CTL1_VAL	0x0
#define CP0_GUEST_CTL2_VAL	0x0
#define CP0_GUEST_CTL0_EXT_VAL	0x40
#define CP0_STATUS_VAL		0x400004
#define CP0_STATUS_EXC_TEST_DEF	0x30000000
#define CP0_INTCTL_VAL		0x0
#define CP0_SRSCTL_VAL		0x0
#define CP0_GUEST_CTL0_VAL	0xC4C0080
#define CP0_GT_OFFSET_VAL	0x0
#define CP0_CAUSE_VAL		0x0
#define CP0_CAUSE_EXC_TEST_DEF	0x8800000
#define CP0_PRID_VAL		0x1A800
#define CP0_CDMMBASE_VAL	0x2
#define CP0_CMGCRBASE_VAL	0x2
#define CP0_MAAR_VAL_H		0x0
#define CP0_MAAR_VAL_L		0x0
#define CP0_MAARI_VAL		0x0
#define CP0_WATCH_LO_VAL	0x0
#define CP0_WATCH_HI_VAL_02	0x80000000
#define CP0_WATCH_HI_VAL_3	0x0
#define CP0_DEBUG_VAL		0x2030000
#define CP0_TRACE_CONTROL_VAL	0x0
#define CP0_TRACE_CONTROL2_VAL	0x0
#define CP0_TRACE_DATA1_VAL	0x0
#define CP0_TRACE_IBPC_VAL	0x0
#define CP0_TRACE_DBPC_VAL	0x0
#define CP0_TRACE_CONTROL3_VAL	0x20000100
#define CP0_TRACE_DATA2_VAL	0x0
#define CP0_PERF02_CTL_VAL	0x80000000
#define CP0_PERF3_CTL_VAL	0x0
#define CP0_ERRCTL_VAL		0x0
#define CP0_TAG_LO1_H_VAL	0x0
#define CP0_TAG_LO1_L_VAL	0x0
#define CP0_TAG_LO2_H_VAL	0x0
#define CP0_TAG_LO2_L_VAL	0x0
#define CP0_CONFIG0_VAL		0x80040602
#define CP0_CONFIG1_VAL		0xFEE3719B
#define CP0_CONFIG2_VAL		0x80000647
#define CP0_CONFIG3_VAL		0xBF8032E9
#define CP0_CONFIG4_VAL		0xC01CC127
#define CP0_CONFIG5_VAL		0x10000038
#define CP0_CONFIG6_VAL		0x0
#define CP0_CONFIG7_VAL		0x80014F20

/* Masks for defined reset values of CP0 registers */

#define CP0_TLB_INDEX_MASK	0x7FFFFC00
#define CP0_TLB_RANDOM_MASK	0xFFFFFFC0
#define CP0_TLB_PG_MASK_MASK	0xE0001FFF
#define CP0_TLB_ENTRY_HI_MASK_H	0xFFFFFF00
#define CP0_TLB_ENTRY_HI_MASK_L	0x1FFF
#define CP0_GUEST_CTL2_MASK	0xFFFF0000
#define CP0_STATUS_MASK		0xCFF500E4
#define CP0_INTCTL_MASK		0x7FFFFF
#define CP0_GUEST_CTL0_MASK	0xFFFFFF83
#define CP0_CAUSE_MASK		0xB1F0083
#define CP0_CDMMBASE_MASK	0xF00007FF
#define CP0_CMGCRBASE_MASK	0xF00007FF
#define CP0_MAAR_MASK_H		0x7F000000
#define CP0_MAAR_MASK_L		0xFFD
#define CP0_MAARI_MASK		0xFFFFFFC0
#define CP0_WATCH_LO_MASK	0x7
#define CP0_WATCH_HI_MASK	0xBF00F000
#define CP0_DEBUG_MASK		0x73FF83C0
#define CP0_TRACE_CONTROL_MASK	0xF0000001
#define CP0_TRACE_CONTROL2_MASK	0xFFFFF000
#define CP0_PERF_CTL_MASK	0xCE7F6010
#define CP0_ERRCTL_MASK		0xFAFFF00F
#define CP0_TAG_LO1_H_MASK	0xFFFF00
#define CP0_TAG_LO1_L_MASK	0x357
#define CP0_TAG_LO2_H_MASK	0x0
#define CP0_TAG_LO2_L_MASK	0x317

/* CP0_STATUS bits. */
#define CP0_STATUS_IE		(1 << 0)	/* Interrupt enable */
#define CP0_STATUS_EXL		(1 << 1)	/* Exception level */
#define CP0_STATUS_ERL		(1 << 2)	/* Error level */
#define CP0_STATUS_IM0		(1 << 8)	/* Mask SW0 */
#define CP0_STATUS_IM1		(1 << 9)	/* Mask SW1 */
#define CP0_STATUS_IM2		(1 << 10)	/* Mask HW0 */
#define CP0_STATUS_IM3		(1 << 11)	/* Mask HW1 */
#define CP0_STATUS_IM4		(1 << 12)	/* Mask HW2 */
#define CP0_STATUS_IM5		(1 << 13)	/* Mask HW3 */
#define CP0_STATUS_IM6		(1 << 14)	/* Mask HW4 */
#define CP0_STATUS_IM7		(1 << 15)	/* Mask HW5 */
#define CP0_STATUS_IM_HW_SHIFT	10
#define CP0_STATUS_IM_HW_MASK	(0x3F << CP0_STATUS_IM_HW_SHIFT)
#define CP0_STATUS_IM_SW_SHIFT	8
#define CP0_STATUS_IM_SW_MASK	(0x3 << CP0_STATUS_IM_SW_SHIFT)
#define CP0_STATUS_BEV		(1 << 22)	/* Boot Exception Vector */
#define CP0_STATUS_CU_MASK	(0xF << 28)	/* Allow access to all the CPs */
#define CP0_STATUS_CU_CP0_ONLY	(0x1 << 28)	/* Allow access only to CP0 the CPs */
#define CP0_STATUS_MX          	(1 << 24)       /* MIPS DSP Extension. Enables access to DSP ASE resources. */

/* MIPS_CP0_CAUSE bits. */
#define CP0_CAUSE_EXCODE_SHIFT	2		/* Shift of the ExcCode field.  */
#define CP0_CAUSE_EXCODE_POS	CP0_CAUSE_EXCODE_SHIFT
#define CP0_CAUSE_EXCODE_MASK	0x1F
#define CP0_CAUSE_EXCODE_BITS	5		/* Number of bits in the ExcCode field. */
#define CP0_CAUSE_IP0		(1 << 8)	/* Cause SW0 */
#define CP0_CAUSE_IP1		(1 << 9)	/* Cause SW1 */
#define CP0_CAUSE_IP2		(1 << 10)	/* Pending HW0 */
#define CP0_CAUSE_IP3		(1 << 11)	/* Pending HW1 */
#define CP0_CAUSE_IP4		(1 << 12)	/* Pending HW2 */
#define CP0_CAUSE_IP5		(1 << 13)	/* Pending HW3 */
#define CP0_CAUSE_IP6		(1 << 14)	/* Pending HW4 */
#define CP0_CAUSE_IP7		(1 << 15)	/* Pending HW5 */
#define CP0_CAUSE_IP_HW_SHIFT	10
#define CP0_CAUSE_IP_HW_POS	CP0_CAUSE_IP_HW_SHIFT
#define CP0_CAUSE_IP_HW_MASK	0x3F
#define CP0_CAUSE_IP_HW_BITS	6		/* Number of bits in the IP_HW field. */
#define CP0_CAUSE_IP_SW_SHIFT	8
#define CP0_CAUSE_IP_SW_POS	CP0_CAUSE_IP_SW_SHIFT
#define CP0_CAUSE_IP_SW_MASK	0x3
#define CP0_CAUSE_IP_SW_BITS	2		/* Number of bits in the IP_SW field. */
#define CP0_CAUSE_WP		(1 << 22)	/* Watch exception. */
#define CP0_CAUSE_WP_POS	22
#define CP0_CAUSE_WP_BITS	1
#define CP0_CAUSE_IV		(1 << 23)	/* Use the special interrupt vector. */
#define CP0_CAUSE_DC		(1 << 27)	/* Disable count register. */
#define CP0_CAUSE_TI		(1 << 30)	/* Timer interrupt is pending. */

/* MIPS_CP0_CONFIG0 bits. */
#define CP0_CONFIG0_MT_SHIFT	7		/* MMU type. */
#define CP0_CONFIG0_MT_SIZE	3		/* 3 bits */
#define CP0_CONFIG0_MT_POS	CP0_CONFIG0_MT_SHIFT
#define	CP0_CONFIG0_MT_NONE	0		/* None */
#define	CP0_CONFIG0_MT_TLB	1		/* Standard TLB. */
#define	CP0_CONFIG0_MT_BAT	2		/* Block Address Translation */
#define	CP0_CONFIG0_MT_FIX_MAP	3		/* Fixed Mapping. */
#define	CP0_CONFIG0_MT_DUAL	4		/* Dual VTLB and FTLB. */
#define	CP0_CONFIG0_VI		(1 << 3)	/* Virtual instruction cache. */
#define	CP0_CONFIG0_K0_UNCACHE	2		/* Kseg0 cacheability and coherency attribute */
#define	CP0_CONFIG0_K0_WB	3		/* Cacheable, non-coherent, write-back, write allocate. */
#define	CP0_CONFIG0_K0_CWBE	4		/* Cacheable, coherent, write-back, write-allocate, read misses request Exclusive */
#define	CP0_CONFIG0_K0_CWB	5		/* Cacheable, coherent, write-back, write-allocate, read misses request Shared */
#define	CP0_CONFIG0_K0_UCA	7		/* Uncached Accelerated, non-coherent */
#define	CP0_CONFIG0_K0_POS	0
#define	CP0_CONFIG0_K0_BITS	3
#define	CP0_CONFIG0_K0_MASK	7

/* MIPS_CP0_CONFIG1 bits. */
#define CP0_CONFIG1_MMUSIZE_SHIFT	25	/* Number of entries in the TLB minus 1. */
#define CP0_CONFIG1_MMUSIZE_BITS	6	/* Number of bits in the register. */
#define CP0_CONFIG1_MMUSIZE_POS		CP0_CONFIG1_MMUSIZE_SHIFT
#define CP0_CONFIG1_IS_SHIFT		22	/* L1 Instruction cache number of sets per way. */
#define CP0_CONFIG1_IS_POS		CP0_CONFIG1_IS_SHIFT
#define CP0_CONFIG1_IS_BITS		3
#define CP0_CONFIG1_IS_256_SETS		2	/* 256 sets per way, 32 KByte instruction cache */
#define CP0_CONFIG1_IS_512_SETS		3	/* 512 sets per way, 64 KByte instruction cache */
#define CP0_CONFIG1_IL_SHIFT		19	/* L1 Instruction cache line size. */
#define CP0_CONFIG1_IL_POS		CP0_CONFIG1_IL_SHIFT
#define CP0_CONFIG1_IL_BITS		3
#define CP0_CONFIG1_IL_32_BYTE		4	/* 32 byte line size. */
#define CP0_CONFIG1_IA_SHIFT		16	/* L1 Instruction cache associativity. */
#define CP0_CONFIG1_IA_POS		CP0_CONFIG1_IA_SHIFT
#define CP0_CONFIG1_IA_BITS		3
#define CP0_CONFIG1_IA_4_WAYS		3
#define CP0_CONFIG1_DS_SHIFT		13	/* L1 Data cache number of sets per way. */
#define CP0_CONFIG1_DS_POS		CP0_CONFIG1_DS_SHIFT
#define CP0_CONFIG1_DS_BITS		3
#define CP0_CONFIG1_DS_256_SETS		2	/* 256 sets per way, 32 KByte data cache */
#define CP0_CONFIG1_DS_512_SETS		3	/* 512 sets per way, 64 KByte data cache */
#define CP0_CONFIG1_DL_SHIFT		10	/* L1 Data cache line size. */
#define CP0_CONFIG1_DL_POS		CP0_CONFIG1_DL_SHIFT
#define CP0_CONFIG1_DL_BITS		3
#define CP0_CONFIG1_DL_32_BYTE		4	/* 32 byte line size. */
#define CP0_CONFIG1_DA_SHIFT		7	/* L1 Data cache associativity. */
#define CP0_CONFIG1_DA_POS		CP0_CONFIG1_DA_SHIFT
#define CP0_CONFIG1_DA_BITS		3
#define CP0_CONFIG1_DA_4_WAYS		3

 /* MIPS_CP0_CONFIG2 bits. */
#define CP0_CONFIG2_L2B	(1 << 12)	/* L2 cache bybass */
#define CP0_CONFIG2_L2B_POS	12
#define CP0_CONFIG2_L2B_BITS	1
#define CP0_CONFIG2_SS_SHIFT	8	/* L2 cache number of sets per way */
#define CP0_CONFIG2_SS_POS	CP0_CONFIG2_SS_SHIFT
#define CP0_CONFIG2_SS_BITS	4
#define CP0_CONFIG2_SS_MASK	(0xF << CP0_CONFIG2_SS_SHIFT)
#define CP0_CONFIG2_SS_512_SETS	3	/* 512 sets per way */
#define CP0_CONFIG2_SS_1024_SETS	4	/* 1024 sets per way */
#define CP0_CONFIG2_SS_2048_SETS	5	/* 2048 sets per way */
#define CP0_CONFIG2_SS_4096_SETS	6	/* 4096 sets per way */
#define CP0_CONFIG2_SS_8192_SETS	7	/* 8192 sets per way */
#define CP0_CONFIG2_SS_16384_SETS	8	/* 16384 sets per way */
#define CP0_CONFIG2_SS_32768_SETS	9	/* 32768 sets per way */
#define CP0_CONFIG2_SL_SHIFT	4	/* L2 data cache line size */
#define CP0_CONFIG2_SL_POS	CP0_CONFIG2_SL_SHIFT
#define CP0_CONFIG2_SL_BITS	4
#define CP0_CONFIG2_SL_MASK	(0xF << CP0_CONFIG2_SL_SHIFT)
#define CP0_CONFIG2_SL_32_BYTE	4	/* 32 byte line size. */
#define CP0_CONFIG2_SL_64_BYTE	5	/* 64 byte line size. */
#define CP0_CONFIG2_SA_SHIFT	0	/* L2 cache associativity */
#define CP0_CONFIG2_SA_POS	CP0_CONFIG2_SA_SHIFT
#define CP0_CONFIG2_SA_BITS	4
#define CP0_CONFIG2_SA_MASK	(0xF << CP0_CONFIG2_SA_SHIFT)
#define CP0_CONFIG2_SA_8_WAY	7	/* 8-way associative L2 cache. */


/* MIPS_CP0_CONFIG3 bits. */
#define CP0_CONFIG3_VEIC_POS		6
#define CP0_CONFIG3_VEIC_BITS		1
#define CP0_CONFIG3_VEIC		(1 << CP0_CONFIG3_VEIC_POS)
#define CP0_CONFIG3_LPA_POS     	7
#define CP0_CONFIG3_LPA        	(1 << CP0_CONFIG3_LPA_POS)


/* MIPS_CP0_CONFIG4 bits. */
#define CP0_CONFIG4_MMU_EXTDEF_POS	14
#define CP0_CONFIG4_MMU_EXTDEF_BITS	2
#define CP0_CONFIG4_MMU_EXTDEF_SIZE	1	/* Config4[7:0] used as MMUSizeExt. */
#define CP0_CONFIG4_MMU_EXTDEF_FTLB	3	/* Config4[3:0] - FTLB ways Config4[7:4] - FTLB sets */
#define CP0_CONFIG4_MMU_SIZE_EXT_POS	0
#define CP0_CONFIG4_MMU_SIZE_EXT_BITS	8
#define CP0_CONFIG4_FTLB_PS_POS		8	/* FTLB Page Size. */
#define CP0_CONFIG4_FTLB_PS_BITS	5
#define CP0_CONFIG4_FTLB_WAYS_POS	4	/* FTLB Ways. */
#define CP0_CONFIG4_FTLB_WAYS_BITS	4
#define CP0_CONFIG4_FTLB_SETS_POS	0	/* FTLB Sets. */
#define CP0_CONFIG4_FTLB_SETS_BITS	4
#define CP0_CONFIG4_P5600_FTLB_SIZE	512	/* FTLB is organized as 128 sets and 4-ways */

/* MIPS_CP0_CONFIG5 bits. */
#define CP0_CONFIG5_K			(1 << 30)

/* MIPS_CP0_CONFIG6 bits. */
#define CP0_CONFIG6_FTLBEN		(1 << 15)	/* FTLB enable. */

/* MIPS_CP0_CONFIG7 bits. */
#define CP0_CONFIG7_HCI_POS		18	/* Hardware Cache Initialization */
#define CP0_CONFIG7_HCI_BITS		1

#define CP0_TLB_ENTRY_HI_EHINV 0x400 /* TLB HW Invalidate */


#endif /* __SYSREG_MIPS_H__ */
