/*
 * Baikal-T hardware addresses etc.
 */

#ifndef ___BAIKAL_HW_H
#define ___BAIKAL_HW_H
#include <asm/addrspace.h>

#define DDR_BANK0_START		0x00000000
#define DDR_BANK0_SIZE		0x08000000
#define DDR_BANK1_START		0x20000000


#define BAIKAL_PLL_PHYS		0x1F04D000
#define DDR_CTRL_PHYS		0x1F042000
#define DDR_PHY_PHYS		0x1F043000
#define BAIKAL_BOOT_PHYS	0x1F040000

#define BAIKAL_PLL_BASE		CKSEG1ADDR(BAIKAL_PLL_PHYS)
#define DDR_CTRL_BASE		CKSEG1ADDR(DDR_CTRL_PHYS)
#define DDR_PHY_BASE		CKSEG1ADDR(DDR_PHY_PHYS)
#define BAIKAL_BOOT_BASE	CKSEG1ADDR(BAIKAL_BOOT_PHYS)

#endif