/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014-2017 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: DDR init functions.
 */

#include <config.h>
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/ddr.h>
#include <asm/ddr_hw.h>
#include <asm/baikal_hw.h>
#include <asm/pmu.h>


#define DDR_WAIT_RETRIES 0xff00

/* distance between PUB_DXn registers groups */
#define PUB_DXn_STEP	(DDR_PUB_DX1GCR0-DDR_PUB_DX0GCR0)
/* log2 of the above */
#define PUB_DXn_SLOG	8
/* sanity check */
#if (1 << PUB_DXn_SLOG) != PUB_DXn_STEP
#error "PUB_DXn_SLOG is incorrect."
#endif

.macro BROM_DDR_DEBUG_STEP	step
	li	v0, \step
.endm

.macro ddrc_set_reg	reg_off, val
	lw	t0, (\val * 4)(v1)
	sw	t0, \reg_off(s0)
.endm

.macro ddrc_get_reg	reg_res, reg_off
	lw	\reg_res, \reg_off(s0)
.endm

.macro pub_set_reg	reg_off, val
	lw	t0, (\val * 4)(v1)
	sw	t0, \reg_off(s1)
	nop	/* Small delay */
.endm

.macro pub_get_reg	reg_res, reg_off
	lw	\reg_res, \reg_off(s1)
.endm




.set noreorder
.text



/* --------------------- */
/*   init_ddr_spd        */
/* --------------------- */
LEAF(ddr_init)
	addi	sp,sp,-24
	sw	ra,20(sp)
	sw	s0,16(sp)
	sw	s1,12(sp)
	move	v1,a0

	/* Initialization based on PMU doc v0.3 and bug#62 */

	/* 1. Wait for BK_PMU_COREPLL_CTL LOCK bit. */
	li	t0, BK_PMU_COREPLL_CTL
	li	t1, DDR_WAIT_RETRIES
	BROM_DDR_DEBUG_STEP 1

_ddr_spd_corepll_lock_wait:
	beqz	t1, _ddr_spd_init_exit
	lw	t2, 0(t0)	/* Delay slot. */
	ext	t2, t2, PMU_COREPLL_CTL_LOCK_POS, PMU_COREPLL_CTL_LOCK_BITS
	beqz	t2, _ddr_spd_corepll_lock_wait
	addi	t1, t1, -1 /* Delay slot. */

	/* 2. Read value of BK_PMU_AXI_DDR_CTL, set EN bit. */
	li	t0, BK_PMU_AXI_DDR_CTL
	lw	t1, 0(t0)
#if defined(CONFIG_BAIKAL_T1)
	ori	t1, t1, PMU_AXI_DDR_CTL_EN
	sw	t1, 0(t0)
	sync
#endif

	/* 3. Set BK_PMU_AXI_DDR_CTL.INIT bit. */
	ori	t1, t1, PMU_AXI_DDR_CTL_INIT
	sw	t1, 0(t0)

	/* get required DDR_PLL_CTL */
	lw	t1, (DDR_PLL_CTL*4)(v1)

	/* 4. Read value of BK_PMU_DDRPLL_CTL, set EN bit, write RST bit and CLKOD. */
	li	t0, BK_PMU_DDRPLL_CTL

	ori	t1, t1, (PMU_DDRPLL_CTL_EN | PMU_DDRPLL_CTL_RST)
	sw	t1, 0(t0)


	/* 5. Wait for BK_PMU_DDRPLL_CTL LOCK bit */
	li	t1, DDR_WAIT_RETRIES
	BROM_DDR_DEBUG_STEP 2

_ddr_spd_ddrpll_lock_wait:
	beqz	t1, _ddr_spd_init_exit
	lw	t2, 0(t0)	/* Delay slot. */
	ext	t2, t2, PMU_DDRPLL_CTL_LOCK_POS, PMU_DDRPLL_CTL_LOCK_BITS
	beqz	t2, _ddr_spd_ddrpll_lock_wait
	addi	t1, t1, -1 /* Delay slot. */

	/* 6. Read value of BK_PMU_DDR_CTL, set EN bit, set INIT bit */
	li	t0, BK_PMU_DDR_CTL
	lw	t1, 0(t0)
	ori	t1, t1, (PMU_DDR_CTL_EN | PMU_DDR_CTL_INIT)
	sw	t1, 0(t0)


	/* 7. Configure DDR memory controller. */
	li	s0, DDR_CTRL_BASE
	li	s1, DDR_PHY_BASE


	ddrc_set_reg	DDRC_INIT3, DDR3_SPD_INIT3
	ddrc_set_reg	DDRC_INIT4, DDR3_SPD_INIT4
	ddrc_set_reg	DDRC_DFITMG0, DDR3_SPD_DFITMG0
	ddrc_set_reg	DDRC_DFITMG1, DDR3_SPD_DFITMG1
	ddrc_set_reg	DDRC_DRAMTMG0, DDR3_SPD_DRAMTMG0
	ddrc_set_reg	DDRC_DRAMTMG1, DDR3_SPD_DRAMTMG1
	ddrc_set_reg	DDRC_DRAMTMG2, DDR3_SPD_DRAMTMG2
	ddrc_set_reg	DDRC_DRAMTMG3, DDR3_SPD_DRAMTMG3
	ddrc_set_reg	DDRC_DRAMTMG4, DDR3_SPD_DRAMTMG4
	ddrc_set_reg	DDRC_DRAMTMG5, DDR3_SPD_DRAMTMG5
	ddrc_set_reg	DDRC_DRAMTMG8, DDR3_SPD_DRAMTMG8
	ddrc_set_reg	DDRC_ADDRMAP0, DDR3_SPD_ADDRMAP0
	ddrc_set_reg	DDRC_ADDRMAP1, DDR3_SPD_ADDRMAP1
	ddrc_set_reg	DDRC_ADDRMAP2, DDR3_SPD_ADDRMAP2
	ddrc_set_reg	DDRC_ADDRMAP3, DDR3_SPD_ADDRMAP3
	ddrc_set_reg	DDRC_ADDRMAP4, DDR3_SPD_ADDRMAP4
	ddrc_set_reg	DDRC_ADDRMAP5, DDR3_SPD_ADDRMAP5
	ddrc_set_reg	DDRC_ADDRMAP6, DDR3_SPD_ADDRMAP6
	ddrc_set_reg	DDRC_SARBASE(0), DDR3_SPD_SARBASE0
	ddrc_set_reg	DDRC_SARSIZE(0), DDR3_SPD_SARSIZE0
	ddrc_set_reg	DDRC_SARBASE(1), DDR3_SPD_SARBASE1
	ddrc_set_reg	DDRC_SARSIZE(1), DDR3_SPD_SARSIZE1
	ddrc_set_reg	DDRC_SARBASE(2), DDR3_SPD_SARBASE2
	ddrc_set_reg	DDRC_SARSIZE(2), DDR3_SPD_SARSIZE2
	ddrc_set_reg	DDRC_RFSHTMG, DDR3_SPD_RFSHTMG
	ddrc_set_reg	DDRC_ODTCFG, DDR3_SPD_ODTCFG
	ddrc_set_reg	DDRC_ODTMAP, DDR3_SPD_ODTMAP
	ddrc_set_reg	DDRC_ZQCTL0, DDR3_SPD_ZQCTL0
	ddrc_set_reg	DDRC_ECCCFG0, DDR3_SPD_ECCCFG0
#if defined(ECC_DBG_CORR_ERR) || defined (ECC_DBG_UNCORR_ERR)
	/* ECCCFG0 enable ECC anf disable scrubber */
	li	t2, 0x14
	sw	t2, DDRC_ECCCFG0(s0)
	/* ECCCFG1 for debug data poison en in corr and uncorr err mode */
	ddrc_get_reg	t2, DDRC_ECCCFG1
#ifdef ECC_DBG_CORR_ERR
	li	t2, 0x3
#endif /* ECC_DBG_CORR_ERR */

#ifdef ECC_DBG_UNCORR_ERR
	li	t2, 0x1
#endif /*ECC_DBG_UNCORR_ERR */
	sw	t2, DDRC_ECCCFG1(s0)

	ddrc_get_reg	t2, DDRC_ECCPOISONADDR0
	li	t2, 0x00000004
	sw	t2, DDRC_ECCPOISONADDR0(s0)

	ddrc_get_reg	t2, DDRC_ECCPOISONADDR1
	li	t2, 0x00000002
	sw	t2, DDRC_ECCPOISONADDR1(s0)
#endif /* ECC_DBG_CORR_ERR || ECC_DBG_UNCORR_ERR */
	ddrc_set_reg	DDRC_INIT0, DDR3_SPD_INIT0
#if defined(CONFIG_BAIKAL_T1)
	ddrc_set_reg	DDRC_INIT1, DDR3_SPD_INIT1
#endif
	ddrc_set_reg	DDRC_DFIUPD0, DDR3_SPD_DFIUPD0
	ddrc_set_reg	DDRC_MSTR, DDR3_SPD_MSTR
#if defined(CONFIG_BAIKAL_T1)
	ddrc_set_reg	DDRC_MRCTRL0, DDR3_SPD_MRCTRL0
#endif
	sync

	/* 8. Reset BK_PMU_DDR_CTL INIT bit */
	li	t0, BK_PMU_DDR_CTL
	lw	t1, 0(t0)
	ins	t1, zero, PMU_DDR_CTL_INIT_POS, PMU_DDR_CTL_INIT_BITS
	sw	t1, 0(t0)

	/* 9. Reset BK_PMU_AXI_DDR_CTL INIT bit */
	li	t0, BK_PMU_AXI_DDR_CTL
	lw	t1, 0(t0)
	ins	t1, zero, PMU_AXI_DDR_CTL_INIT_POS, PMU_AXI_DDR_CTL_INIT_BITS
	sw	t1, 0(t0)

	/* 10. DDR PUB initialization. */

	/* Disable unused bytes */
	la	t0, DDR_PUB_DX0GCR0(s1)
	lw	t2, (DDR_BYTE_WIDTH*4)(v1)
	la	t1, DDR_PUB_DX4GCR0(s1)
	sll	t2, t2, PUB_DXn_SLOG
	add	t0, t0, t2
	/* t0 - first DXn to disable, t1 - last */
__ddr_dis_next:
	sub	t2, t0, t1
	bgtz	t2, 1f
	nop
	lw	t2, 0(t0)
	ins	t2, zero, 0, 1
	sw	t2, 0(t0)
	b	__ddr_dis_next
	addi	t0, t0, PUB_DXn_STEP
1:

	pub_set_reg	DDR_PUB_PGCR1, DDR_PUB_SPD_PGCR1
	pub_set_reg	DDR_PUB_PGCR2, DDR_PUB_SPD_PGCR2
	pub_set_reg	DDR_PUB_PTR1, DDR_PUB_SPD_PTR1
	pub_set_reg	DDR_PUB_PTR3, DDR_PUB_SPD_PTR3
	pub_set_reg	DDR_PUB_PTR4, DDR_PUB_SPD_PTR4
	pub_set_reg	DDR_PUB_DTPR0, DDR_PUB_SPD_DTPR0
	pub_set_reg	DDR_PUB_DTPR1, DDR_PUB_SPD_DTPR1
	pub_set_reg	DDR_PUB_DTPR2, DDR_PUB_SPD_DTPR2
	pub_set_reg	DDR_PUB_DTPR3, DDR_PUB_SPD_DTPR3
	pub_set_reg	DDR_PUB_DTPR4, DDR_PUB_SPD_DTPR4
	pub_set_reg	DDR_PUB_DTPR5, DDR_PUB_SPD_DTPR5
	pub_set_reg	DDR_PUB_MR0, DDR_PUB_SPD_MR0
	pub_set_reg	DDR_PUB_MR1, DDR_PUB_SPD_MR1
	pub_set_reg	DDR_PUB_MR2, DDR_PUB_SPD_MR2
	pub_set_reg	DDR_PUB_DTCR0, DDR_PUB_SPD_DTCR0
	pub_set_reg	DDR_PUB_DTCR1, DDR_PUB_SPD_DTCR1

#if 1 /* Not sure that it helps but let it be */
	pub_get_reg t0, DDR_PUB_PLLCR
	lui	t1, 0x8
	or	t0, t0, t1	/* set FRQSEL to 225-490MHz */
	sw	t0, DDR_PUB_PLLCR(s1)
	nop
#endif

	pub_set_reg	DDR_PUB_PIR, DDR_PUB_SPD_PIR_VAL1

	/* Wait till PGSR0[IDONE] is equal to 1. */
	sync
	li	t1, DDR_WAIT_RETRIES
	BROM_DDR_DEBUG_STEP 3

_ddr_spd_impedance_wait:
	beqz	t1, _ddr_spd_init_exit
	pub_get_reg	t2, DDR_PUB_PGSR0 /* Delay slot. */
	andi	t2, t2, DDR_PUB_PGSR_IDONE
	beqz	t2, _ddr_spd_impedance_wait
	addi	t1, t1, -1 /* Delay slot. */

	/* Select Data Training algorithms. */
	pub_set_reg	DDR_PUB_DXCCR, DDR_PUB_SPD_DXCCR
	pub_set_reg	DDR_PUB_PIR, DDR_PUB_SPD_PIR_VAL2


	/* Wait till PGSR0[IDONE] is equal to 1. */
	sync
	li	t1, DDR_WAIT_RETRIES
	BROM_DDR_DEBUG_STEP 4

_ddr_spd_data_train_wait:
	beqz	t1, _ddr_spd_init_exit
	pub_get_reg	t2, DDR_PUB_PGSR0 /* Delay slot. */
	andi	t2, t2, DDR_PUB_PGSR_IDONE
	beqz	t2, _ddr_spd_data_train_wait
	addi	t1, t1, -1 /* Delay slot. */


	BROM_DDR_DEBUG_STEP 5

#if !defined(CONFIG_BAIKAL_T1)
	/* ignore training algorithms errors */
#else
	/* Check errors of training algorithms. */
	pub_get_reg	t2, DDR_PUB_PGSR0
	ext	t1, t2, DDR_PUB_PGSR_TRAIN_ERRBIT_START, DDR_PUB_PGSR_TRAIN_ERRBIT_LEN
	bnez	t1, _ddr_spd_init_exit
	nop	/* Delay slot. */
#endif

	/* Check if ECC is enabled */
	lw	t0, (DDR3_SPD_ECCCFG0*4)(v1)
	beqz	t0, _ddr_scrubber_skip
	nop
	/*
	 * Perform initialization memory with pattern using SCRUBBER.
	 */

	/* Set scrubber write mode and clear scrub_interval. */
	ddrc_get_reg	t2, DDRC_SBRCTL
	ori	t2, t2, SCRUB_MODE_WR
	ins	t2, zero, SCRUB_INTVAL_SHIFT, SCRUB_INTVAL_LEN
	sw	t2, DDRC_SBRCTL(s0)
	sync

	/* Set patterns */
	li	t6, SCRUB_PATTERN0
	sw	t6, DDRC_SBRWDATA0(s0)
	sw	t6, DDRC_SBRWDATA1(s0)
	sync

	/* Enable scrubber */
	ori	t2, t2, SCRUB_EN
	sw	t2, DDRC_SBRCTL(s0)
	sync

	BROM_DDR_DEBUG_STEP 6
	/* Wait setting DONE 1 and clearing BUSY */
	lui	t1, DDR_WAIT_RETRIES
_ddr_spd_scrubber_wait:
	beqz	t1, _ddr_spd_init_exit
	ddrc_get_reg	t3, DDRC_SBRSTAT
	andi	t4, t3, SCRUB_DONE
	beqz	t4, _ddr_spd_scrubber_wait
	addi	t1, t1, -1 /* Delay slot. */
	andi	t5, t3, SCRUB_BUSY
	bnez	t5, _ddr_spd_scrubber_wait
	nop	/* Delay slot */

	/* Disable scrubber */
	ins	t2, zero, SCRUB_EN_SHIFT, 1
	sw	t2, DDRC_SBRCTL(s0)
	sync

	/* Set reading mode and the interval. */
	ins	t2, zero, SCRUB_MODE_SHIFT, 1
	ori	t2, (SCRUB_INTVAL_DEF << SCRUB_INTVAL_SHIFT)
	sw	t2, DDRC_SBRCTL(s0)
	sync

	/* Enable scrubber */
	ori	t2, t2, SCRUB_EN
	sw	t2, DDRC_SBRCTL(s0)
	sync

_ddr_scrubber_skip:
	/* Enable AXI Port */
	ddrc_set_reg	DDRC_PCTRL(0), DDR3_SPD_PCTRL_0

	/* Wait till DDRC STAT is equal to 1. */
	sync
	li	t1, DDR_WAIT_RETRIES
	BROM_DDR_DEBUG_STEP 7

_ddr_spd_stat_wait:
	beqz	t1, _ddr_spd_init_exit
	ddrc_get_reg	t2, DDRC_STAT /* Delay slot. */
	ext	t2, t2, DDRC_STAT_MODE_POS, DDRC_STAT_MODE_BITS
	li	t3, DDRC_STAT_MODE_NORMAL
	bne	t2, t3, _ddr_spd_stat_wait
	addi	t1, t1, -1 /* Delay slot. */
	move	v0,zero

_ddr_spd_init_exit:
	lw	s0,16(sp)
	lw	s1,12(sp)
	lw	ra,20(sp)
	jr	ra
	addi	sp,sp,24

	END(ddr_init)

