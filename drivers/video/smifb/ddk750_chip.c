// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <linux/sizes.h>

#include "ddk750_help.h"
#include "ddk750_reg.h"
#include "ddk750_chip.h"
#include "ddk750_power.h"

#define MHz(x) ((x) * 1000000)

logical_chip_type_t sm750_get_chip_type(void)
{
	unsigned short physicalID;
	char physicalRev;
	logical_chip_type_t chip;

	physicalID = devId750; /* either 0x718 or 0x750 */
	physicalRev = revId750;

	if (physicalID == 0x718) {
		chip = SM718;
	} else if (physicalID == 0x750) {
		chip = SM750;
		/* SM750 and SM750LE are different in their revision ID only. */
		if (physicalRev == SM750LE_REVISION_ID)
			chip = SM750LE;
	} else {
		chip = SM_UNKNOWN;
	}

	return chip;
}

static unsigned int get_mxclk_freq(void)
{
	unsigned int pll_reg;
	unsigned int M, N, OD, POD;

	if (sm750_get_chip_type() == SM750LE)
		return MHz(130);

	pll_reg = PEEK32(MXCLK_PLL_CTRL);
	M = (pll_reg & PLL_CTRL_M_MASK) >> PLL_CTRL_M_SHIFT;
	N = (pll_reg & PLL_CTRL_N_MASK) >> PLL_CTRL_M_SHIFT;
	OD = (pll_reg & PLL_CTRL_OD_MASK) >> PLL_CTRL_OD_SHIFT;
	POD = (pll_reg & PLL_CTRL_POD_MASK) >> PLL_CTRL_POD_SHIFT;

	return DEFAULT_INPUT_CLOCK * M / N / (1 << OD) / (1 << POD);
}

/*
 * This function set up the main chip clock.
 *
 * Input: Frequency to be set.
 */
static void setChipClock(unsigned int frequency)
{
	pll_value_t pll;

	/* Cheok_0509: For SM750LE, the chip clock is fixed. Nothing to set. */
	if (sm750_get_chip_type() == SM750LE)
		return;

	if (frequency) {
		/*
		 * Set up PLL, a structure to hold the value to be set in
		 * clocks.
		 */
		pll.inputFreq = DEFAULT_INPUT_CLOCK; /* Defined in CLOCK.H */
		pll.clockType = MXCLK_PLL;

		/*
		 * Call calcPllValue() to fill the other fields of PLL
		 * structure.
		 * Sometime, the chip cannot set up the exact clock
		 * required by the User.
		 * Return value of calcPllValue gives the actual possible clock.
		 */
		calcPllValue(frequency, &pll);

		/* Master Clock Control: MXCLK_PLL */
		POKE32(MXCLK_PLL_CTRL, formatPllReg(&pll));
	}
}

static void setMemoryClock(unsigned int frequency)
{
	unsigned int reg, divisor;

	/* Cheok_0509: For SM750LE, the memory clock is fixed.
	 * Nothing to set.
	 */
	if (sm750_get_chip_type() == SM750LE)
		return;

	if (frequency) {
		/*
		 * Set the frequency to the maximum frequency
		 * that the DDR Memory can take which is 336MHz.
		 */
		if (frequency > MHz(336))
			frequency = MHz(336);

		/* Calculate the divisor */
		divisor = DIV_ROUND_CLOSEST(get_mxclk_freq(), frequency);

		/* Set the corresponding divisor in the register. */
		reg = PEEK32(CURRENT_GATE) & ~CURRENT_GATE_M2XCLK_MASK;
		switch (divisor) {
		default:
		case 1:
			reg |= CURRENT_GATE_M2XCLK_DIV_1;
			break;
		case 2:
			reg |= CURRENT_GATE_M2XCLK_DIV_2;
			break;
		case 3:
			reg |= CURRENT_GATE_M2XCLK_DIV_3;
			break;
		case 4:
			reg |= CURRENT_GATE_M2XCLK_DIV_4;
			break;
		}

		setCurrentGate(reg);
	}
}

/*
 * This function set up the master clock (MCLK).
 *
 * Input: Frequency to be set.
 *
 * NOTE:
 *      The maximum frequency the engine can run is 168MHz.
 */
static void setMasterClock(unsigned int frequency)
{
	unsigned int reg, divisor;

	/* Cheok_0509: For SM750LE, the memory clock is fixed.
	 * Nothing to set.
	 */
	if (sm750_get_chip_type() == SM750LE)
		return;

	if (frequency) {
		/* Set the frequency to the maximum frequency
		 * that the SM750 engine can run, which is about 190 MHz.
		 */
		if (frequency > MHz(190))
			frequency = MHz(190);

		/* Calculate the divisor */
		divisor = DIV_ROUND_CLOSEST(get_mxclk_freq(), frequency);

		/* Set the corresponding divisor in the register. */
		reg = PEEK32(CURRENT_GATE) & ~CURRENT_GATE_MCLK_MASK;
		switch (divisor) {
		default:
		case 3:
			reg |= CURRENT_GATE_MCLK_DIV_3;
			break;
		case 4:
			reg |= CURRENT_GATE_MCLK_DIV_4;
			break;
		case 6:
			reg |= CURRENT_GATE_MCLK_DIV_6;
			break;
		case 8:
			reg |= CURRENT_GATE_MCLK_DIV_8;
			break;
		}

		setCurrentGate(reg);
		}
}

unsigned int ddk750_getVMSize(void)
{
	unsigned int reg;
	unsigned int data;

	/* sm750le only use 64 mb memory*/
	if (sm750_get_chip_type() == SM750LE)
		return SZ_64M;

	/* for 750,always use power mode0*/
	reg = PEEK32(MODE0_GATE);
	reg |= MODE0_GATE_GPIO;
	POKE32(MODE0_GATE, reg);

	/* get frame buffer size from GPIO */
	reg = PEEK32(MISC_CTRL) & MISC_CTRL_LOCALMEM_SIZE_MASK;
	switch (reg) {
	case MISC_CTRL_LOCALMEM_SIZE_8M:
		data = SZ_8M;  break; /* 8  Mega byte */
	case MISC_CTRL_LOCALMEM_SIZE_16M:
		data = SZ_16M; break; /* 16 Mega byte */
	case MISC_CTRL_LOCALMEM_SIZE_32M:
		data = SZ_32M; break; /* 32 Mega byte */
	case MISC_CTRL_LOCALMEM_SIZE_64M:
		data = SZ_64M; break; /* 64 Mega byte */
	default:
		data = 0;
		break;
	}
	return data;
}

int ddk750_initHw(initchip_param_t *pInitParam)
{
	unsigned int reg;

	if (pInitParam->powerMode != 0)
		pInitParam->powerMode = 0;
	setPowerMode(pInitParam->powerMode);

	/* Enable display power gate & LOCALMEM power gate*/
	reg = PEEK32(CURRENT_GATE);
	reg |= (CURRENT_GATE_DISPLAY | CURRENT_GATE_LOCALMEM);
	setCurrentGate(reg);

	if (sm750_get_chip_type() != SM750LE) {
		/*	set panel pll and graphic mode via mmio_88 */
		reg = PEEK32(VGA_CONFIGURATION);
		reg |= (VGA_CONFIGURATION_PLL | VGA_CONFIGURATION_MODE);
		POKE32(VGA_CONFIGURATION, reg);
	}

	/* Set the Main Chip Clock */
	setChipClock(MHz((unsigned int)pInitParam->chipClock));

	/* Set up memory clock. */
	setMemoryClock(MHz(pInitParam->memClock));

	/* Set up master clock */
	setMasterClock(MHz(pInitParam->masterClock));

	/* Reset the memory controller.
	 * If the memory controller is not reset in SM750,
	 * the system might hang when sw accesses the memory.
	 * The memory should be resetted after changing the MXCLK.
	 */
	if (pInitParam->resetMemory == 1) {
		reg = PEEK32(MISC_CTRL);
		reg &= ~MISC_CTRL_LOCALMEM_RESET;
		POKE32(MISC_CTRL, reg);

		reg |= MISC_CTRL_LOCALMEM_RESET;
		POKE32(MISC_CTRL, reg);
	}

	if (pInitParam->setAllEngOff == 1) {
		enable2DEngine(0);

		/* Disable Overlay, if a former application left it on */
		reg = PEEK32(VIDEO_DISPLAY_CTRL);
		reg &= ~DISPLAY_CTRL_PLANE;
		POKE32(VIDEO_DISPLAY_CTRL, reg);

		/* Disable video alpha, if a former application left it on */
		reg = PEEK32(VIDEO_ALPHA_DISPLAY_CTRL);
		reg &= ~DISPLAY_CTRL_PLANE;
		POKE32(VIDEO_ALPHA_DISPLAY_CTRL, reg);

		/* Disable alpha plane, if a former application left it on */
		reg = PEEK32(ALPHA_DISPLAY_CTRL);
		reg &= ~DISPLAY_CTRL_PLANE;
		POKE32(ALPHA_DISPLAY_CTRL, reg);

		/* Disable DMA Channel, if a former application left it on */
		reg = PEEK32(DMA_ABORT_INTERRUPT);
		reg |= DMA_ABORT_INTERRUPT_ABORT_1;
		POKE32(DMA_ABORT_INTERRUPT, reg);

		/* Disable DMA Power, if a former application left it on */
		enableDMA(0);
	}

	/* We can add more initialization as needed. */

	return 0;
}

/*
 * monk liu @ 4/6/2011:
 *	re-write the calculatePLL function of ddk750.
 *	the original version function does not use
 *	some mathematics tricks and shortcut
 *	when it doing the calculation of the best N,M,D combination
 *	I think this version gives a little upgrade in speed
 *
 * 750 pll clock formular:
 * Request Clock = (Input Clock * M )/(N * X)
 *
 * Input Clock = 14318181 hz
 * X = 2 power D
 * D ={0,1,2,3,4,5,6}
 * M = {1,...,255}
 * N = {2,...,15}
 */
unsigned int calcPllValue(unsigned int request_orig, pll_value_t *pll)
{
	/* as sm750 register definition,
	 * N located in 2,15 and M located in 1,255
	 */
	int N, M, X, d;
	int mini_diff;
	unsigned int RN, quo, rem, fl_quo;
	unsigned int input, request;
	unsigned int tmpClock, ret;
	const int max_OD = 3;
	int max_d = 6;

	if (sm750_get_chip_type() == SM750LE) {
		/* SM750LE don't have
		 * programmable PLL and M/N values to work on.
		 * Just return the requested clock.
		 */
		return request_orig;
	}

	ret = 0;
	mini_diff = ~0;
	request = request_orig / 1000;
	input = pll->inputFreq / 1000;

	/* for MXCLK register,
	 * no POD provided, so need be treated differently
	 */
	if (pll->clockType == MXCLK_PLL)
		max_d = 3;

	for (N = 15; N > 1; N--) {
		/* RN will not exceed maximum long
		 * if @request <= 285 MHZ (for 32bit cpu)
		 */
		RN = N * request;
		quo = RN / input;
		rem = RN % input;/* rem always small than 14318181 */
		fl_quo = (rem * 10000 / input);

		for (d = max_d; d >= 0; d--) {
			X = BIT(d);
			M = quo * X;
			M += fl_quo * X / 10000;
			/* round step */
			M += (fl_quo * X % 10000) > 5000 ? 1 : 0;
			if (M < 256 && M > 0) {
				unsigned int diff;

				tmpClock = pll->inputFreq * M / N / X;
				diff = abs(tmpClock - request_orig);
				if (diff < mini_diff) {
					pll->M = M;
					pll->N = N;
					pll->POD = 0;
					if (d > max_OD)
						pll->POD = d - max_OD;
					pll->OD = d - pll->POD;
					mini_diff = diff;
					ret = tmpClock;
				}
			}
		}
	}
	return ret;
}

unsigned int formatPllReg(pll_value_t *pPLL)
{
#ifndef VALIDATION_CHIP
	unsigned int POD = pPLL->POD;
#endif
	unsigned int OD = pPLL->OD;
	unsigned int M = pPLL->M;
	unsigned int N = pPLL->N;
	unsigned int reg = 0;

	/*
	 * Note that all PLL's have the same format. Here, we just use
	 * Panel PLL parameter to work out the bit fields in the
	 * register. On returning a 32 bit number, the value can be
	 * applied to any PLL in the calling function.
	 */
	reg = PLL_CTRL_POWER |
#ifndef VALIDATION_CHIP
		((POD << PLL_CTRL_POD_SHIFT) & PLL_CTRL_POD_MASK) |
#endif
		((OD << PLL_CTRL_OD_SHIFT) & PLL_CTRL_OD_MASK) |
		((N << PLL_CTRL_N_SHIFT) & PLL_CTRL_N_MASK) |
		((M << PLL_CTRL_M_SHIFT) & PLL_CTRL_M_MASK);

	return reg;
}
