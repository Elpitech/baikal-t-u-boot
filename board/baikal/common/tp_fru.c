// SPDX-License-Identifier: GPL-2.0+
/*
 *
 */

#include <common.h>
#include "fru.h"
#include <dm.h>
#include <i2c_eeprom.h>
#include <i2c.h>
#include <net.h>

#define flog	printf
#ifndef BOARD_FRU_MAX_IFACES
#define BOARD_FRU_MAX_IFACES	1
#endif
#define FRU_SIZE		4096

struct fru fru;

void
print_board_area(struct fru *f) {
	flog("FRU Board area:\n");
	flog("Board mfg:          \t%s\n", f->mfg_name);
	flog("Board name:         \t%s\n", f->product_name);
	flog("Board serial number:\t%s\n", f->serial_number);
	flog("Board part number:  \t%s\n", f->part_number);
	flog("Board fru id:       \t%s\n", f->fru_id);
}

void
print_product_area(struct fru *f) {
	flog("FRU Product area:\n");
	flog("Product mfg:          \t%s\n", f->p_product_mfg);
	flog("Product name:         \t%s\n", f->p_product_name);
	flog("Product model number: \t%s\n", f->p_part_model_number);
	flog("Product version:      \t%s\n", f->p_product_version);
	flog("Product serial number:\t%s\n", f->p_serial_number);
	flog("Product fru id:       \t%s\n", f->p_fru_id);
}

int
read_fru(uint8_t *buf)
{
	int ret;
	struct udevice *dev;

	ret = uclass_get_device_by_name(UCLASS_I2C_EEPROM, TP_FRU_NAME, &dev);
	if (ret) {
		printf("FRU device not found (%d)\n", ret);
		return ret;
	}

	ret = i2c_eeprom_read(dev, 0, buf, FRU_SIZE);
	if (ret) {
		printf("Fru read failed (%d)\n", ret);
		return ret;
	}

	return FRU_SIZE;
}

int write_fru(uint8_t *buf, int off, int size)
{
	int ret;
	struct udevice *dev;

	ret = uclass_get_device_by_name(UCLASS_I2C_EEPROM, TP_FRU_NAME, &dev);
	if (ret) {
		printf("FRU device not found (%d)\n", ret);
		return ret;
	}

	ret = i2c_eeprom_write(dev, off, buf, size);
	if (ret) {
		printf("Fru write failed (%d)\n", ret);
		return ret;
	}

	return size;
}

static int fru_setall_macaddr(void)
{
	uint8_t enetaddr[6], old_addr[6];
	char name[32];
	int i, rc, ret = 0;

	for (i = 0; i < BOARD_FRU_MAX_IFACES; ++i) {
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

		if (is_valid_ethaddr(enetaddr)) {
			sprintf(name, i ? "eth%daddr" : "ethaddr", i);
			rc = eth_env_get_enetaddr(name, old_addr);
			if (!rc) { /* Address not set - it is safe to set it */
				rc = eth_env_set_enetaddr(name, enetaddr);
				if (rc) {
					printf("%s: Cannot set %s to %pM\n",
						__func__, name, enetaddr);
					ret++;
				}
			} else if (memcmp(enetaddr, old_addr, 6)) {
				/* Address is set but different from FRU */
				printf("%s mismatch:\n"
					"Address in FRU is          %pM\n"
					"Address in environment is  %pM\n",
					name, enetaddr, old_addr);
				printf("Please fix it manually and save\n");
				ret++;
			}
		} else {
			printf("%s: Address %pM is not valid for iface %d\n",
				__func__, enetaddr, i);
			ret++;
		}
	}

	return ret;
}

int
fru_mrec_update_bootdevice(struct fru *f, uint8_t *bootdevice) {
	int i;
	int len = strlen((char *)bootdevice);
	struct multirec *m;

	if (len > FRU_STR_MAX)
		len = FRU_STR_MAX;
	memset(f->bootdevice, 0, FRU_STR_MAX);
	memcpy(f->bootdevice, bootdevice, len);
	for (i = 0; i < f->mrec_count; i++) {
		if (f->mrec[i].type == MR_SATADEV_REC) {
			f->mrec[i].data = f->bootdevice;
			f->mrec[i].length = len;
			return 0;
		}
	}

	//no mrec for sata found, creating one
	m = &f->mrec[f->mrec_count];
	m->type = MR_SATADEV_REC;
	m->format = 2;
	m->length = len;
	m->data = f->bootdevice;
	f->mrec_count++;

	return -1;
}

int
fru_mrec_update_passwd_line(struct fru *f, uint8_t *passwd_line) {
	int i;
	int len = strlen((char *)passwd_line);
	struct multirec *m;

	if (len > FRU_PWD_MAX)
		len = FRU_PWD_MAX;
	memset(f->passwd_line, 0, FRU_PWD_MAX);
	memcpy(f->passwd_line, passwd_line, len);
	for (i = 0; i < f->mrec_count; i++) {
		if (f->mrec[i].type == MR_PASSWD_REC) {
			f->mrec[i].data = f->passwd_line;
			f->mrec[i].length = len;
			return 0;
		}
	}
	m = &f->mrec[f->mrec_count];
	m->type = MR_PASSWD_REC;
	m->format = 2;
	m->length = len;
	m->data = f->passwd_line;
	f->mrec_count++;

	return -1;
}

int
fru_mrec_update_test_ok(struct fru *f, uint8_t test_ok) {
	int i;
	struct multirec *m;

	f->test_ok = test_ok;
	for (i = 0; i < f->mrec_count; i++) {
		if (f->mrec[i].type == MR_TESTOK_REC) {
			f->mrec[i].data = &f->test_ok;
			f->mrec[i].length = 1;
			return 0;
		}
	}
	m = &f->mrec[f->mrec_count];
	m->type = MR_TESTOK_REC;
	m->format = 2;
	m->length = 1;
	m->data = &f->test_ok;
	f->mrec_count++;

	return -1;
}

int
fru_mrec_update_power_policy(struct fru *f, enum POWER_POLICY pp) {
	int i;
	struct multirec *m;

	f->power_policy = pp;
	for (i = 0; i < f->mrec_count; i++) {
		if (f->mrec[i].type == MR_POWER_POLICY_REC) {
			f->mrec[i].data = &f->power_policy;
			f->mrec[i].length = 1;
			return 0;
		}
	}
	m = &f->mrec[f->mrec_count];
	m->type = MR_POWER_POLICY_REC;
	m->format = 2;
	m->length = 1;
	m->data = &f->power_policy;
	f->mrec_count++;

	return -1;
}

int
fru_mrec_update_power_state(struct fru *f) {
	int i = 0;
	struct multirec *m;

	f->power_state = 1;
	for (; i<f->mrec_count; i++) {
		if (f->mrec[i].type == MR_POWER_STATE_REC) {
			f->mrec[i].data = &f->power_state;
			f->mrec[i].length = 1;
			return 0;
		}
	}
	m = &f->mrec[f->mrec_count];
	m->type = MR_POWER_STATE_REC;
	m->format = 2;
	m->length = 1;
	m->data = &f->power_state;
	f->mrec_count++;

	return -1;
}

int tp_check_fru()
{
	int rc;

	rc = fru_open_parse(&fru);

	if (rc) {
		printf("fru_open_parse() returned %d\n", rc);
		return 1;
	}

	printf("P/N:   %s\n", fru.part_number);
	printf("S/N:   %s\n", fru.serial_number);
	print_board_area(&fru);
	print_product_area(&fru);

	env_set("board_serial", (char *)fru.serial_number);
	env_set("board_rev", (char *)fru.part_number);
	env_set("board_name", (char *)fru.product_name);

	rc = fru_setall_macaddr();

	return rc;
}
