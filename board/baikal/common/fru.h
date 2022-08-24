// SPDX-License-Identifier: GPL-2.0+

#ifndef __FRU_H__
#define __FRU_H__

#include <stdbool.h>

#define MR_MAC_REC          0xC0
#define MR_SATADEV_REC      0xC1
#define MR_PASSWD_REC       0xC2
#define MR_TESTOK_REC       0xC3
#define MR_POWER_POLICY_REC 0xC4
#define MR_POWER_STATE_REC  0xC5
#define MR_MAC2_REC         0xC6
#define MR_MAC3_REC         0xC7

#define FRU_VERSION 1
#define BOARD_AREA_VERSION 1
#define PRODUCT_AREA_VERSION 1
#define FRU_SIZE 4096

#define FRU_STR_MAX		32
#define FRU_PWD_MAX		128
#define N_MULTIREC		16	/* Currently, only 8 used */
#define FRU_MR_DATA_MAX		1024	/* buffer for multirecords data */

#define N_MAC 3

#define FRU_STR(name, len) unsigned int name##_len; uint8_t name[len]

#define TP_FRU_BUG

//IANA Private Enterprise Number
#define ELPITECH_PEN 58584
#define OEM_ID ELPITECH_PEN

enum POWER_POLICY {
	PP_OFF=0,
	PP_KEEP,
	PP_ON,
	PP_NUM
};

struct multirec {
	uint8_t type;
	uint8_t format;
	unsigned int length;
	unsigned int oem_id;
	uint8_t *data;
};

struct fru {
	uint8_t mfg_date[3];
	unsigned int board_area_offset;
	unsigned int product_area_offset;
	unsigned int mrec_area_offset;

	FRU_STR(mfg_name, FRU_STR_MAX);
	FRU_STR(product_name, FRU_STR_MAX);
	FRU_STR(serial_number, FRU_STR_MAX);
	FRU_STR(part_number, FRU_STR_MAX);
	FRU_STR(fru_id, FRU_STR_MAX);
  
	FRU_STR(p_product_mfg, FRU_STR_MAX);
	FRU_STR(p_product_name, FRU_STR_MAX);
	FRU_STR(p_part_model_number, FRU_STR_MAX);
	FRU_STR(p_product_version, FRU_STR_MAX);
	FRU_STR(p_serial_number, FRU_STR_MAX);
	FRU_STR(p_fru_id, FRU_STR_MAX);

	struct multirec mrec[N_MULTIREC];
	unsigned int mrec_count;
	uint8_t mr_buf[FRU_MR_DATA_MAX];
	int mr_buf_used;

	int fru_size;

	/* T-Platforms OEM data */
	uint8_t mac_data[6*N_MAC];
	uint8_t *mac0;
	uint8_t *mac1;
	uint8_t *mac2;
	uint8_t bootdevice[FRU_STR_MAX];
	uint8_t passwd_line[FRU_PWD_MAX];
	uint8_t power_policy;
	uint8_t power_state;
	uint8_t test_ok;
};

struct fru_header {
	uint8_t version;
	uint8_t internal_area_offset;
	uint8_t chassis_info_offset;
	uint8_t board_area_offset;
	uint8_t product_info_offset;
	uint8_t multirec_offset;
	uint8_t _pad;
	uint8_t cs;
};

struct fru_mrec {
	uint8_t type;
	uint8_t format_version;
	uint8_t len;
	uint8_t rec_cs;
	uint8_t hdr_cs;
	uint8_t data[0];
};

/* registered Manufacturer ID for T-Platforms */
#define TPLATFORMS_ID	0x9bcc

int fru_open_parse(struct fru *f);
int fru_update_mrec(struct fru *f);
int fru_mrec_update_mac(struct fru *f, uint8_t *mac, int iface);
int fru_mrec_update_bootdevice(struct fru *f, uint8_t *bootdevice);
int fru_mrec_update_passwd_line(struct fru *f, uint8_t *passwd_line);
int fru_mrec_update_test_ok(struct fru *f, uint8_t test_ok);
int fru_mrec_update_power_policy(struct fru *f, enum POWER_POLICY pp);
int fru_mrec_update_power_state(struct fru *f);
void print_board_area(struct fru *f);
void print_product_area(struct fru *f);

int read_fru(uint8_t *buf);
int write_fru(uint8_t *buf, int offset, int size);
int tp_check_fru(void);
#endif/*__FRU_H__*/
