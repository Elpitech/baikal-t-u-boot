/*
 * Configuration settings for the Baikal-T1 based SBC101 boards.
 *
 * Copyright (C) 2018-2019 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TP_SBC101_H
#define __CONFIG_TP_SBC101_H

/*
 *-----------------------------------------------
 * Default Baikal-T1 config macro
 *-----------------------------------------------
 */
#include <configs/baikal_mips.h>

/*
 *-----------------------------------------------
 * Generic Configurations
 *-----------------------------------------------
 */

#define BOARD_NAME			"RBBT"

/*
 *-----------------------------------------------
 * DDR parameters
 *-----------------------------------------------
 */
#define BAIKAL_SPD_NAME		"eeprom@54"

/*
 *-----------------------------------------------
 * Misc parameters
 *-----------------------------------------------
 */
#define BAIKAL_USB_RESET_GPIO	"porta13"

#define TP_EN_IO_GPIO   	"porta7"
#define TP_PCIE_CLK_EN_GPIO	"porta16"

#define TP_FRU_NAME		"eeprom@57"

#define BOARD_FRU_MAX_IFACES	2

/*
 *-----------------------------------------------
 * Default boot parameters
 *-----------------------------------------------
 */
/* U-Boot config information */
#define CONFIG_ENV_VARS_UBOOT_CONFIG
/* Default variables */
#define CONFIG_PCI_BOOTDELAY		3
#define CONFIG_BOOTARGS			"mem=128M console=ttyS0,mmio32,115200"
#define CONFIG_BOOTCOMMAND 		"sf probe; sf read 0x81000000 0x00400000 0x00400000; bootm 0x81000000"
#define CONFIG_RAMBOOTCOMMAND 		""
#define CONFIG_NFSBOOTCOMMAND		""
#define CONFIG_ETHPRIME			"eth0"
#define CONFIG_ETHADDR 			de:ad:be:ef:01:01
#define CONFIG_ETH1ADDR			de:ad:be:ef:01:02
#define CONFIG_ETH2ADDR			de:ad:be:ef:01:03
#define CONFIG_IPADDR 			192.168.66.226
#define CONFIG_SERVERIP			192.168.66.151
#define CONFIG_GATEWAYIP		192.168.66.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_HOSTNAME			"baikal"
#define CONFIG_ROOTPATH			"/tftpboot/"
#define CONFIG_BOOTFILE			"uImage"
#define CONFIG_LOADADDR 		CONFIG_SYS_LOAD_ADDR

#endif /* __CONFIG_TP_SBC101_H */
