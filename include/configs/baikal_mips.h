/*
 * Configuration settings for the Baikal T platform.
 *
 * Copyright (C) 2013-2017 Baikal Electronics JSC
 *
 * Authors:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *   Alexey.Malahov <alexey.malahov@baikalelectronics.ru>
 *   Ekaterina.Skachko <ekaterina.skachko@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_BAIKAL_MIPS_H
#define __CONFIG_BAIKAL_MIPS_H

/*
 *-----------------------------------------------
 * Common MIPS U-Boot Parameters
 *-----------------------------------------------
 */
#ifdef CONFIG_BAIKAL_T1
#define CONFIG_SYS_CLK_FREQ		1000000000
#else
#define CONFIG_SYS_CLK_FREQ		850000000
#endif
#define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_SYS_CLK_FREQ / 2)

/*
 *-----------------------------------------------
 * U-Boot initial parameters
 *-----------------------------------------------
 */
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MONITOR_LEN		(1 << 20)	/* 1 MB */
#define CONFIG_SYS_MALLOC_LEN		(1 << 20)	/* 1 MB */
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 << 10)	/* 128 KB */
#define CONFIG_SYS_BOOTM_LEN 		0x01000000

/*
 *-----------------------------------------------
 * SRAM and it variables
 *-----------------------------------------------
 */
#define CONFIG_SRAM_BASE		0x9BF80000
#define CONFIG_SRAM_SIZE		0x10000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SRAM_BASE + CONFIG_SRAM_SIZE)

/*
 *-----------------------------------------------
 * Physical Memory Map
 *-----------------------------------------------
 */
#define CONFIG_SYS_SDRAM_BASE		0x80000000
/* Only one bank on lower 512 MB can be reached without MMU */
#define CONFIG_SYS_SDRAM_SIZE		0x08000000
/* Default load address */
#define CONFIG_SYS_LOAD_ADDR		0x80100000

/*
 *-----------------------------------------------
 * Miscellaneous configurable options
 *-----------------------------------------------
 */
/* Console I/O Buffer Size  */
#define CONFIG_SYS_CBSIZE			1024
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			256
/* Мax number of command args   */
#define CONFIG_SYS_MAXARGS			16
/* Boot Argument Buffer Size    */
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE

/*
 *-----------------------------------------------
 * UART Configuration
 *-----------------------------------------------
 */
/* Device vemory load/store as word */
#define CONFIG_SYS_NS16550_MEM32

#ifndef CONFIG_DM_SERIAL
/* UART pool base address */
#define BAIKAL_NS16550_BASE			0xBF04A000
/* Register width 4 bytes, little endian */
#define CONFIG_SYS_NS16550_REG_SIZE		(-4)
/* Use NS16550 UART as Serial device */
#define CONFIG_SYS_NS16550_SERIAL		1
/* UART1 is enable and COM1 Port base */
#define CONFIG_SYS_NS16550_COM1			(BAIKAL_NS16550_BASE + 0x0000)
/* UART2 is enable and COM2 Port base  */
#define CONFIG_SYS_NS16550_COM2			(BAIKAL_NS16550_BASE + 0x1000)
#endif

/* UART CLK divisor */
#if defined(CONFIG_BAIKAL_T1)
#define CONFIG_SYS_NS16550_CLK			(7350000) /* 7.35MHZ / DIV=4 */
#else
#define CONFIG_SYS_NS16550_CLK			(12500000) /* 12.5MHZ / DIV=10 */
#endif /* CONFIG_BAIKAL_T1 */

/*
 *-----------------------------------------------
 * SPI-flash/Boot controller parameters
 *-----------------------------------------------
 */
#define CONFIG_SYS_FLASH_BASE		0x9C000000

#endif /* __CONFIG_BAIKAL_MIPS_H */
