// SPDX-License-Identifier: GPL-2.0+
/*
 * Baikal Electronics Baikal-T clock driver
 *
 * Loosely based on Linux kernel sci-clk.c...
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <clk-uclass.h>
#include <asm/io.h>
#include <asm/clk.h>
#include <dm/device_compat.h>
#include <linux/delay.h>

/**
 * struct be_pll_clk_data - clock controller information structure
 * @reg: PLL clock register
 */
struct be_pll_clk_data {
	void __iomem *reg;
	struct clk parent;
	struct clk self;
	u_int div_width;
	bool enabled;
};

static int be_pll_clk_probe(struct udevice *dev)
{
	struct be_pll_clk_data *data = dev_get_priv(dev);
	int ret;

	debug("%s(dev=%p)\n", __func__, dev);

	if (!data)
		return -ENOMEM;

	ret = clk_get_by_index(dev, 0, &data->parent);
	if (ret) {
		printf("%s: get parent failed (%d)\n", __func__, ret);
		return ret;
	}

	data->reg = devfdt_map_physmem(dev, sizeof(u32));
	if (!data->reg) {
		dev_err(dev, "no reg property?\n");
		return -EINVAL;
	}

	if (readl(data->reg) & BE_CLK_ENABLE)
		data->enabled = true;

	data->self.dev = dev;
	dev->uclass_priv_ = &data->self;

	if (data->parent.dev) {
		/* Re-parent clk device */
		list_del(&dev->sibling_node);
		list_add_tail(&dev->sibling_node, &data->parent.dev->child_head);
	}

	return 0;
}

static ulong be_pll_clk_get_rate(struct clk *clk)
{
	struct be_pll_clk_data *data = dev_get_priv(clk->dev);
	u32 reg, nf, nr, od;
	ulong parent_rate;
	ulong freq;

	debug("%s(clk=%p)\n", __func__, clk);

	if (!data->enabled)
		return 0;

	parent_rate = clk_get_rate(&data->parent);

	reg = readl(data->reg);
	nr = BE_RD_CLKR(reg);
	nf = BE_RD_CLKF(reg);
	od = BE_RD_CLKOD(reg);

	freq = parent_rate / (nr + 1) / (od + 1) * (nf + 1);

	debug("%s(current_freq=%lu)\n", __func__, freq);

	return freq;
}

static ulong be_pll_clk_set_rate(struct clk *clk, ulong rate)
{
	struct be_pll_clk_data *data = dev_get_priv(clk->dev);
	u32 reg, nf, nr, od;
	u32 div, rem;
	ulong parent_rate;
	ulong vco;
	int ret;

	debug("%s(clk=%p, rate=%lu)\n", __func__, clk, rate);

	ret = clk_enable(&data->parent);
	if (ret && ret != -ENOSYS) 
		return ret;

	parent_rate = clk_get_rate(&data->parent);
	if (!parent_rate)
		return -ENXIO;

	/* find the best VCO in range 700-3500MHz */
	od = 3500000000UL / rate;
	/* OD must be 1 or even in range 2..16 */
	if (od == 0)
		return -EINVAL; /* too high rate requested */
	else if (od > 16)
		od = 16;
	else if (od > 1)
		od &= ~1;

	vco = rate * od;

	div = 1;
	rem = vco % parent_rate;
	if (rem != 0) {
		if (rem > (parent_rate / 2))
			rem = parent_rate - rem;
		div = parent_rate / rem;
	}
	nr = div;
	nf = rate * nr / parent_rate * od;

	debug("%s: nf %d, nr %d, od %d, result %ld\n", __func__,
		nf, nr, od, parent_rate / od * nf / nr);

	reg = BE_PLL_CLK_VAL(nr, nf, od);
	writel(reg & ~BE_CLK_RESET, data->reg);
	udelay(10);
	writel(reg, data->reg);
	udelay(20);

	while(!(readl(data->reg) & BE_CLK_LOCK))
		;
	data->enabled = true;

	return 0;
}

static int be_pll_clk_enable(struct clk *clk)
{
	struct be_pll_clk_data *data = dev_get_priv(clk->dev);
	u32 reg;
	int ret;

	debug("%s(clk=%p)\n", __func__, clk);

	ret = clk_enable(&data->parent);
	if (ret && ret != -ENOSYS)
		return ret;

	if (data->self.enable_count++)
		return 0; /* already enabled */

	reg = readl(data->reg);
	reg |= BE_CLK_ENABLE;
	writel(reg, data->reg);
	data->enabled = true;

	return 0;
}

static int be_pll_clk_disable(struct clk *clk)
{
	struct be_pll_clk_data *data = dev_get_priv(clk->dev);
	u32 reg;

	debug("%s(clk=%p)\n", __func__, clk);

	if (--data->self.enable_count)
		return 0; /* still in use - should remain enabled */

	reg = readl(data->reg);
	reg &= ~BE_CLK_ENABLE;
	writel(reg, data->reg);
	data->enabled = false;

	return 0;
}

static const struct udevice_id be_pll_clk_of_match[] = {
	{ .compatible = "be,pmu-pll-clock" },
	{ /* sentinel */ },
};

static struct clk_ops be_pll_clk_ops = {
	.get_rate = be_pll_clk_get_rate,
	.set_rate = be_pll_clk_set_rate,
	.enable = be_pll_clk_enable,
	.disable = be_pll_clk_disable,
};

U_BOOT_DRIVER(be_pll_clk) = {
	.name = "be-pll-clk",
	.id = UCLASS_CLK,
	.of_match = be_pll_clk_of_match,
	.probe = be_pll_clk_probe,
	.priv_auto = sizeof(struct be_pll_clk_data),
	.ops = &be_pll_clk_ops,
};

static int be_pmu_clk_probe(struct udevice *dev)
{
	struct be_pll_clk_data *data = dev_get_priv(dev);
	int ret;

	debug("%s(dev=%p)\n", __func__, dev);

	if (!data)
		return -ENOMEM;

	ret = clk_get_by_index(dev, 0, &data->parent);
	if (ret) {
		printf("%s: get parent failed (%d)\n", __func__, ret);
		return ret;
	}

	data->reg = devfdt_map_physmem(dev, sizeof(u32));
	if (!data->reg) {
		dev_err(dev, "no reg property?\n");
		return -EINVAL;
	}

	ret = dev_read_u32(dev, "divider-width", &data->div_width);
	if (ret) {
		printf("%s: %s: divider-width not specified, assuming max (17)\n",
			__func__, dev->name);
		data->div_width = BE_CLK_DIV_MAX_WIDTH;
	}

	data->enabled = dev_read_bool(dev, "be,is-enabled");
	if (!data->enabled && (readl(data->reg) & BE_CLK_ENABLE))
		data->enabled = true;

	data->self.dev = dev;
	dev->uclass_priv_ = &data->self;

	if (data->parent.dev) {
		/* Re-parent clk device */
		list_del(&dev->sibling_node);
		list_add_tail(&dev->sibling_node, &data->parent.dev->child_head);
	}

	return 0;
}

static ulong be_pmu_clk_get_rate(struct clk *clk)
{
	struct be_pll_clk_data *data = dev_get_priv(clk->dev);
	u32 reg, div;
	ulong parent_rate;
	ulong freq;

	debug("%s(clk=%p)\n", __func__, clk);

	if (!data->enabled)
		return 0;

	parent_rate = clk_get_rate(&data->parent);

	reg = readl(data->reg);
	div = (reg & BE_CLK_DIV_MASK) >> BE_DIV_SHFT;

	div &= (1 << data->div_width) - 1;
	if (!div)
		div = 1;

	freq = parent_rate / div;

	debug("%s(current_freq=%lu)\n", __func__, freq);

	return freq;
}

static ulong be_pmu_clk_set_rate(struct clk *clk, ulong rate)
{
	struct be_pll_clk_data *data = dev_get_priv(clk->dev);
	u32 reg;
	u32 div;
	ulong parent_rate;
	int ret;

	debug("%s(clk=%p, rate=%lu)\n", __func__, clk, rate);

	ret = clk_enable(&data->parent);
	if (ret && ret != -ENOSYS)
		return ret;

	parent_rate = clk_get_rate(&data->parent);
	if (!parent_rate)
		return -ENXIO;

	div = parent_rate / rate;
	if (div > (1 << data->div_width))
		return -EINVAL;

	reg = readl(data->reg);
	reg &= ~BE_CLK_DIV_MASK;
	reg |= (div << BE_DIV_SHFT) | BE_CLK_ENABLE | BE_CLK_RESET;
	writel(reg, data->reg);

	while(!(readl(data->reg) & BE_CLK_LOCK))
		;
	data->enabled = true;

	return 0;
}

static const struct udevice_id be_pmu_clk_of_match[] = {
	{ .compatible = "be,pmu-device-clock" },
	{ /* sentinel */ },
};

static struct clk_ops be_pmu_clk_ops = {
	.get_rate = be_pmu_clk_get_rate,
	.set_rate = be_pmu_clk_set_rate,
	.enable = be_pll_clk_enable,
	.disable = be_pll_clk_disable,
};

U_BOOT_DRIVER(be_pmu_clk) = {
	.name = "be-pmu-clk",
	.id = UCLASS_CLK,
	.of_match = be_pmu_clk_of_match,
	.probe = be_pmu_clk_probe,
	.priv_auto = sizeof(struct be_pll_clk_data),
	.ops = &be_pmu_clk_ops,
};
