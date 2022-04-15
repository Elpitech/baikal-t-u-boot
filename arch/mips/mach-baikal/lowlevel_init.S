/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2019 T-Platforms
 */

#include <config.h>

#include <asm-offsets.h>
#include <asm/addrspace.h>
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

	.text
	.set noreorder

ENTRY(lowlevel_init)
	addiu	sp,sp,-8
	sw	ra,4(sp)
	ehb

	/* nothing here so far */

#ifdef DEBUG
	mfc0	a0, CP0_CONFIG
	PTR_LA	t9,printhex8
	jalr	t9
	nop

	li	a0,'\n'
	PTR_LA	t9,printch
	jalr	t9
	nop
#endif

	lw	ra,4(sp)
	jr	ra
	addiu	sp,sp,8
END(lowlevel_init)
