/*
 * File:        lowlevel_init.c
 * Description: Low Level initialization code
 * Authors:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *   Alexey.Malahov <alexey.malahov@baikalelectronics.ru>
 *   Ekaterina.Skachko <ekaterina.skachko@baikalelectronics.ru>
 *
 * Copyright 2014-2016 Baikal Electronics JSC
 *
 * On BFK board the 24C02 EEPROM is used as FRU device and as
 * storage for several variables.
 */

/* Memory sub-system initialization code */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/arch/p5600.h>
#include <asm/arch/segcfg.h>
#include <asm/arch/sysreg.h>
#include <asm/arch/clock_manager.h>

#define SRAM_BASE			0xBBF80000
#define SRAM_STACK_OFF		0x8000
#define DDR_BASE			0xBF042000
#define DDR_SCHED_OFF		0x0250
#define DDR_SCHED_INIT		0x00001001
#define DDR_SCHED_MSK		0xFFFFFFFB

#ifdef CONFIG_SYS_BOOT_DELAY_MS
#define BOOT_TIMER_FREQ_MS	(_ULCAST_(CONFIG_SYS_INIT_TIMER_FREQ) / 1000)
#define BOOT_DELAY_TICKS	(CONFIG_SYS_BOOT_DELAY_MS * BOOT_TIMER_FREQ_MS)
#endif
						 
	.text
	.set noreorder
	.set mips0

	.globl	lowlevel_init
lowlevel_init:
	/* Boot controller barrier */
#ifdef CONFIG_SYS_BOOT_DELAY_MS
#ifdef CONFIG_MIPS_GIC_BASE
	/* Disable GIC */
	li       t0, KSEG1ADDR(CONFIG_MIPS_GCR_BASE)
	lw       t1, MIPS_GCR_GIC_BASE(t0)
	and      t1, t1, 0xfffffffe
	sw       t1, MIPS_GCR_GIC_BASE(t0)
#endif /* CONFIG_MIPS_GIC_BASE */
	/* Disable CPU IRQ */
	mfc0     t1, CP0_STATUS, 0
	and      t1, t1, ~ST0_IE
	and      t1, t1, ~ST0_IM
	mtc0     t1, CP0_STATUS, 0
	/* Setup timer */
	lui      t1, (BOOT_DELAY_TICKS >> 16)
	ori      t1, t1, (BOOT_DELAY_TICKS & 0xFFFF)
	mtc0     t1, CP0_COMPARE, 0
	mtc0     zero, CP0_COUNT, 0
	mfc0     t1, CP0_STATUS, 0
	or       t1, t1, IE_IRQ5
	mtc0     t1, CP0_STATUS, 0
	/* Wait for internal timer */
	.set mips3
	nop
	wait
	nop
	.set mips0
#endif /* CONFIG_SYS_BOOT_DELAY_MS */

	/* Check CPU Number */
	mfc0     a0, CP0_EBASE, 1
	.set mips2
	andi     a0, a0, 0xFF
	bgtz     a0, secondary_cpu
	nop

	/* Fix DDR auto page close */
	lui t0, (DDR_BASE >> 16)
	ori t0, t0, (DDR_BASE & 0xFFFF)
	lw t1, DDR_SCHED_OFF(t0)
	and t1, t1, DDR_SCHED_MSK
	sw t1, DDR_SCHED_OFF(t0)
	sync

	.set mips0
	/*
	 * Set the stack pointer to SRAM.
	 */
	lui	sp, ((SRAM_BASE + SRAM_STACK_OFF) >> 16)
	ori	sp, sp, ((SRAM_BASE + SRAM_STACK_OFF) & 0xFFFF)

	/* Save ra */
	addiu	sp, sp, -4
	sw	ra, 4(sp)

#ifdef CONFIG_BAIKAL_FW
	la	t0, __start_baikal_fw
#else
	la	t0, __lowlevel_init_cp0
#endif /* CONFIG_BAIKAL_FW */
	jalr	t0
	nop	/* Delay slot */

	/* Setup Global Configuration Register (GCR) */
#ifdef CONFIG_MIPS_GCR_BASE
	/* Set GCR base addresses */
	li       t0, KSEG1ADDR(CONFIG_MIPS_GCR_BASE)
#ifdef CONFIG_MIPS_GIC_BASE
	/* Set GIC base addresses, block disabled */
	li       t1, CPHYSADDR(CONFIG_MIPS_GIC_BASE)
	sw       t1, MIPS_GCR_GIC_BASE(t0)
#endif /* CONFIG_MIPS_GIC_BASE */
#ifdef CONFIG_MIPS_CPC_BASE
	/* Set CPC base addresses, block enabled */
	li       t1, CPHYSADDR(CONFIG_MIPS_CPC_BASE) | MIPS_GCR_BLOCK_ENABLE
	sw       t1, MIPS_GCR_CPC_BASE(t0)
#endif /* CONFIG_MIPS_CPC_BASE */
#endif /* CONFIG_MIPS_GCR_BASE */
	nop
	/* Restore ra */
	lw	ra, 4(sp)
	jr	ra
	addiu	sp, sp, 4	/* Delay slot */

	/* __start_baikal_fw duplicates these settings.  */
__lowlevel_init_cp0:

	/*
	 * Step 2) Establish Status Register
	 * (set BEV, clear ERL, clear EXL, clear IE)
	 */
	li	t1, 0x00400000
	mtc0	t1, CP0_STATUS

	/*
	 * Step 3) Establish CP0 Config0
	 * (set K0=3)
	 */
	li	t1, 0x00000003
	mtc0	t1, CP0_CONFIG

	/*
	 * Step 7) Establish Cause
	 * (set IV bit)
	 */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE

	/* Establish Wired (and Random) */
	mtc0	zero, CP0_WIRED
	nop
	li      t0, MIPS_LEGACY_SEGCFG0
	mtc0    t0, CP0_PAGEMASK, 2

	li      t0, MIPS_LEGACY_SEGCFG1
	mtc0    t0, CP0_PAGEMASK, 3

	li      t0, MIPS_LEGACY_SEGCFG2
	mtc0    t0, CP0_PAGEMASK, 4

	mfc0    t0, CP0_CONFIG, 5
	li      t1, 1
	ins     t0, t1, 0, 30
	mtc0    t0, CP0_CONFIG, 5

	jr	ra
	nop

secondary_cpu:
	.set mips3
	wait
	.set mips0
	b secondary_cpu
	nop	/* Delay slot */

#ifdef U_BOOT_VERSION_SDK
    .long 0x27051956	/* U-Boot Magic Number */

    .globl version_string
version_string:
    .ascii U_BOOT_VERSION
    .ascii " / SDK ", U_BOOT_VERSION_SDK
    .ascii " (", __DATE__, " - ", __TIME__, ")"
    .ascii " ", CONFIG_IDENT_STRING, "\0"
#endif
