// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020 T-Platforms
 */

#include <common.h>
#include <dm.h>
#include <misc.h>
#include <i2c.h>

static int i2c_misc_read(struct udevice *dev, int offset, void *buf,
			       int size)
{
	return dm_i2c_read(dev, offset, buf, size);
}

static int i2c_misc_write(struct udevice *dev, int offset,
				const void *buf, int size)
{
	return dm_i2c_write(dev, offset, buf, size);
}

static const struct misc_ops i2c_misc_ops = {
	.read	= i2c_misc_read,
	.write	= i2c_misc_write,
};

static int i2c_misc_probe(struct udevice *dev)
{
	debug("i2c_misc_probe\n");
	return 0;
}

static const struct udevice_id i2c_misc_ids[] = {
	{ .compatible = "i2c-generic" },
	{ .compatible = "microchip,usb251x" },
	{ }
};

U_BOOT_DRIVER(i2c_misc) = {
	.name			= "i2c_misc",
	.id			= UCLASS_MISC,
	.of_match		= i2c_misc_ids,
	.probe			= i2c_misc_probe,
	.ops			= &i2c_misc_ops,
};
