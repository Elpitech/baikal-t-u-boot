/*
 * File:        clk.h
 * Description: PLL information and manipulation for Baikal-T
 * Created:     01 Jun 2015
 * Author:      Dmitry Dunaev
 * Copyright 2015 Baikal Electronics
 *
 */

#ifndef _ASM_CLK_H
#define _ASM_CLK_H

#define BE_CLK_ENABLE		(1 << 0)
#define BE_CLK_RESET		(1 << 1)
#define BE_CLK_SET		(1 << 2)
#define BE_CLK_BYPASS		(1 << 30)
#define BE_CLK_LOCK		(1 << 31)

#define BE_CLKR_SHFT			2
#define BE_DIV_SHFT			4
#define BE_CLKF_SHFT			8
#define BE_CLKOD_SHFT			21

#define BE_CLKR_VAL(NR)			((NR - 1) << BE_CLKR_SHFT)
#define BE_CLKF_VAL(NF)			((NF - 1) << BE_CLKF_SHFT)
#define BE_CLKOD_VAL(OD)		((OD - 1) << BE_CLKOD_SHFT)

#define BE_PLL_CLK_VAL(NR, NF, OD)	\
	(BE_CLK_ENABLE | BE_CLK_RESET | \
	BE_CLKR_VAL(NR) | BE_CLKF_VAL(NF) | BE_CLKOD_VAL(OD))

#define BE_CLK_DIV_MAX_WIDTH	17
#define BE_CLK_DIV_MASK		(((1 << BE_CLK_DIV_MAX_WIDTH) - 1) << BE_DIV_SHFT)
#define BE_CLK_PARAM_MASK	(0x3FFFFFFC)

#define BE_RD_CLKR(src)			(((src) & 0x000000fc) >> BE_CLKR_SHFT)
#define BE_RD_CLKF(src)			(((src) & 0x001FFF00) >> BE_CLKF_SHFT)
#define BE_RD_CLKOD(src)		(((src) & 0x01E00000) >> BE_CLKOD_SHFT)

#endif /* _ASM_CLK_H */
