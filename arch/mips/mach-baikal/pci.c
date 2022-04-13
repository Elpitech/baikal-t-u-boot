// SPDX-License-Identifier: GPL-2.0+
/*
 * Baikal-T PCIe initialization callbacks
 *
 * Copyright 2019 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <asm/baikal_hw.h>
#include <asm/pmu.h>
#include <linux/delay.h>

void board_pcie_dw_start(void *regs)
{
	u32 reg, rstc_mask = 0;

#ifndef CONFIG_CLK
	/* 3. Read value of BK_PMU_AXI_PCIE_M_CTL, set EN bit. */
	reg = READ_PMU_REG(BK_PMU_AXI_PCIE_M_CTL);
	reg |= PMU_AXI_PCIE_M_CTL_EN;
	WRITE_PMU_REG(BK_PMU_AXI_PCIE_M_CTL, reg);

	/* 4. Read value of BK_PMU_AXI_PCIE_S_CTL, set EN bit. */
	reg = READ_PMU_REG(BK_PMU_AXI_PCIE_S_CTL);
	reg |= PMU_AXI_PCIE_S_CTL_EN;
	WRITE_PMU_REG(BK_PMU_AXI_PCIE_S_CTL, reg);
#endif

	/*
	 * 5. Manage RESET* bits
	 * (PHY_RESET, PIPE_RESET, CORE_RST, PWR_RST, STICKY_RST, NONSTICKY_RST)
	 */
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
#ifdef CONFIG_BAIKAL_T1
	/* we have Baikal-T1 chip, perform enhanced reset procedure */
	if (reg & PMU_PCIE_RSTC_REQ_PHY_RST)
		rstc_mask |= PMU_PCIE_RSTC_PHY_RESET;
	if (reg & PMU_PCIE_RSTC_REQ_CORE_RST)
		rstc_mask |= PMU_PCIE_RSTC_CORE_RST;
	if (reg & PMU_PCIE_RSTC_REQ_STICKY_RST)
		rstc_mask |= PMU_PCIE_RSTC_STICKY_RST;
	if (reg & PMU_PCIE_RSTC_REQ_NON_STICKY_RST)
		rstc_mask |= PMU_PCIE_RSTC_NONSTICKY_RST;
#else /* BAIKAL-T */
	/* we have Baikal-T chip, perform simplified reset procedure */
	rstc_mask = (PMU_PCIE_RSTC_PHY_RESET | PMU_PCIE_RSTC_PIPE_RESET |
		     PMU_PCIE_RSTC_CORE_RST|  PMU_PCIE_RSTC_PWR_RST |
		     PMU_PCIE_RSTC_STICKY_RST | PMU_PCIE_RSTC_NONSTICKY_RST);
#endif
	WRITE_PMU_REG(BK_PMU_PCIE_RSTC, reg | rstc_mask);
	udelay(10);
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	reg &= ~rstc_mask;
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	if (reg & 0x3f11) {
		reg &= ~0x3f11;
		WRITE_PMU_REG(BK_PMU_PCIE_RSTC, reg);
		udelay(10);
		reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	}

}

void board_pcie_dw_ltssm_start(void *regs_base)
{
	u32 reg;

	/* 10. Set LTSSM enable, app_ltssm_enable=0x1 */
	reg = READ_PMU_REG(BK_PMU_PCIE_GENC);
	reg |= PMU_PCIE_GENC_LTSSM_ENABLE;
	WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);

}

#define PCIE_MISC_CONTROL_1_OFF		0x8bc
#define DBI_RO_WR_EN			1

void board_pcie_dw_dbi_rw(void *regs_base, bool enable)
{
	u32 reg;

	if (enable) {
		/* 3.2 Set writing to RO Registers Using DBI */
		writel(DBI_RO_WR_EN, regs_base + PCIE_MISC_CONTROL_1_OFF);

		/* 3.1 Set DBI2 mode, dbi2_cs = 0x1 */
		reg = READ_PMU_REG(BK_PMU_PCIE_GENC);
		reg &= ~PMU_PCIE_GENC_DBI2_MODE;
		WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);
	} else {
		reg = READ_PMU_REG(BK_PMU_PCIE_GENC);
		reg |= PMU_PCIE_GENC_DBI2_MODE;
		WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);
	}
}

void pci_set_io_port_base(unsigned long base)
{
	set_io_port_base(CKSEG1ADDR(base));
}

