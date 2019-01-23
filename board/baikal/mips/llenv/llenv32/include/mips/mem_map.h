/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: MIPS memory map header file.
 */

#ifndef __MEM_MAP_MIPS_H__
#define __MEM_MAP_MIPS_H__

#include "irq_map.h"
#ifndef __ASSEMBLER__
#include "llenv.h"
#endif /*__ASSEMBLER__*/

#define CPU_ACTIVE	0x00 /* Core0 */
#define CPU_SPEED

#define NUM_CORES	2

/* DRAM Memory map. */
#ifdef HAPS
#define MEMORY_LOW_PADDR		0x00100000
#define MEMORY_LOW_VADDR		0xE0100000
#define MEMORY_LOW_SIZE			0x00040000 /* 256 KBytes */
#define MEMORY_BASE_UNMAP		0xA0100000
#else
#define MEMORY_LOW_PADDR        0x00000000
#define MEMORY_LOW_VADDR        0xE0000000 /* Define memory from kseg3 (kernel mapped) */
#define MEMORY_LOW_SIZE         0x08000000
#define MEMORY_BASE_UNMAP		0xA0000000
#endif /* HAPS */

#define MEMORY_HIGH_PADDR       0x20000000
#define MEMORY_HIGH_VADDR       0xC0000000
#define MEMORY_HIGH_SIZE        0x20000000
#define MEMORY_BASE_MAP         MEMORY_LOW_VADDR /* Use low memory */

#define MEMORY_BASE			MEMORY_BASE_MAP
#define MEMORY_SIZE			MEMORY_LOW_SIZE
#define GET_PADDR(v)			((0x1FFFFFFF) & (v))

/* Peripheral devices. */
#define PCIE_MAP_PHYS			0x08000000
#define PCIE_MAP_AREA			0xA8000000
#define GIC_BASE_PHYS			0x1BDC0000
#define GIC_BASE			0xBBDC0000
#define CPC_BASE_PHYS			0x1BDE0000
#define CPC_BASE			0xBBDE0000
#define SRAM_BASE			0xBBF80000
#define SRAM_SIZE			0x10000	/* 64Kbytes */
#define ROM_BASE			0x9BFC0000
#define SPI_FLASH_BASE			0xBC000000
#define BOOT_CNTR_BASE			0xBF040000
#define BOOT_SPI_OFFSET                 0x100
#define DMA_CFGREG_BASE			0xBF041000
#define DDR_CTRL			0xBF042000
#define DDR_PHY				0xBF043000
#define GPIO_BASE			0xBF044000
#define GPIO_CTRL_BASE			0xBF045000
#define IPORT_OFF			0x00001000
#define I2C_0_BASE			0xBF046000
#define I2C_1_BASE			0xBF047000
#define I2C_BASE			I2C_0_BASE
#define RTC_BASE			0xBF048000
#define TIMERS_BASE			0xBF049000
#define UART0_BASE			0xBF04A000
#define UART1_BASE			0xBF04B000
#define UART_BASE			UART0_BASE
#define UPORT_OFF			0x00001000
#define WDT_BASE			0xBF04C000
#define PMU_BASE			0xBF04D000
#define SPI0_BASE			0xBF04E000
#define SPI1_BASE			0xBF04F000
#define SPI_BASE			SPI0_BASE
#define SPI_OFFSET			0x00001000
#define SATA_BASE			0xBF050000
#define PCIE_CFG_BASE			0xBF052000
#define PCIE_DMA			0xBF053000
#define XGMAC_BASE			0xBF054000
#define APBT_BASE			0xBF059000
#define INTERCONNECT_BASE		0xBF05A000
#define CRYPTO_BASE			0xBF05B000
#define CRYPTO_DMA_BASE			0xBF05C000
#define CRYPTO_DMA_PHYS_BASE		0x1F05C000
#define XGMAC_XPCS_BASE			0xBF05D000
#define GMAC0_BASE			0xBF05E000
#define GMAC1_BASE			0xBF060000
#define GMAC_BASE			GMAC0_BASE
#define USB1_CTRL_BASE			0xBF100000
#define USB_BASE			USB1_CTRL_BASE
#define PVT_BASE                        0xBF200000



#define QEMU_STOP_ADDR			0xBF0FF000

#ifdef HAPS
#define TEST_RESULT_ADDR		(MEMORY_BASE_UNMAP + MEMORY_SIZE - 0x10) // 0xA013FFF0
#else /* HAPS */
#define TEST_RESULT_ADDR		0xBF0FF000
#endif /* HAPS */

/*
 * Define the LLENV32_TEST_RESULT macro to save the test result.
 */
#if defined(BAIKAL_T)
#define LLENV32_TEST_RESULT(c, x)	debug_msg("Test on core%d %s st=0x%x\n", (c), (x) ? "FAILED" : "PASSED", (x))
#elif ((defined(RTL) || defined(QEMU)) && !defined(FCT))
#define LLENV32_TEST_RESULT(c, x)	(*((int *) (TEST_RESULT_ADDR + 16 * (c))) = (x))
#else /* RTL */
#define LLENV32_TEST_RESULT(c, x)
#endif

/* Define the base address of the vector table. */
#ifdef HAPS
#define EBASE_VEC_VAL			(MEMORY_BASE_UNMAP)
#else
#define EBASE_VEC_VAL			(MEMORY_BASE_UNMAP + 0x00100000)
#endif /* HAPS */

/*
 * Set stack before the special-memory map region in kseg3. The address
 * should be less than (MEMORY_BASE + MEMORY_SIZE).
 */
#define STACK_TOP			(MEMORY_BASE_UNMAP + MEMORY_SIZE - 0x200)
#define STACK_CORE_SIZE			0x10000	/* 64 KBytes. */

#define UBOOT_SPD_REGS_ADDR		(SRAM_BASE + 0x1000)

#endif /* __MEM_MAP_MIPS_H__ */

