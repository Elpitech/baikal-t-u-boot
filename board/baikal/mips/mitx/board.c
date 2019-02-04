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

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
    fru_open_parse();

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
