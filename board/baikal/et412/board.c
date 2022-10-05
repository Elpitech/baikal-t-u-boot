// SPDX-License-Identifier: GPL-2.0
/*
 * File:	board.c
 * Description:	Elpitech ET412-EM Baikal-T1 based board initialization
 * Authors:
 *
 * Copyright 2022 Elpitech
 */

#include <common.h>
#include <dm.h>
#include <scsi.h>
#include <usb.h>
#include <i2c.h>
#include <stdio_dev.h>
#include <asm/gpio.h>
#include <../common/fru.h>
#include <linux/delay.h>
#include <init.h>
#include <env.h>

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
	udelay(100);
	if (!rc)
		rc = dm_gpio_set_value(&usb_reset_gpio, 1);
	if (rc) {
		printf("board_usb_reset: can't set gpio dir/value (%d)\n", rc);
		return rc;
	}

	udelay(1000);

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	char *stdio_name;

	debug("et412: board_late_init\n");
	board_usb_reset();
#ifdef CONFIG_DM_PCI
	pci_init();
#ifdef CONFIG_DM_VIDEO
	stdio_name = env_get("stdout");
	if (stdio_name && strstr(stdio_name, "vidconsole"))
		env_set("stdout", stdio_name);
#endif
#endif
#ifdef CONFIG_DM_USB
	usb_init();
#ifdef CONFIG_USB_KEYBOARD
	stdio_name = env_get("stdin");
	if (stdio_name && strstr(stdio_name, "usbkbd"))
		env_set("stdin", stdio_name);
#endif
#endif
#ifdef CONFIG_DM_SCSI
	scsi_scan(false);
#endif
	tp_check_fru();

	return 0;
}
#endif
