/*
 * (C) Copyright 2009
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include "designware_i2c.h"

#ifdef CONFIG_I2C_MULTI_BUS
static unsigned int bus_initialized[CONFIG_SYS_I2C_BUS_MAX];
int i2c_get_bus_num(void);
#endif

#if CONFIG_SYS_SPD_BUS_NUM == 1
static struct i2c_regs *i2c_regs_p = (struct i2c_regs *)CONFIG_SYS_I2C_BASE1;
#else
static struct i2c_regs *i2c_regs_p = (struct i2c_regs *)CONFIG_SYS_I2C_BASE;
#endif

long baikal_pll_get(int id);
/*
 * set_speed - Set the i2c speed mode (standard, high, fast)
 * @i2c_spd:	required i2c speed mode
 *
 * Set the i2c speed mode (standard, high, fast)
 */
static void set_speed(int i2c_spd)
{
	unsigned int cntl;
	unsigned int hcnt, lcnt;
	unsigned int enbl;
/* Set IC_CLK by ethernet_pll freq */
	int eth_pll = 14;
	long eth_pll_val = baikal_pll_get(eth_pll);
	int ic_clk = eth_pll_val / 1000000 / 10;	/* Hz->MHz->MHz/10 */
	/* Calculate and initialize IC_SDA_HOLD */
	int t_ic_clk = (1000 / ic_clk); 	/* T of ic_clk in nS */
	int ic_sda_hold = 640; 			/* SDA hold 640 nS check i2c bus spec */
	cntl = ic_sda_hold / t_ic_clk;
/* End */
	/* to set speed cltr must be disabled */
	enbl = readl(&i2c_regs_p->ic_enable);
	enbl &= ~IC_ENABLE_0B;
	writel(enbl, &i2c_regs_p->ic_enable);

	writel(ic_sda_hold / t_ic_clk, &i2c_regs_p->ic_sda_hold);	/* Write ic_sda_hold into register */

	cntl = (readl(&i2c_regs_p->ic_con) & (~IC_CON_SPD_MSK));

	switch (i2c_spd) {
	case IC_SPEED_MODE_MAX:
		cntl |= IC_CON_SPD_HS;
		hcnt = (ic_clk * MIN_HS_SCL_HIGHTIME) / NANO_TO_MICRO;
		writel(hcnt, &i2c_regs_p->ic_hs_scl_hcnt);
		lcnt = (ic_clk * MIN_HS_SCL_LOWTIME) / NANO_TO_MICRO;
		writel(lcnt, &i2c_regs_p->ic_hs_scl_lcnt);
		break;

	case IC_SPEED_MODE_STANDARD:
		cntl |= IC_CON_SPD_SS;
		hcnt = (ic_clk * MIN_SS_SCL_HIGHTIME) / NANO_TO_MICRO;
		writel(hcnt, &i2c_regs_p->ic_ss_scl_hcnt);
		lcnt = (ic_clk * MIN_SS_SCL_LOWTIME) / NANO_TO_MICRO;
		writel(lcnt, &i2c_regs_p->ic_ss_scl_lcnt);
		break;

	case IC_SPEED_MODE_FAST:
	default:
		cntl |= IC_CON_SPD_FS;
		hcnt = (ic_clk * MIN_FS_SCL_HIGHTIME) / NANO_TO_MICRO;
		writel(hcnt, &i2c_regs_p->ic_fs_scl_hcnt);
		lcnt = (ic_clk * MIN_FS_SCL_LOWTIME) / NANO_TO_MICRO;
		writel(lcnt, &i2c_regs_p->ic_fs_scl_lcnt);
		break;
	}

#ifdef IC_SDA_HOLD
	writel(IC_SDA_HOLD, &i2c_regs_p->ic_sda_hold);
#endif
	writel(cntl, &i2c_regs_p->ic_con);

	/* Enable back i2c now speed set */
	enbl |= IC_ENABLE_0B;
	writel(enbl, &i2c_regs_p->ic_enable);
}

/*
 * i2c_set_bus_speed - Set the i2c speed
 * @speed:	required i2c speed
 *
 * Set the i2c speed.
 */
int i2c_set_bus_speed(int speed)
{
	if (speed >= I2C_MAX_SPEED)
		set_speed(IC_SPEED_MODE_MAX);
	else if (speed >= I2C_FAST_SPEED)
		set_speed(IC_SPEED_MODE_FAST);
	else
		set_speed(IC_SPEED_MODE_STANDARD);

	return 0;
}

/*
 * i2c_get_bus_speed - Gets the i2c speed
 *
 * Gets the i2c speed.
 */
int i2c_get_bus_speed(void)
{
	u32 cntl;

	cntl = (readl(&i2c_regs_p->ic_con) & IC_CON_SPD_MSK);

	if (cntl == IC_CON_SPD_HS)
		return I2C_MAX_SPEED;
	else if (cntl == IC_CON_SPD_FS)
		return I2C_FAST_SPEED;
	else if (cntl == IC_CON_SPD_SS)
		return I2C_STANDARD_SPEED;

	return 0;
}

void i2c_hw_init(int speed, int slaveadd)
{
        unsigned int enbl;

        /* Disable i2c */
        enbl = readl(&i2c_regs_p->ic_enable);
        enbl &= ~IC_ENABLE_0B;
        writel(enbl, &i2c_regs_p->ic_enable);

        writel((IC_CON_SD | IC_CON_SPD_FS | IC_CON_MM), &i2c_regs_p->ic_con);
        writel(IC_RX_TL, &i2c_regs_p->ic_rx_tl);
        writel(IC_TX_TL, &i2c_regs_p->ic_tx_tl);
        i2c_set_bus_speed(speed);
        writel(IC_STOP_DET, &i2c_regs_p->ic_intr_mask);
        writel(slaveadd, &i2c_regs_p->ic_sar);

        /* Enable i2c */
        enbl = readl(&i2c_regs_p->ic_enable);
        enbl |= IC_ENABLE_0B;
        writel(enbl, &i2c_regs_p->ic_enable);
}

/*
 * i2c_init - Init function
 * @speed:	required i2c speed
 * @slaveadd:	slave address for the device
 *
 * Initialization function.
 */
void i2c_init(int speed, int slaveadd)
{
	i2c_hw_init(speed, slaveadd);

#ifdef CONFIG_I2C_MULTI_BUS
	bus_initialized[i2c_get_bus_num()] = 1;
#endif
}


/*
 * i2c_setaddress - Sets the target slave address
 * @i2c_addr:	target i2c address
 *
 * Sets the target slave address.
 */
static void i2c_setaddress(unsigned int i2c_addr)
{
	unsigned int enbl;

	/* Disable i2c */
	enbl = readl(&i2c_regs_p->ic_enable);
	enbl &= ~IC_ENABLE_0B;
	writel(enbl, &i2c_regs_p->ic_enable);

	writel(i2c_addr, &i2c_regs_p->ic_tar);

	/* Enable i2c */
	enbl = readl(&i2c_regs_p->ic_enable);
	enbl |= IC_ENABLE_0B;
	writel(enbl, &i2c_regs_p->ic_enable);
}

/*
 * i2c_flush_rxfifo - Flushes the i2c RX FIFO
 *
 * Flushes the i2c RX FIFO
 */
static void i2c_flush_rxfifo(void)
{
	while (readl(&i2c_regs_p->ic_status) & IC_STATUS_RFNE)
		readl(&i2c_regs_p->ic_cmd_data);
}

/*
 * i2c_wait_for_bb - Waits for bus busy
 *
 * Waits for bus busy
 */
static int i2c_wait_for_bb(void)
{
	unsigned long start_time_bb = get_timer(0);

	while ((readl(&i2c_regs_p->ic_status) & IC_STATUS_MA) ||
	       !(readl(&i2c_regs_p->ic_status) & IC_STATUS_TFE)) {

		/* Evaluate timeout */
		if (get_timer(start_time_bb) > (unsigned long)(I2C_BYTE_TO_BB))
			return 1;
	}

	return 0;
}

static int i2c_xfer_init(uchar chip, uint addr, int alen)
{
	if (i2c_wait_for_bb())
		return 1;

	i2c_setaddress(chip);
	while (alen) {
		alen--;
		/* high byte address going out first */
		writel((addr >> (alen * 8)) & 0xff,
		       &i2c_regs_p->ic_cmd_data);
	}
	return 0;
}

static int i2c_xfer_finish(void)
{
	ulong start_stop_det = get_timer(0);

	while (1) {
		if ((readl(&i2c_regs_p->ic_raw_intr_stat) & IC_STOP_DET)) {
			readl(&i2c_regs_p->ic_clr_stop_det);
			break;
		} else if (get_timer(start_stop_det) > I2C_STOPDET_TO) {
			break;
		}
	}

	if (i2c_wait_for_bb()) {
		printf("Timed out waiting for bus\n");
		return 1;
	}

	i2c_flush_rxfifo();

	return 0;
}

/*
 * i2c_read - Read from i2c memory
 * @chip:	target i2c address
 * @addr:	address to read from
 * @alen:
 * @buffer:	buffer for read data
 * @len:	no of bytes to be read
 *
 * Read from i2c memory.
 */
int i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	unsigned long start_time_rx;

#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	chip |= ((addr >> (alen * 8)) & CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW);
	addr &= ~(CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW << (alen * 8));

	debug("%s: fix addr_overflow: chip %02x addr %02x\n", __func__, chip,
	      addr);
#endif

	if (i2c_xfer_init(chip, addr, alen))
		return 1;

	start_time_rx = get_timer(0);
	while (len) {
		if (len == 1)
			writel(IC_CMD | IC_STOP, &i2c_regs_p->ic_cmd_data);
		else
			writel(IC_CMD, &i2c_regs_p->ic_cmd_data);

		if (readl(&i2c_regs_p->ic_status) & IC_STATUS_RFNE) {
			*buffer++ = (uchar)readl(&i2c_regs_p->ic_cmd_data);
			len--;
			start_time_rx = get_timer(0);

		} else if (get_timer(start_time_rx) > I2C_BYTE_TO) {
				return 1;
		}
	}

	return i2c_xfer_finish();
}

/*
 * i2c_write - Write to i2c memory
 * @chip:	target i2c address
 * @addr:	address to read from
 * @alen:
 * @buffer:	buffer for read data
 * @len:	no of bytes to be read
 *
 * Write to i2c memory.
 */
int i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int nb = len;
	unsigned long start_time_tx;

#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	chip |= ((addr >> (alen * 8)) & CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW);
	addr &= ~(CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW << (alen * 8));

	debug("%s: fix addr_overflow: chip %02x addr %02x\n", __func__, chip,
	      addr);
#endif

	if (i2c_xfer_init(chip, addr, alen))
		return 1;

	start_time_tx = get_timer(0);
	while (len) {
		if (readl(&i2c_regs_p->ic_status) & IC_STATUS_TFNF) {
			if (--len == 0)
				writel(*buffer | IC_STOP, &i2c_regs_p->ic_cmd_data);
			else
				writel(*buffer, &i2c_regs_p->ic_cmd_data);
			buffer++;
			start_time_tx = get_timer(0);

		} else if (get_timer(start_time_tx) > (nb * I2C_BYTE_TO)) {
				printf("Timed out. i2c write Failed\n");
				return 1;
		}
	}

	return i2c_xfer_finish();
}

/*
 * i2c_probe - Probe the i2c chip
 */
int i2c_probe(uchar chip)
{
	u32 tmp;
	int ret;

	/*
	 * Try to read the first location of the chip.
	 */
	ret = i2c_read(chip, 0, 1, (uchar *)&tmp, 1);
	if (ret)
		i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	return ret;
}

#ifdef CONFIG_I2C_MULTI_BUS
int i2c_set_bus_num(unsigned int bus)
{
	switch (bus) {
	case 0:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE;
		break;
#ifdef CONFIG_SYS_I2C_BASE1
	case 1:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE1;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE2
	case 2:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE2;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE3
	case 3:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE3;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE4
	case 4:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE4;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE5
	case 5:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE5;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE6
	case 6:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE6;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE7
	case 7:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE7;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE8
	case 8:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE8;
		break;
#endif
#ifdef CONFIG_SYS_I2C_BASE9
	case 9:
		i2c_regs_p = (void *)CONFIG_SYS_I2C_BASE9;
		break;
#endif
	default:
		printf("Bad bus: %d\n", bus);
		return -1;
	}

	if (!bus_initialized[bus])
		i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	return 0;
}

int i2c_get_bus_num(void)
{
	switch ((unsigned int)i2c_regs_p) {
	case CONFIG_SYS_I2C_BASE:
		return 0;
#ifdef CONFIG_SYS_I2C_BASE1
	case CONFIG_SYS_I2C_BASE1:
		return 1;
#endif
#ifdef CONFIG_SYS_I2C_BASE2
	case CONFIG_SYS_I2C_BASE2:
		return 2;
#endif
#ifdef CONFIG_SYS_I2C_BASE3
	case CONFIG_SYS_I2C_BASE3:
		return 3;
#endif
#ifdef CONFIG_SYS_I2C_BASE4
	case CONFIG_SYS_I2C_BASE4:
		return 4;
#endif
#ifdef CONFIG_SYS_I2C_BASE5
	case CONFIG_SYS_I2C_BASE5:
		return 5;
#endif
#ifdef CONFIG_SYS_I2C_BASE6
	case CONFIG_SYS_I2C_BASE6:
		return 6;
#endif
#ifdef CONFIG_SYS_I2C_BASE7
	case CONFIG_SYS_I2C_BASE7:
		return 7;
#endif
#ifdef CONFIG_SYS_I2C_BASE8
	case CONFIG_SYS_I2C_BASE8:
		return 8;
#endif
#ifdef CONFIG_SYS_I2C_BASE9
	case CONFIG_SYS_I2C_BASE9:
		return 9;
#endif
	default:
		printf("Bad i2c bus regs ptr: %p\n", i2c_regs_p);
		return -1;
	}
}
#endif
