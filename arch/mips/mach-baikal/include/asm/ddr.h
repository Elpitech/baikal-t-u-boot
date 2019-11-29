/*
 *
 */

#ifndef __DDR_H__
#define __DDR_H__

#ifndef __ASSEMBLY__
#include <compiler.h>
#endif


#define DDR3_SPD_INIT3		0
#define DDR3_SPD_INIT4		1
#define DDR3_SPD_DFITMG0	2
#define DDR3_SPD_DFITMG1	3
#define DDR3_SPD_DRAMTMG0	4
#define DDR3_SPD_DRAMTMG1	5
#define DDR3_SPD_DRAMTMG2	6
#define DDR3_SPD_DRAMTMG3	7
#define DDR3_SPD_DRAMTMG4	8
#define DDR3_SPD_DRAMTMG5	9
#define DDR3_SPD_DRAMTMG8	10
#define DDR3_SPD_ADDRMAP0	11
#define DDR3_SPD_ADDRMAP1	12
#define DDR3_SPD_ADDRMAP2	13
#define DDR3_SPD_ADDRMAP3	14
#define DDR3_SPD_ADDRMAP4	15
#define DDR3_SPD_ADDRMAP5	16
#define DDR3_SPD_ADDRMAP6	17
#define DDR3_SPD_SARBASE0	18
#define DDR3_SPD_SARSIZE0	19
#define DDR3_SPD_SARBASE1	20
#define DDR3_SPD_SARSIZE1	21
#define DDR3_SPD_SARBASE2	22
#define DDR3_SPD_SARSIZE2	23
#define DDR3_SPD_RFSHTMG	24
#define DDR3_SPD_ODTCFG		25
#define DDR3_SPD_ODTMAP		26
#define DDR3_SPD_ZQCTL0		27
#define DDR3_SPD_ECCCFG0	28
#define DDR3_SPD_INIT0		29
#define DDR3_SPD_DFIUPD0	30
#define DDR3_SPD_MSTR		31
#define DDR3_SPD_PCTRL_0	32
#define DDR_PUB_SPD_PGCR1	33
#define DDR_PUB_SPD_PGCR2	34
#define DDR_PUB_SPD_PTR1	35
#define DDR_PUB_SPD_PTR3	36
#define DDR_PUB_SPD_PTR4	37
#define DDR_PUB_SPD_DTPR0	38
#define DDR_PUB_SPD_DTPR1	39
#define DDR_PUB_SPD_DTPR2	40
#define DDR_PUB_SPD_DTPR3	41
#define DDR_PUB_SPD_DTPR4	42
#define DDR_PUB_SPD_DTPR5	43
#define DDR_PUB_SPD_MR0		44
#define DDR_PUB_SPD_MR1		45
#define DDR_PUB_SPD_MR2		46
#define DDR_PUB_SPD_DTCR0	47
#define DDR_PUB_SPD_PIR_VAL1	48
#define DDR_PUB_SPD_DXCCR	49
#define DDR_PUB_SPD_PIR_VAL2	50
#define DDR_PUB_SPD_DTCR1	51
#define DDR3_SPD_MRCTRL0	52
#define DDR3_SPD_INIT1		53
#define DDR_PLL_CTL		54
#define DDR_BYTE_WIDTH		55
#define DDR_SPD_LAST		56

#ifndef __ASSEMBLY__

struct ddr3_timings {
	unsigned tCK;
	unsigned tAA;
	unsigned tRCD;
	unsigned tRP;
	unsigned tRC;
	unsigned tRASmin;
	unsigned tFAWmin;
	unsigned tRTP;
	unsigned tWRmin;
	unsigned tRRD;
	unsigned tRFC;
	unsigned tWTR;
};

struct ddr3_geometry {
	unsigned bus_width;
	unsigned ranks;
	unsigned whole_mem;
	unsigned rows;
	unsigned cols;
	unsigned ecc;
};

#define DLL_FAST_EXIT 0
#define MEMC_FREQ_RATIO 2
#define MEMC_MODE_2T 0
#define BL 8

#define tMRD    4       /* Mode Register Set cycle time (nCK) */
#define tREFI   7800000 /* Average Periodic Refresh interval (ps) */
#define tCCD    4       /* CAS to CAS delay (nCK) */
#define tWLMRD  40      /* nCK */

#define DDR3_MR0_PD_S	12
#define DDR3_MR0_WR_S	9
#define DDR3_MR0_CL_S0	2
#define DDR3_MR0_CL_S1	4

#define DDR3_MR1_AL_S	3
#define DDR3_MR1_AL_0	0
#define DDR3_MR1_AL_CL1	1
#define DDR3_MR1_AL_CL2	2
#if !defined(CONFIG_BAIKAL_T1)
#define DDR3_MR1_AL	DDR3_MR1_AL_CL1
#else
/*
 * vvv: The Doc recommends to avoid DRAMTMG4.t_rcd equal 1 when
 * MEMC_FREQ_RATIO=2, so disable additive latency.
 */
#define DDR3_MR1_AL	DDR3_MR1_AL_0
/*
 * However, performance degradation was not confirmed for the setting
 * #define DDR3_MR1_AL	DDR3_MR1_AL_CL1
 */
#endif

#define DDR3_MR1_DR_S   1
#define DDR3_MR1_RTT_S  2


#define DDR3_MR2_CWL_S	3
#define DDR3_MR2_RTT_S  9

extern int ddr_prepare_regs(struct ddr3_timings *, struct ddr3_geometry *, uint32_t *);
extern int ddr_parse_spd(uint8_t *, struct ddr3_timings *, struct ddr3_geometry *);
extern int ddr_init(uint32_t *);
#endif

#endif /* __DDR_H__ */
