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


#ifndef __BAIKAL_MIPS_H__
#define __BAIKAL_MIPS_H__

#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_USE_PRIVATE_LIBGCC


/* spd */
#define CONFIG_SYS_SPD_BUS_NUM      1
#define CONFIG_BAIKAL_SPD_ADDRESS   0x50

/* uboot continues work on DDR fail. */
#define CONFIG_BAIKAL_FW_CONTINUE_ON_DDR_FAIL


/*
 *-----------------------------------------------
 * SRAM
 *-----------------------------------------------
 */
#define CONFIG_SRAM_BASE	0xBBF80000
#define CONFIG_SRAM_SIZE	0x10000


/*
 *-----------------------------------------------
 * variables in SRAM
 *-----------------------------------------------
 */
#define CONFIG_SRAM_SPD_TIMESTAMP	(CONFIG_SRAM_BASE + 0x0)
#define CONFIG_SRAM_DDR_INIT_RESULT	(CONFIG_SRAM_BASE + 0x100)
#define CONFIG_SRAM_BUF0_BASE		(CONFIG_SRAM_BASE + 0x200)
#define CONFIG_SRAM_BUF1_BASE		(CONFIG_SRAM_BASE + 0x300)
#define CONFIG_SRAM_STACK_BASE		(CONFIG_SRAM_BASE + CONFIG_SRAM_SIZE - 0x100)

/* Offset of the DDR init handler. */
#define CONFIG_DDR_INIT_RESULT_OFF_v0	0x0
#define CONFIG_DDR_INIT_RESULT_OFF_v1	0x4
#define CONFIG_DDR_INIT_RESULT_OFF_t7	0x8
#define CONFIG_DDR_INIT_RESULT_OFF_i2c	0xc

#define CONFIG_DDR_INIT_RESULT_v0	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_v0)
#define CONFIG_DDR_INIT_RESULT_v1	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_v1)
#define CONFIG_DDR_INIT_RESULT_t7	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_t7)
#define CONFIG_DDR_INIT_RESULT_i2c	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_i2c)


/*
 *-----------------------------------------------
 * High Level Configuration Options
 *-----------------------------------------------
 */
/* Baikal Electronics' MAC address range: 4C:A5:15:x:x:x */
#define CONFIG_BAIKAL_MAC_BASE		0x4CA515000000ULL

/* Configuration for QEMU simulator */
#ifdef CONFIG_BAIKAL_QEMU
/* Clocks */
#define CONFIG_SYS_CPU_CLOCK_RATE	100000000	/* 1000 MHz */
/* Configuration name */
#define CONFIG_BAIKAL_NAME			"QEMU"
#endif

/* Configuration for the FPGA board */
#ifdef CONFIG_BAIKAL_FPGA
/* Clocks */
#define CONFIG_SYS_CPU_CLOCK_RATE	3838383
/* Configuration name */
#define CONFIG_BAIKAL_NAME			"HAPS_FPGA"
#endif /* CONFIG_BAIKAL_FPGA */

/* Configuration for the BFK board */
#ifdef CONFIG_BAIKAL_BFK
/* Clocks */
#define CONFIG_SYS_CPU_CLOCK_MHZ	850
#define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)
#define CONFIG_SYS_CPU_CLOCK_SET
/* Set CPU freq */

/* Configuration name */
#define CONFIG_BAIKAL_NAME			"BFK"
#endif /* CONFIG_BAIKAL_BFK */

/* Configuration for the BFKX board (BFK1.6+Baikal-T1) */
#ifdef CONFIG_BAIKAL_BFKX
/* Clocks */
#define CONFIG_SYS_CPU_CLOCK_MHZ	1200
#define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)
#define CONFIG_SYS_CPU_CLOCK_SET
/* Set CPU freq */

/* Configuration name */
#define CONFIG_BAIKAL_NAME			"BFKX"
#endif /* CONFIG_BAIKAL_BFKX */

/* Configuration for the BFK3 board */
#ifdef CONFIG_BAIKAL_BFK3
/* Clocks */
#define CONFIG_SYS_CPU_CLOCK_MHZ	1200
#define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)
#define CONFIG_SYS_CPU_CLOCK_SET
/* Set CPU freq */

/* Configuration name */
#define CONFIG_BAIKAL_NAME			"BFK3"
#endif /* CONFIG_BAIKAL_BFK3 */

/*
 *-----------------------------------------------
 * Include low-level tests
 *
 * Locate the libllenv.a and llenv.h files to
 * the <uboot_dir>/lib/baikal/llenv directory
 *-----------------------------------------------
 */
#define CONFIG_BAIKAL_LLENV

/*
 *-----------------------------------------------
 * Common MIPS U-Boot Parameters
 *-----------------------------------------------
 */
/* Base address of reset vector */
#define CONFIG_SYS_TEXT_BASE		0x9FC00000
#define CONFIG_SYS_CLK_FREQ			(CONFIG_SYS_CPU_CLOCK_RATE)
#define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_SYS_CPU_CLOCK_RATE / 2)
/* Startup core and timer freq for lowlevel init */
#define CONFIG_SYS_INIT_CLK_FREQ	600000000
#define CONFIG_SYS_INIT_TIMER_FREQ	(CONFIG_SYS_INIT_CLK_FREQ / 2)
/*
 * Fix for boot controller startup.
 * Bootup delay barrier (in ms).
 * Up to 16s for 300MHz core timer clock.
 */
#if !defined(CONFIG_BAIKAL_T1)
#define CONFIG_SYS_BOOT_DELAY_MS	1000 /* 1s */
#endif /* BAIKAL_T1 */
#define CONFIG_SYS_HZ				1000
#define CONFIG_OF_UPDATE_FREQ
#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_ARCH_CPU_INIT_R

/*
 *-----------------------------------------------
 * FIT image support
 *-----------------------------------------------
 */
#define CONFIG_FIT
#define CONFIG_FIT_SIGNATURE
#define CONFIG_SHA1
#define CONFIG_SHA256
#define CONFIG_MD5
#define CONFIG_LZMA
#define CONFIG_AES
#define CONFIG_RSA
#define CONFIG_IMAGE_FORMAT_LEGACY

#define OF_STDOUT_PATH	"serial0:115200n8"		/* Serial console settings */

#define CONFIG_OF_LIBFDT						/* Open Firmware devicetree support	*/
#define CONFIG_OF_EMBED							/* Internal FDT embedded in image */
#define CONFIG_DEFAULT_DEVICE_TREE 	baikal

/*
 *-----------------------------------------------
 * Cache Configuration
 *-----------------------------------------------
 */
#define CONFIG_SYS_DCACHE_SIZE		(64 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(64 * 1024)
#define CONFIG_SYS_CACHELINE_SIZE	32
#define CONFIG_MIPS_L2_SIZE		(1024 * 1024)
#define CONFIG_MIPS_L2_LINE_SIZE	32
#define ARCH_DMA_MINALIGN 		CONFIG_SYS_CACHELINE_SIZE

/*
 *-----------------------------------------------
 * U-Boot initial parameters
 *-----------------------------------------------
 */
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MONITOR_LEN		(1 << 20) /* 1 MB */
#define CONFIG_SYS_MALLOC_LEN		(1 << 20)	/* 1 MB */
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 << 10)	/* 128 KB */
#define CONFIG_SYS_INIT_SP_OFFSET	0xc00000
#define CONFIG_SYS_BOOTM_LEN 		0x01000000

/*
 *-----------------------------------------------
 * U-Boot startup menu
 *-----------------------------------------------
 */
#if defined(CONFIG_BAIKAL_BFK) || defined(CONFIG_BAIKAL_BFK3) || defined(CONFIG_BAIKAL_BFKX)
#define CONFIG_CMD_BOOTMENU
#define CONFIG_MENU
#define CONFIG_MENU_SHOW
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_BOOTDELAY 3
#define CONFIG_MENU_HEADER "--== " CONFIG_BAIKAL_NAME " boot menu ==--"
#else
#define CONFIG_BOOTDELAY 3
#endif


/*
 *-----------------------------------------------
 * MIPS P5600 Warrior Core defaults
 *-----------------------------------------------
 */
#define CONFIG_MIPS_P5600
#define CONFIG_MIPS_GIC_BASE 		0xBBDC0000
#define CONFIG_MIPS_CPC_BASE 		0xBBDE0000
#define CONFIG_MIPS_GCR_BASE 		0xBFBF8000

/*
 *-----------------------------------------------
 * Physical Memory Map
 *-----------------------------------------------
 */
#define CONFIG_SYS_SDRAM_BASE		0x80000000
/* Only one bank on lower 512 MB can be reached without MMU */
#define CONFIG_SYS_SDRAM_SIZE		0x08000000
/* Default load address */
#define CONFIG_SYS_LOAD_ADDR		0xA0100000
#define CONFIG_PAGE_SIZE_16KB

/*
 *-----------------------------------------------
 * Define DRAM Banks
 *-----------------------------------------------
 */
/* Memory banks */
#define CONFIG_NR_DRAM_BANKS		1
/* DRAM Bank #1 start address */
#define PHYS_DRAM_BANK1		 	0x00000000
/* DRAM Bank #1 size  */
#define PHYS_DRAM_BANK1_SIZE		0x08000000
/* DRAM Bank #2 start address */
#define PHYS_DRAM_BANK2		 	0x20000000
/* SDRAM Bank #1 size */
#define PHYS_DRAM_BANK2_SIZE		0x04000000
/*
 *-----------------------------------------------
 * PLL setup
 *-----------------------------------------------
 */
#define CONFIG_CMD_CLK
#define CONFIG_BAIKAL_PLL
#define CONFIG_BAIKAL_PLL_BASE	0xBF04D000
#define CONFIG_BAIKAL_OSC_CLOCK 25000000

/*
 *-----------------------------------------------
 * OEM, FRU, NVRAM and service options
 *-----------------------------------------------
 */
#define CONFIG_CMD_SERVICE
#define CONFIG_SYS_NVRAM_BUS_NUM	0x01
#define CONFIG_SYS_I2C_NVRAM_ADDR	0x56
#define CONFIG_SYS_OEM_BUS_NUM		0x01
#define CONFIG_SYS_OEM_I2C_ADDR		0x51
#define CONFIG_SYS_OEM_FRU_OFS		0x00
#define CONFIG_SYS_OEM_VENDOR_OFS	0xC0
#define CONFIG_SYS_OEM_NET_OFS		0xD0
#define CONFIG_SYS_OEM_MISC_OFS		0xE8

/*
 *-----------------------------------------------
 * RTC Temperature sensor
 *-----------------------------------------------
 */
#if 0
#define CONFIG_CMD_DTT
#define CONFIG_SYS_DTT_BUS_NUM		0x01
#define CONFIG_SYS_I2C_DTT_ADDR		0x56
#define CONFIG_DTT_SENSORS			{ 0 }
#define CONFIG_DTT_ABEOZ9S3
#endif

/*
 *-----------------------------------------------
 * Miscellaneous configurable options
 *-----------------------------------------------
 */
#ifndef DEBUG
/* Should be undefined to save memory */
#define CONFIG_SYS_LONGHELP
#endif
/* Monitor Command Prompt   */
#define CONFIG_SYS_PROMPT			"BAIKAL # "
/* Configuration identification string */
#define CONFIG_INDENT_STRING		"BAIKAL " CONFIG_BAIKAL_NAME " Core"
/* Console I/O Buffer Size  */
#define CONFIG_SYS_CBSIZE			1024
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
/* Мax number of command args   */
#define CONFIG_SYS_MAXARGS			16
/* Boot Argument Buffer Size    */
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE
/* Use port0 as console */
#define CONFIG_CONS_INDEX       1
#define CONFIG_SYS_HUSH_PARSER
/*
 *-----------------------------------------------
 * UART Configuration
 *-----------------------------------------------
 */
/* Use 16550 compatible device */
#define CONFIG_SYS_NS16550
/* UART pool base address */
#define CONFIG_SYS_NS16550_BASE			0xBF04A000
/* Device vemory load/store as word */
#define CONFIG_SYS_NS16550_MEM32
/* Register width 4 bytes, little endian */
#define CONFIG_SYS_NS16550_REG_SIZE		(-4)
/* Use NS16550 UART as Serial device */
#define CONFIG_SYS_NS16550_SERIAL		1
/* UART CLK divisor */
#if defined(CONFIG_BAIKAL_T1)
#define CONFIG_SYS_NS16550_CLK			(7350000) /* 7.35MHZ / DIV=4 */
#else
#define CONFIG_SYS_NS16550_CLK			(12500000) /* 12.5MHZ / DIV=10 */
#endif /* CONFIG_BAIKAL_T1 */
/* UART1 is enable and COM1 Port base */
#define CONFIG_SYS_NS16550_COM1			(CONFIG_SYS_NS16550_BASE + 0x00000)
/* UART2 is enable and COM2 Port base  */
#define CONFIG_SYS_NS16550_COM2			(CONFIG_SYS_NS16550_BASE + 0x10000)

/*
 *-----------------------------------------------
 * Watchdog Timer parameters
 *-----------------------------------------------
 */
#define CONFIG_DESIGNWARE_WATCHDOG
#define CONFIG_HW_WATCHDOG
#define CONFIG_HW_WATCHDOG_TIMEOUT_MS	2000		/* 2000ms */
#define CONFIG_DW_WDT_CLOCK_KHZ		1000000		/* 1000KHz */
#define CONFIG_DW_WDT_BASE		0xBF04C000



/*
 *-----------------------------------------------
 * DesignWire DDR controller
 *-----------------------------------------------
 */
#define CONFIG_DDR_CTRL_BASE		0xBF042000
#define CONFIG_DDR_PHY_BASE		0xBF043000
#define CONFIG_DDR_SCHED_OFF		0x0250
#define CONFIG_DDR_SCHED_INIT		0x00001001
#define CONFIG_DDR_SCHED_MSK		0xFFFFFFFB
// #define CONFIG_BAIKAL_PRINT_DDR

/* user data */
// #define DDR_USER_SPD
// #define DDR_USER_REGS

/* ddr pll */
// #define DDR_SPEED_BIN_SPD
// #define DDR_SPEED_BIN_1066
// #define DDR_SPEED_BIN_1333
// #define DDR_SPEED_BIN_1600


#if defined(CONFIG_BAIKAL_BFK)
	#define DDR_USER_SPD
	#define DDR_SPEED_BIN_SPD

	#undef DDR_USER_REGS
	#undef DDR_SPEED_BIN_1066
	#undef DDR_SPEED_BIN_1333
	#undef DDR_SPEED_BIN_1600
#endif

#if defined(CONFIG_BAIKAL_BFKX)
	#define DDR_USER_SPD
	#define DDR_SPEED_BIN_1600

	#undef DDR_USER_REGS
	#undef DDR_SPEED_BIN_1066
	#undef DDR_SPEED_BIN_1333
	#undef DDR_SPEED_BIN_SPD
#endif

#if defined(CONFIG_BAIKAL_BFK3)
	#define DDR_SPEED_BIN_1600

	#undef DDR_USER_SPD
	#undef DDR_USER_REGS
	#undef DDR_SPEED_BIN_SPD
	#undef DDR_SPEED_BIN_1066
	#undef DDR_SPEED_BIN_1333
#endif


/*
 *-----------------------------------------------
 * GPIO ports parameters
 *-----------------------------------------------
 */
#define CONFIG_DW_GPIO
#define CONFIG_DW_GPIO_BASE		0xBF044000

/*
 *-----------------------------------------------
 * Ethernet controller parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_NET
#define CONFIG_DESIGNWARE_ETH
#define CONFIG_DESIGNWARE_ETH0_BASE	0xBF05E000
#define CONFIG_DESIGNWARE_ETH1_BASE	0xBF060000
#define CONFIG_NET_MULTI
#define CONFIG_MII
#define CONFIG_CMD_MII
#define CONFIG_PHYLIB
#define CONFIG_PHY_ADDR				0x03
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_GIGE
#define CONFIG_DW_ALTDESCRIPTOR

/*
 *-----------------------------------------------
 * I2C controller parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_BUS_MAX			2
#define CONFIG_HARD_I2C
#define CONFIG_DW_I2C
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_SYS_I2C_BASE				0xBF046000
#define CONFIG_SYS_I2C_BASE1			0xBF047000
#define CONFIG_SYS_I2C_SPEED			400000		/* 400 kHz */
#define CONFIG_SYS_I2C_SLAVE			0x02
#define CONFIG_I2C_CHIPADDRESS			0x50 		/* should be 0x50 for AT24C02 */
/* EEPROM parameters */
#define CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_MULTI_EEPROMS
#define CONFIG_SYS_EEPROM_WREN
#define CONFIG_ENV_EEPROM_IS_ON_I2C
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN			1
#define CONFIG_SYS_I2C_EEPROM_ADDR				0x51
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS		3
/*
 *-----------------------------------------------
 * Board RTC parameters
 *-----------------------------------------------
 */
#if 0
#define CONFIG_CMD_DATE
#define CONFIG_RTC_ABEOZ9
#define CONFIG_SYS_RTC_BUS_NUM  0x01
#define CONFIG_SYS_I2C_RTC_ADDR 0x56
#endif

/*
 *-----------------------------------------------
 * DesignWire AHCI SATA controller parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_SATA
#define CONFIG_CMD_SATA_PHY
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE      1
#define CONFIG_DWC_AHSATA_PORT_ID       0
#define CONFIG_DWC_AHSATA_BASE_ADDR	0xBF050000
#define CONFIG_LBA48
#define CONFIG_LIBATA

/*
 *-----------------------------------------------
 * DesignWire xHCD USB controller parameters
 * Due to size constraints (currently, u-boot.bin must fit 0x80000 bytes)
 * we have to choose between SATA and USB.
 *-----------------------------------------------
 */
#if defined(CONFIG_BAIKAL_BFK3)
#define CONFIG_USB
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_USB
#define CONFIG_USB_XHCI
#define CONFIG_USB_XHCI_BAIKAL
#define CONFIG_USB_XHCI_BAIKAL_BASE		0xBF100000
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS	1
#define CONFIG_USB_MAX_CONTROLLER_COUNT		1
#endif

/*
 *-----------------------------------------------
 * DesignWire SPI controller parameters
 *-----------------------------------------------
 */
/* #define CONFIG_SPI */
/* #define CONFIG_HARD_SPI */
#define CONFIG_SPI_FIFO_SIZE		16
#define CONFIG_DW_SPI
#define CONFIG_SPI0_BASE		0xBF040100
#define CONFIG_SPI1_BASE		0xBF04E000
#define CONFIG_SPI2_BASE		0xBF04F000
#define CONFIG_DW_CLOCK_FREQ		12500000
#define CONFIG_DW_SPI_FREQ		11
#ifdef DEBUG
#define CONFIG_DW_SPI_VERBOSE
#endif

/*
 *-----------------------------------------------
 * MMC/SD card support over SPI
 *-----------------------------------------------
 */
#if 0
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC_SPI
#define CONFIG_CMD_MMC
#define CONFIG_CMD_MMC_SPI
#endif
/* Boot controller parameters */
#define CONFIG_BOOT_BASE		0xBF040000
#define CONFIG_BOOT_SPI_FREQ		1000000
#define CONFIG_BOOT_SPI_BUS		0
#define CONFIG_BOOT_SPI_CS		0

/*
 *-----------------------------------------------
 * FLASH driver setup
 *-----------------------------------------------
 */
/* No memory-mapped flash at now */
#define CONFIG_SYS_NO_FLASH
/* Use SPI flash */
#define CONFIG_SPI_FLASH
/* SPI flash banked */
#define CONFIG_SPI_FLASH_BAR
/* SPI framework command */
#define CONFIG_CMD_SF
#define CONFIG_CMD_SF_TEST
/* Support for Atmel SPI Flash */
#define CONFIG_SPI_FLASH_ATMEL
/* Support for Macronix SPI Flash */
#define CONFIG_SPI_FLASH_MACRONIX
/* Support for Spanison SPI Flash */
#define CONFIG_SPI_FLASH_SPANSION
/* Support for SST SPI Flash */
#define CONFIG_SPI_FLASH_SST
/* Support for STMicro SPI Flash */
#define CONFIG_SPI_FLASH_STMICRO
/* Support for Winbond SPI Flash */
#define CONFIG_SPI_FLASH_WINBOND
/*-----------------------------------------------
 * FLASH and environment organization
 *-----------------------------------------------
 */
#if 1
#define CONFIG_LLENV32_SAVEENV
/*#define CONFIG_ENV_OVERWRITE		1*/
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_SYS_FLASH_BASE		0x9C000000
#define CONFIG_ENV_OFFSET		0x00080000
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
#define CONFIG_ENV_SIZE			0x00010000 /* 64 KB */
#define CONFIG_ENV_SPI_BUS		CONFIG_BOOT_SPI_BUS
#define CONFIG_ENV_SPI_CS		CONFIG_BOOT_SPI_CS
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_BOOT_SPI_FREQ
#define CONFIG_ENV_SPI_MODE		SPI_MODE_3

#else
/* Enviroment in flash */
#define CONFIG_ENV_IS_IN_SPI_FLASH
/* #define CONFIG_ENV_IS_NOWHERE */
/* Enviroment block size */
#define CONFIG_ENV_SIZE					0x00010000 /* 64 KB */
/* Select SPI Bus controller */
#define CONFIG_ENV_SPI_BUS				0
/* Select SPI chip */
#define CONFIG_ENV_SPI_CS				0
/* Select chip max frequency */
#define CONFIG_ENV_SPI_MAX_HZ			10000000 /* 10 MHz */
/* Enviroment sector size */
#define CONFIG_ENV_SECT_SIZE			(CONFIG_ENV_SIZE)
/* Environment starts here  */
#define CONFIG_ENV_OFFSET				0x00080000

#endif

/*
 *-----------------------------------------------
 * Default boot parameters
 *-----------------------------------------------
 */
/* U-Boot config information */
#define CONFIG_ENV_VARS_UBOOT_CONFIG
/* Default variables */
#define CONFIG_CLOCKS_IN_MHZ	(CONFIG_SYS_CLK_FREQ / 1000000)
#define CONFIG_PCI_BOOTDELAY	3
#define CONFIG_BOOTARGS			"mem=128M console=ttyS0,mmio32,115200"
#define CONFIG_BOOTCOMMAND 		"sf probe; sf read 0x81000000 0x00400000 0x00400000; bootm 0x81000000"
#define CONFIG_RAMBOOTCOMMAND 	""
#define CONFIG_NFSBOOTCOMMAND	""
#define CONFIG_ETHPRIME			"eth0"
#define CONFIG_ETHADDR 			de:ad:be:ef:01:01
#define CONFIG_ETH1ADDR			de:ad:be:ef:01:02
#define CONFIG_ETH2ADDR			de:ad:be:ef:01:03
#define CONFIG_ETH3ADDR			de:ad:be:ef:01:04
#define CONFIG_ETH4ADDR			de:ad:be:ef:01:05
#define CONFIG_ETH5ADDR			de:ad:be:ef:01:06
#define CONFIG_IPADDR 			192.168.66.226
#define CONFIG_SERVERIP			192.168.66.151
#define CONFIG_GATEWAYIP		192.168.66.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_HOSTNAME			"baikal"
#define CONFIG_ROOTPATH			"/tftpboot/"
#define CONFIG_BOOTFILE			"uImage"
#define CONFIG_LOADADDR 		CONFIG_SYS_LOAD_ADDR

/*
 *-----------------------------------------------
 * Filesystem support
 *-----------------------------------------------
 */
#define CONFIG_FS_EXT2
#define CONFIG_FS_EXT3
#define CONFIG_FS_EXT4
#define CONFIG_EXT4_WRITE
#define CONFIG_FS_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FS_GENERIC
/*
 *-----------------------------------------------
 * Default commands
 *-----------------------------------------------
 */
#define CONFIG_CMD_BDI		/* bdinfo			*/
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_BOOTNR	/* bootnr 			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_EDITENV	/* editenv			*/
/*#define CONFIG_CMD_FPGA*/		/* FPGA configuration Support	*/
#define CONFIG_CMD_IMI		/* iminfo			*/
#define CONFIG_CMD_ITEST	/* Integer (and string) test	*/
#define CONFIG_CMD_LICENSE	/* console license display	*/
#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_PING		/* ping cmd			*/
#define CONFIG_CMD_RARP		/* rarp cmd			*/
#define CONFIG_CMD_DNS		/* dns support			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_NFS		/* NFS support			*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/
#define CONFIG_CMD_XIMG		/* Load part of Multi Image	*/
#define CONFIG_CMD_DHCP		/* dhcp client */
#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_EXT3		/* EXT3 Support			*/
#define CONFIG_CMD_EXT4		/* EXT4 Support			*/
#define CONFIG_CMD_FAT 		/* FAT support			*/
/*#define CONFIG_CMD_CRAMFS*/	/* CramFS Support (obsolete) */
#define CONFIG_CMD_REGINFO	/* Register dump		*/
#define CONFIG_CMD_READ		/* Read data from partition	*/
#define CONFIG_CMD_GPIO		/* GPIO manipulation command	*/

#define CONFIG_BOOTSTAGE_FDT
#define CONFIG_BOOTSTAGE_REPORT

/*#define CONFIG_CMD_TIME*/ 	/* Time suppoer commands */
/*#define CONFIG_CMD_GETTIME*/	/* Time suppoer commands */
#define CONFIG_CMD_MTDPARTS /* MTD partitions support */
#define CONFIG_MTD_CMDLINE_PARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_MENU			/* User-frendly menu style */

#endif /* __BAIKAL_MIPS_H__ */
