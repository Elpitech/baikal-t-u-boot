// SPDX-License-Identifier: GPL-2.0
/*
 * File:	board.c
 * Description:	T-platforms Azure Baikal-T1 based board initialization
 * Authors:
 *
 * Copyright 2020 T-platforms JSC
 */

#include <common.h>
#include <scsi.h>
#include <usb.h>
#include <clk.h>
#include <dm/uclass.h>
#include <asm/gpio.h>
#include <../common/fru.h>
#include <init.h>

DECLARE_GLOBAL_DATA_PTR;

int board_usb_reset(void)
{
	struct gpio_desc usb_reset_gpio;
	int rc;

	rc = dm_gpio_lookup_name(BAIKAL_USB_RESET_GPIO, &usb_reset_gpio);
	if (rc) {
		printf("board_usb_reset: can't lookup gpio (%d)\n", rc);
		return rc;
	}

	rc = dm_gpio_request(&usb_reset_gpio, "usb-reset");
	if (rc) {
		printf("board_usb_reset: can't request gpio (%d)\n", rc);
		return rc;
	}

	rc = dm_gpio_set_dir_flags(&usb_reset_gpio, GPIOD_IS_OUT);
	if (!rc)
		rc = dm_gpio_set_value(&usb_reset_gpio, 1);
	if (rc) {
		printf("board_usb_reset: can't set gpio dir/value (%d)\n", rc);
		return rc;
	}

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	struct udevice *clkdev;
	struct clk clk;
	int rc;

	debug("azure: board_late_init\n");

#ifdef BAIKAL_EXTRA_CLOCK_NAME
	rc = uclass_get_device_by_name(UCLASS_CLK, BAIKAL_EXTRA_CLOCK_NAME, &clkdev);
	if (rc) {
		printf("can't find clock dev (%d)\n", rc);
	} else {
		rc = clk_request(clkdev, &clk);
		if (rc) {
			printf("can't get clk (%d)\n", rc);
		} else {
			rc = clk_enable(&clk);
			clk_free(&clk);
		}
	}
#endif

	board_usb_reset();
#ifdef CONFIG_DM_PCI
	pci_init();
#endif
#ifdef CONFIG_DM_USB
	usb_init();
#endif
#ifdef CONFIG_DM_SCSI
	scsi_scan(false);
#endif
#ifdef CONFIG_TP_FRU
	tp_check_fru();
#endif

	return 0;
}
#endif
