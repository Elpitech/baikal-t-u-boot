// SPDX-License-Identifier: GPL-2.0
/*
 * Baikal-T memory initialization
 */

#include <common.h>
#include <dm.h>
#include <i2c_eeprom.h>
#include <i2c.h>
#include <asm/baikal_hw.h>
#include <asm/ddr.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_CUSTOM_SPD
extern const unsigned char ddr_user_spd[];
#endif

int read_spd(unsigned char *buf, int len)
{
	int rc = -1;
	unsigned cov, crc;

#ifdef BAIKAL_SPD_NAME
	struct udevice *dev;

	rc = uclass_get_device_by_name(UCLASS_I2C_EEPROM, BAIKAL_SPD_NAME, &dev);
	if (rc) {
		printf("find eeprom failed (%d)\n", rc);
		goto try_next;
	}
	rc = i2c_eeprom_read(dev, 0, buf, len);
	if (rc) {
		printf("i2c_eeprom_read failed (%d)\n", rc);
		goto try_next;
	}
	cov = (buf[0] & (1 << 7)) ? 117 : 126;
	crc = crc16_ccitt(0, buf, cov);
	if (crc != (((unsigned)buf[127] << 8) | buf[126])) {
		rc = -1;
		printf("I2C SPD crc16 failed: %04x != %02x%02x\n",
			crc, buf[126], buf[127]);
	} else {
		return 0;
	}
try_next:
#endif

#ifdef CONFIG_CUSTOM_SPD
	memcpy(buf, ddr_user_spd, len);
	cov = (buf[0] & (1 << 7)) ? 117 : 126;
	crc = crc16_ccitt(0, buf, cov);
	if (crc != (((unsigned)buf[127] << 8) | buf[126])) {
		rc = -1;
		printf("Custom SPD crc16 failed: %04x != %02x%02x\n",
			crc, buf[126], buf[127]);
	} else {
		return 0;
	}
#endif
	puts("No valid SPD data available!\n");
	return rc;
}

int dram_init(void)
{
	uint32_t ddr_regs[64];
	unsigned char *spd_buf;
	struct ddr3_timings spd_t;
	struct ddr3_geometry spd_g;
	unsigned mem_size_mb;
	int rc;

	spd_buf = (unsigned char *)ddr_regs;
	rc = read_spd(spd_buf, 128);
	if (rc)
		return -1;

	rc = ddr_parse_spd(spd_buf, &spd_t, &spd_g);
	if (rc) {
		puts("SPD parse error\n");
		return -1;
	}

	mem_size_mb = spd_g.whole_mem;

	rc = ddr_prepare_regs(&spd_t, &spd_g, ddr_regs);
	if (rc)
		puts("prepare_regs failed?\n");
#ifdef DEBUG
	{
		int i;
		for (i = 0; i < DDR_SPD_LAST; i++)
			debug("Reg %02i: %08x\n", i, ddr_regs[i]);
	}
#endif
	rc = ddr_init(ddr_regs);
	if (rc) {
		puts("DDR init failed\n");
		return -1;
	}
	printf("%uMB, %s%d-bit\n", mem_size_mb,
		ddr_regs[DDR3_SPD_ECCCFG0]?"ECC, ":"",
		(spd_g.bus_width >= 32)?32:16);

	gd->ram_size = DDR_BANK0_SIZE; // 128MB

	return 0;
}

phys_size_t get_effective_memsize(void)
{
	return DDR_BANK0_SIZE;
}
