/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: CPC header file.
 */

#ifndef __CPC_H__
#define __CPC_H__

#include "mem_map.h"

/* Cluster Power Controller Map. */
#define GCR_CPC_BASE		CPC_BASE
#define CPC_GLOBAL_BLOCK	GCR_CPC_BASE
#define CPC_CORE_LOCAL_BLOCK	(GCR_CPC_BASE + 0x2000)
#define CPC_CORE_OTHER_BLOCK	(GCR_CPC_BASE + 0x4000)

/* Global Control Block. */
#define CPC_ACCESS_REG		(CPC_GLOBAL_BLOCK + 0x000)	/* CPC Global CSR Access Privilege Register */
#define CPC_SEQDEL_REG		(CPC_GLOBAL_BLOCK + 0x008)	/* CPC Global Sequence Delay Counter */
#define CPC_RAIL_REG		(CPC_GLOBAL_BLOCK + 0x010)	/* CPC Global Rail Delay Counter Register */
#define CPC_RESETLEN_REG	(CPC_GLOBAL_BLOCK + 0x018)	/* CPC Global Reset Width Counter Register */
#define CPC_REVISION_REG	(CPC_GLOBAL_BLOCK + 0x020)	/* CPC Global Revision Register */

/* Core-Local Block Register */
#define CPC_CL_CMD_REG		(CPC_CORE_LOCAL_BLOCK + 0x000)	/* CPC Local Command Register */
#define CPC_CL_STAT_CONF_REG	(CPC_CORE_LOCAL_BLOCK + 0x008)	/* CPC Local Status and Configuration register */
#define CPC_CL_OTHER_REG	(CPC_CORE_LOCAL_BLOCK + 0x010)	/* CPC Core Other Addressing Register */

/* CPC_CL_OTHER */
#define CPC_CL_OTHER_PCORE_POS	16
#define CPC_CL_OTHER_PCORE_BITS	8

/* Core-Other Block Register */
#define CPC_CO_CMD_REG		(CPC_CORE_OTHER_BLOCK + 0x000)	/* CPC Local Command Register */
#define CPC_CO_STAT_CONF_REG	(CPC_CORE_OTHER_BLOCK + 0x008)	/* CPC Local Status and Configuration register */
#define CPC_CO_OTHER_REG	(CPC_CORE_OTHER_BLOCK + 0x010)	/* CPC Core Other Addressing Register */

/* CPC_CO_CMD_REG */
#define CMD_CLOCK_OFF	1
#define CMD_PWRDOWN	2
#define CMD_PWRUP	3
#define CMD_RESET	4

/* CPC_CO_STAT_CONF_REG */
#define SEQ_STATE_SHIFT		19
#define SEQ_STATE_POS		SEQ_STATE_SHIFT
#define SEQ_STATE_BITS		4
#define SEQ_STATE_MASK		0x00780000
#define CPC_CO_STAT_EJTAG_PROBE	(1 << 15)

#define SEQ_STATE_D0	0x0 	/* PwrDwn */
#define SEQ_STATE_U0	0x1 	/* VddOK */
#define SEQ_STATE_U1	0x2 	/* UpDelay */
#define SEQ_STATE_U2	0x3 	/* UClkOff */
#define SEQ_STATE_U3	0x4 	/* Reset */
#define SEQ_STATE_U4	0x5 	/* ResetDly */
#define SEQ_STATE_U5	0x6 	/* nonCoherent execution */
#define SEQ_STATE_U6	0x7 	/* Coherent execution */
#define SEQ_STATE_D1	0x8 	/* Isolate */
#define SEQ_STATE_D3	0x9 	/* ClrBus */
#define SEQ_STATE_D2	0xa 	/* DClkOff */

#define BOOT_MAGIC	0xC001

#define READ_CPC_REG(r)       READ_MEMORY_REG(r)
#define WRITE_CPC_REG(r, v)   WRITE_MEMORY_REG(r, v)

/* 
 * TEST_CPC_REG_ID macro returns 0 if the reg register is to equal to
 * its reg_val default value.
 */
#define TEST_CPC_REG_ID TEST_MEM_REG_ID

/* 
 * TEST_CPC_READ macro returns 0 if the reg register can be read.
 */
#define TEST_CPC_READ   TEST_READ_MEM_REG


#endif /* __CPC_H__ */
