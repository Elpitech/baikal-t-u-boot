// SPDX-License-Identifier: GPL-2.0
/*
 * File:	board.c
 * Description:	T-platforms SBC101 Baikal-T1 based board initialization
 * Authors:
 *
 * Copyright 2020 T-platforms JSC
 */

#include <common.h>
#include <dm.h>
#include <scsi.h>
#include <usb.h>
#include <i2c.h>
#include <stdio_dev.h>
#include <asm/gpio.h>
#include <../common/fru.h>

DECLARE_GLOBAL_DATA_PTR;

#define USB_HUB_NAME	"usb_hub@2c"

/* USB hub initialization data */
static u8 usb_hub_cfg0[] = {
	17, /* size of data */
	0x24, 0x04, 0x17, 0x25,
	0x00, 0x00, 0x9b, 0x20,
	0x00, 0x00, 0x00, 0x00,
	0x32, 0x32, 0x32, 0x32,
	0x32
};

static u8 usb_hub_cfgff[] = {
	1,
	0x01 /* .[0xff] = 1 - enable */
};

int board_usb_hub_configure(void)
{
	int rc;
	struct udevice *dev;

	rc = uclass_get_device_by_name(UCLASS_MISC, USB_HUB_NAME, &dev);
	if (rc) {
		printf("%s not found (%d)\n", USB_HUB_NAME, rc);
		return rc;
	}
	rc = dm_i2c_write(dev, 0, usb_hub_cfg0, sizeof(usb_hub_cfg0));
	if (rc) {
		printf("usb_hub_cfg0 write failed (%d)\n", rc);
		return rc;
	}
	rc = dm_i2c_write(dev, 0xff, usb_hub_cfgff, sizeof(usb_hub_cfgff));
	if (rc)
		printf("usb_hub_cfgff write failed (%d)\n", rc);

	return rc;
}

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

	udelay(1000);
	board_usb_hub_configure();

	return 0;
}

int board_misc_init(void)
{
	struct gpio_desc tp_gpio;
	int rc;

#ifdef TP_EN_IO_GPIO
	rc = dm_gpio_lookup_name(TP_EN_IO_GPIO, &tp_gpio);
	if (rc) {
		printf("%s: can't lookup EN_IO (%s) gpio (%d)\n",
		       __func__, TP_EN_IO_GPIO, rc);
		return rc;
	}

	rc = dm_gpio_request(&tp_gpio, "en_io");
	if (rc) {
		printf("%s: can't request gpio (%d)\n", __func__, rc);
		return rc;
	}

	rc = dm_gpio_set_dir_flags(&tp_gpio, GPIOD_IS_OUT);
	if (!rc)
		rc = dm_gpio_set_value(&tp_gpio, 1);
	if (rc) {
		printf("%s: can't set gpio dir/value (%d)\n", __func__, rc);
		return rc;
	}
#endif
#ifdef TP_PCIE_CLK_EN_GPIO
	rc = dm_gpio_lookup_name(TP_PCIE_CLK_EN_GPIO, &tp_gpio);
	if (rc) {
		printf("%s: can't lookup PCIE_CLK_EN (%s) gpio (%d)\n",
		       __func__, TP_PCIE_CLK_EN_GPIO, rc);
		return rc;
	}

	rc = dm_gpio_request(&tp_gpio, "pcie_clk_en");
	if (rc) {
		printf("%s: can't request gpio (%d)\n", __func__, rc);
		return rc;
	}

	rc = dm_gpio_set_dir_flags(&tp_gpio, GPIOD_IS_OUT);
	if (!rc)
		rc = dm_gpio_set_value(&tp_gpio, 1);
	if (rc) {
		printf("%s: can't set gpio dir/value (%d)\n", __func__, rc);
		return rc;
	}
#endif
	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	char *stdio_name;

	debug("sbc101: board_late_init\n");
	board_usb_reset();
	board_misc_init();
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
