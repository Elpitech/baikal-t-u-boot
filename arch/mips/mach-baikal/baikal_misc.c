/*
 * File:	baikal_misc.c
 * Description:	Initialization of Baikal-T1 hardware
 * Authors:
 *
 * Copyright 2019 T-platforms JSC
 */

#include <common.h>
#include <dm.h>
#include <clk.h>
#include <fdt_support.h>
#include <asm/baikal_hw.h>
#include <asm/io.h>
#include <asm/pmu.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	ulong val;

	/* switch boot controller into SPI mode */
	val = readl((void __iomem *)BAIKAL_BOOT_BASE);
	val |= 0x100;
	writel(val, (void __iomem *)BAIKAL_BOOT_BASE);

	/* Enable whatever clocks not enabled by reset */
#ifndef CONFIG_CLK
	/* SATA: */
	val = READ_PMU_REG(BK_PMU_AXI_SATA_CTL);
	val |= 1;
	WRITE_PMU_REG(BK_PMU_AXI_SATA_CTL, val);

	val = READ_PMU_REG(BK_PMU_SATA_REF_CTL);
	val |= 1;
	WRITE_PMU_REG(BK_PMU_SATA_REF_CTL, val);

	/* USB: */
	val = READ_PMU_REG(BK_PMU_AXI_USB_CTL);
	val |= 1;
	WRITE_PMU_REG(BK_PMU_AXI_USB_CTL, val);

	val = READ_PMU_REG(BK_PMU_USB_CTL);
	val |= 1;
	WRITE_PMU_REG(BK_PMU_USB_CTL, val);

	val = READ_PMU_REG(BK_PMU_AXI_GMAC1_CTL);
	val |= 1; /* enable */
	WRITE_PMU_REG(BK_PMU_AXI_GMAC1_CTL, val);

	val = READ_PMU_REG(BK_PMU_AXI_GMAC2_CTL);
	val |= 1; /* enable */
	WRITE_PMU_REG(BK_PMU_AXI_GMAC2_CTL, val);
#endif

	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

void board_preboot_os(void)
{
	u32 val;

	/* restore boot controller to "ROM" state */
	val = readl((void __iomem *)BAIKAL_BOOT_BASE);
	val &= ~0x100;
	writel(val, (void __iomem *)BAIKAL_BOOT_BASE);
}

int ft_board_setup(void *blob, struct bd_info *bd)
{
	u64 mem_start[2], mem_size[2];
	unsigned long bank1_size = gd->arch.highmem_size;
	unsigned long mem_max;

	mem_start[0] = DDR_BANK0_START;
	mem_size[0] = DDR_BANK0_SIZE;
	mem_start[1] = DDR_BANK1_START;

	/* Setting "mem_max" can limit top of memory passed to linux.
	 * This may be useful for older kernels not supporting XPA
	 * and/or 32-bit FDT.
	 */
	mem_max = env_get_ulong("mem_max", 10, 65536);
	if ((bank1_size + 512) > mem_max)
		mem_size[1] = (((u64)mem_max << 20) - 1 - mem_start[1]) & ~(0xffffULL);
	else
		mem_size[1] = (u64)bank1_size << 20;

	return fdt_fixup_memory_banks(blob, mem_start, mem_size, 2);
}
