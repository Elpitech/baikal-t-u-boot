// SPDX-License-Identifier: GPL-2.0+
/*
 *
 */

#include <common.h>
#include <dm.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <errno.h>

struct dwmac_gpio_regs {
	u32 data;
};

struct dwmac_gpio_priv {
	struct dwmac_gpio_regs *regs;
	u32 ngpo;
	u32 ngpi;
	char name[16];
};

static int dwmac_gpio_direction_input(struct udevice *dev, unsigned int gpio)
{
	struct dwmac_gpio_priv *priv = dev_get_priv(dev);

	if (gpio < priv->ngpo || gpio >= (priv->ngpo + priv->ngpi))
		return -EINVAL;

	return 0;
}

static int dwmac_gpio_direction_output(struct udevice *dev, unsigned gpio,
				       int value)
{
	struct dwmac_gpio_priv *priv = dev_get_priv(dev);
	struct dwmac_gpio_regs *regs = priv->regs;

	if (gpio >= priv->ngpo)
		return -EINVAL;

	debug("dwmac_gpio_output: pin %d val %d\n", gpio, value);
	if (value)
		setbits_le32(&regs->data, BIT(gpio + 8));
	else
		clrbits_le32(&regs->data, BIT(gpio + 8));

	return 0;
}

static int dwmac_gpio_get_function(struct udevice *dev, unsigned gpio)
{
	struct dwmac_gpio_priv *priv = dev_get_priv(dev);

	if (gpio < priv->ngpo)
		return GPIOF_OUTPUT;
	else if (gpio < (priv->ngpo + priv->ngpi))
		return GPIOF_INPUT;
	else
		return -EINVAL;
}

static int dwmac_gpio_set_value(struct udevice *dev, unsigned gpio,
				int value)
{
	struct dwmac_gpio_priv *priv = dev_get_priv(dev);
	struct dwmac_gpio_regs *regs = priv->regs;

	if (gpio >= priv->ngpo)
		return -EINVAL;

	debug("dwmac_gpio_set: pin %d val %d\n", gpio, value);
	if (value)
		setbits_le32(&regs->data, BIT(gpio + 8));
	else
		clrbits_le32(&regs->data, BIT(gpio + 8));

	return 0;
}

static int dwmac_gpio_get_value(struct udevice *dev, unsigned gpio)
{
	struct dwmac_gpio_priv *priv = dev_get_priv(dev);
	struct dwmac_gpio_regs *regs = priv->regs;

	if (gpio < priv->ngpo)
		return !!(readl(&regs->data) & BIT(gpio + 8));
	else if(gpio < (priv->ngpo + priv->ngpi))
		return !!(readl(&regs->data) & BIT(gpio - priv->ngpo));
	else
		return -EINVAL;
}

static int dwmac_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct dwmac_gpio_priv *priv = dev_get_priv(dev);

	priv->regs = (struct dwmac_gpio_regs *)devfdt_map_physmem(dev, 0x4);
	priv->ngpo = 1;
	priv->ngpi = 1;
	uc_priv->gpio_count = priv->ngpo + priv->ngpi;

	snprintf(priv->name, 15, "macgpio%d", dev->seq_);
	uc_priv->bank_name = priv->name;

	return 0;
}

static const struct dm_gpio_ops dwmac_gpio_ops = {
	.direction_input	= dwmac_gpio_direction_input,
	.direction_output	= dwmac_gpio_direction_output,
	.get_function		= dwmac_gpio_get_function,
	.get_value		= dwmac_gpio_get_value,
	.set_value		= dwmac_gpio_set_value,
};

static const struct udevice_id dwmac_gpio_ids[] = {
	{ .compatible = "snps,dwmac-gpio" },
	{ }
};

U_BOOT_DRIVER(gpio_dwmac) = {
	.name			= "gpio_dwmac",
	.id			= UCLASS_GPIO,
	.of_match		= dwmac_gpio_ids,
	.ops			= &dwmac_gpio_ops,
	.probe			= dwmac_gpio_probe,
	.priv_auto		= sizeof(struct dwmac_gpio_priv),
};
