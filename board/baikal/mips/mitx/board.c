/*
 * File:        board.c
 * Description: T-platforms MITX Baikal-T1 based board initialization
 * Author:      Konstantin Kirik <dmitry.dunaev@baikalelectronics.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <environment.h>
#include <spi_flash.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <miiphy.h>
//#include <sata.h>
#include <i2c.h>
#include <netdev.h>
#include <malloc.h>
#include <asm/gpio.h>
#include <asm/arch/sata.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>

#include "fru.h"
#include "bootconf.h"

DECLARE_GLOBAL_DATA_PTR;

/* Set enviroment variables of all mac address from EEPROM. */
static void __maybe_unused setall_macaddr(void)
{
    uchar enetaddr[6];
    char name[32];
    int i;

    for (i = 0; i < CONFIG_FRU_MAX_IFACES; ++i) {
        switch (i) {
        case 0:
            memcpy(enetaddr, fru.mac0, 6);
            break;
        case 1:
            memcpy(enetaddr, fru.mac1, 6);
            break;
        case 2:
            memcpy(enetaddr, fru.mac2, 6);
            break;
        default:
            break;
        }

        sprintf(name, i ? "eth%daddr" : "ethaddr", i);
        eth_setenv_enetaddr(name, enetaddr);
    }
}

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
    memset(fru.bootdevice, 0, FRU_STR_MAX);
    fru_open_parse();

    printf("P/N:   %s\n", fru.val_part_number);
    printf("S/N:   %s\n", fru.val_serial_number);

    tp_reset_peripherals();

    /* Return success */
    return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
    tp_bmc_get_version();
    tp_check_boot();

    if (fru.bootdevice[0] != 0) {
        setenv("sata_dev", (char *)fru.bootdevice);
        static const char disk1[] = "sata0:0";
        static const char disk2[] = "sata0:1";
        if (memcmp(fru.bootdevice, disk2, strlen(disk2))==0) {
            printf("FRU: boot from %s\n", disk2);
        } else if (memcmp(fru.bootdevice, disk1, strlen(disk1))==0) {
            printf("FRU: boot from %s\n", disk1);
        } else {
            printf("FRU: unknown boot device %s, falling back to %s\n", fru.bootdevice, disk1);
        }
    }

    /* Set mac addresses */
    setall_macaddr();
    
    setenv("board_serial", (char *)fru.val_serial_number);

    setenv("board_rev", (char *)fru.val_part_number);

    setenv("board_name", (char *)fru.val_product_name);

#ifdef CONFIG_SATA_PHY_INIT
    unsigned int phy = getenv_ulong("sataphy", 10, 0);

    /* Init SATA PHY */
    if (phy) {
        sata_phy_init_val(0, phy);
        sata_phy_init_val(1, phy);
    }
    else
        sata_phy_init();
    printf("SATA:  PHY init complete\n");
#endif
        return 0;
}
#endif /* CONFIG_BOARD_LATE_INIT */

/* Initialization of network */
int board_eth_init(bd_t *bis)
{
    int err = 0;

#if defined(CONFIG_DESIGNWARE_ETH0_BASE)
    if (designware_initialize(CONFIG_DESIGNWARE_ETH0_BASE,
                              PHY_INTERFACE_MODE_RGMII) < 0)
        err |= (1 << 0);
#endif /* CONFIG_DESIGNWARE_ETH0_BASE */
#if defined(CONFIG_DESIGNWARE_ETH1_BASE)
    if (designware_initialize(CONFIG_DESIGNWARE_ETH1_BASE,
                              PHY_INTERFACE_MODE_RGMII) < 0)
        err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH1_BASE */
#if defined(CONFIG_DESIGNWARE_ETH2_BASE)
    if (designware_initialize(CONFIG_DESIGNWARE_ETH2_BASE,
                              PHY_INTERFACE_MODE_RGMII) < 0)
        err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH2_BASE */
    return (! err);
}
