/*
 * File:        board.c
 * Description: Board initialization for devices based on Baikal-T* SOCs 
 * Authors:     Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *              Ramil Zaripov <Ramil.Zaripov@baikalelectronics.com>
 *              Alexander Sazonov <Alexander.Sazonov@baikalelectronics.ru>
 * Copyright 2014-2017 Baikal Electronics JSC
 */

#include <common.h>
#include <environment.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <miiphy.h>
#include <i2c.h>
#include <netdev.h>
#include <malloc.h>
#include <asm/arch/sata.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>
#include "spi.h"
#include <board.h>

DECLARE_GLOBAL_DATA_PTR;

#define BRD15_BUS_NUM		0x01
#define BRD15_ADDR		0x51
#define BRD16_BUS_NUM		0x01
#define BRD16_ADDR		0x20

struct vendor_data vdata;

/*
 * Get board revision
 */
int get_board_revision(void)
{
    int old_bus, board_rev;

#ifndef CONFIG_BAIKAL_BFK3
    /* switch to correct I2C bus */
    old_bus = I2C_GET_BUS();
    I2C_SET_BUS(BRD15_BUS_NUM);

    /* Read control value */
    if (i2c_probe(BRD15_ADDR) == 0)
        board_rev = 0x15;
    else
        board_rev = 0x10;

    I2C_SET_BUS(BRD16_BUS_NUM);
    if (i2c_probe(BRD16_ADDR) == 0)
        board_rev = 0x16;

    /* switch back to original I2C bus */
    I2C_SET_BUS(old_bus);
#else
    board_rev = 0x31;
#endif /* CONFIG_BAIKAL_BFK3 */

    return board_rev;
}

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
    int updated = 0;

    vdata.crc = 0;
    vdata.type = 0xff;
    vdata.revision = 0x31;
    vdata.serial = 0x0;

    if (vdata.revision == 0xff) {
        vdata.revision = get_board_revision() & 0xff;
        updated = 1;
    }

    printf("Rev:   %u.%u\n", (vdata.revision >> 4) & 0x0f, vdata.revision & 0x0f);

    if (vdata.serial != 0) {
        printf("S/N:   %llu\n", vdata.serial);
    }

    /* Return success */
    return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

#ifdef CONFIG_BOARD_LATE_INIT

#ifdef CONFIG_BAIKAL_BFK3
#define ETH_ID_1G_1		1
#define ETH_ID_1G_2		2
#define ETH_ID_10G		0
int calc_and_set_macaddr(int serial, int rev)
{
	u64 macaddr;
	char buffer[11];
	u8* b=(u8*)&macaddr;

	/* Ethernet 10G (eth0) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_10G;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("ethaddr", buffer);

	/* Ethernet 1G (eth1) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_1G_1;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("eth1addr", buffer);

	/* Ethernet 1G (eth2) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_1G_2;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("eth2addr", buffer);
}
#endif /* CONFIG_BAIKAL_BFK3 */

#define BOOT_FLASH_ID_LEN 20

int board_late_init(void)
{
    char buf[32];

    /* Set board serial number */
    snprintf(buf, 20, "%llu", vdata.serial);
    setenv("board_serial", buf);

    /* Set board revision */
    snprintf(buf, 10, "%u.%u", (vdata.revision >> 4) & 0x0f,
            vdata.revision & 0x0f);
    setenv("board_rev", buf);

    /* Set board name */
    setenv("board_name", CONFIG_BAIKAL_NAME);

    return 0;
}
#endif /* CONFIG_BOARD_LATE_INIT */

/* Init SPI interface */
#if 0
void spi_init(void)
{
    /* Nothing todo here */
    return;
}
#endif


/*
 * Show board name at startup
 */
int checkboard(void)
{
#if defined(CONFIG_BAIKAL_T1)
	puts("Board: Baikal-T1 " CONFIG_BAIKAL_NAME "\n");
#else
	puts("Board: Baikal-T " CONFIG_BAIKAL_NAME "\n");
#endif
	return 0;
}

#ifdef CONFIG_CMD_EEPROM
int eeprom_write_enable (unsigned dev_addr, int state)
{
    return 0;
}
#endif
/* End of file board.c */


