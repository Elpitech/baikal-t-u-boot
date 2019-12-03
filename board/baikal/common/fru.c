// SPDX-License-Identifier: GPL-2.0+
/*
 *
 */

#include <common.h>
#include "fru.h"

//#define FRU_DEBUG
#ifdef FRU_DEBUG
#define fru_dbg(...) {printf (__VA_ARGS__); }
#else
#define fru_dbg(...)
#endif

#ifndef min
#define min(a,b) ((a) > (b)) ? (b) : (a)
#endif

static uint8_t fru_buf[FRU_SIZE];
static uint8_t fru_buf2[FRU_SIZE];

uint8_t
calc_cs(uint8_t *buf, int size) {
	uint8_t cs = 0;
	int i;

	for (i = 0; i < size; i++)
		cs += buf[i];

	return cs;
}

int
fru_mk_multirecord(uint8_t *buf, unsigned int buf_size, struct multirec *mr, bool last)
{
	struct fru_mrec *mrec = (struct fru_mrec *)buf;

	if (mr->length + 5 >= buf_size)
		return -1;

	mrec->type = mr->type;
	mrec->format_version = (last ? (1 << 7) : 0) | 0x02;
	mrec->len = mr->length;
	memcpy(mrec->data, mr->data, mr->length);
	mrec->rec_cs = 256 - calc_cs(buf + 5, mr->length);
	mrec->hdr_cs = 256 - calc_cs(buf, 4);

	return mr->length + 5;
}

static uint8_t
bcd_to_ascii(uint8_t bcd)
{
	if (bcd < 0xa)
		return '0' + bcd;
	else if (bcd == 0xa)
		return ' ';
	else if (bcd == 0xb)
		return '-';
	else if (bcd == 0xc)
		return '.';
	else
		return 0;
}

static int
bcd_to_str(uint8_t *str, uint8_t *buf, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		*str++ = bcd_to_ascii(buf[i] & 0xf);
		*str++ = bcd_to_ascii((buf[i] >> 4) & 0xf);
	}

	return len * 2;
}

static uint8_t
ascii6_to_ascii(uint8_t a6)
{
	return a6 + 0x20;
}

static int
ascii6_to_str(uint8_t *str, uint8_t *buf, int len)
{
	int i;

	for (i = 0; i < len; i += 3) {
		*str++ = ascii6_to_ascii(buf[i] & 0x3f);
		*str++ = ascii6_to_ascii((buf[i] >> 6) | ((buf[i + 1] & 0xf) << 2));
		*str++ = ascii6_to_ascii((buf[i + 1] >> 4) | ((buf[i + 2] & 3) << 4));
		*str++ = ascii6_to_ascii(buf[i + 2] >> 2);
	}

	return len / 3 * 4;
}

int
read_fru_str(uint8_t *buf, uint8_t *str, unsigned int *len, unsigned int offset)
{
	int str_len, ret_len;
	uint8_t type;

	str_len = buf[offset] & 0x3f;
	type = buf[offset] >> 6;
	ret_len = min(str_len, FRU_STR_MAX - 1);
	/* if str is NULL then we just skip to next record */
	if (str) {
		if (type == 0 || type == 3) { /* binary or ASCII/Unicode */
			memcpy(str, buf + offset + 1, ret_len);
			*len = ret_len;
		} else if (type == 1) { /* BCD-plus */
			*len = bcd_to_str(str, buf + offset + 1, ret_len);
		} else if (type == 2) { /* ASCII-6 */
			*len = ascii6_to_str(str, buf + offset + 1, ret_len);
		}
	}

	return offset + str_len + 1;
}

/*
 * Parse Board Info Area pointed by 'buf' into struct fru.
 * Returns size of the area or negative error.
 */
int
parse_board_area(struct fru *f, uint8_t *buf, unsigned int buf_len) {
	uint8_t cs;
	int offt, len;
  
	if (buf[0] != BOARD_AREA_VERSION) {
		printf("FRU: Board area version is not valid\n");
		return -1;
	}
	len = buf[1] * 8;
	if (len > buf_len) {
		printf("FRU: Board area size mismatch\n");
		return -2;
	}

	cs = calc_cs(buf, len);

	if (cs != 0) {
		printf("FRU: Bad board area checksum [0-%i]: %i\n",
			len, cs);
		return -3;
	}
	f->mfg_date[0] = buf[3];
	f->mfg_date[1] = buf[4];
	f->mfg_date[2] = buf[5];

#ifdef TP_FRU_BUG
	/*
	 * Older versions of FRU writer put mfg_name starting from byte 5.
	 * Try to detect if we deal with such one.
	 */
	if (buf[5] < 0x3f) {
		/*
		 * Timestamp 0x3fffff translates into Dec 22, 2003 - 
		 * we do not beleive it is correct. OTOH, we do not expect
		 * manufacturer string longer then 63.
		 */
		offt = 5;
	} else
#endif
	offt = 6;
	offt = read_fru_str(buf, f->mfg_name, &f->mfg_name_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->product_name, &f->product_name_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->serial_number, &f->serial_number_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->part_number, &f->part_number_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->fru_id, &f->fru_id_len, offt);
	if (offt > len)
		return -4;

	return len;
}

/*
 * Parse Product Info Area.
 */
int
parse_product_area(struct fru *f, uint8_t *buf, unsigned int buf_len) {
	int offt, len;

	if (buf[0] != PRODUCT_AREA_VERSION) {
		printf("FRU: Product area version is not valid\n");
		return -1;
	}
	len = buf[1] * 8;
	if (len > buf_len) {
		printf("FRU: Product area size mismatch\n");
		return -2;
	}

	if (calc_cs(buf, len) != 0) {
		printf("FRU: Bad product area checksum\n");
		return -3;
	}
	offt = 3;
	offt = read_fru_str(buf, f->p_product_mfg, &f->p_product_mfg_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->p_product_name, &f->p_product_name_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->p_part_model_number, &f->p_part_model_number_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->p_product_version, &f->p_product_version_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->p_serial_number, &f->p_serial_number_len, offt);
	if (offt > len)
		return -4;
	offt = read_fru_str(buf, f->p_fru_id, &f->p_fru_id_len, offt);
	if (offt > len)
		return -4;

	return len;
}

/*
 * Parse multirecord.
 * m->data pointer must be initialized by caller.
 * Return: record size or 0 if last, < 0 on error.
 */
int
fru_parse_multirecord(struct multirec *m, uint8_t *buf, unsigned int buf_len) {
	int len;
	uint8_t data_cs;

	if (buf_len < 5) {
		printf("FRU: no space in multirecord buffer, failed to parse header\n");
		return -4;
	}

	if (calc_cs(buf, 5) != 0) {
		printf("FRU: multirecord header checksum is invalid\n");
		return -1;
	}
	m->type = buf[0];
	m->format = buf[1] & 0x7;
	if (m->format != 0x2) {
		printf("FRU: multirecord format is unknown [%i]\n", m->format);
		return -2;
	}

	len = m->length = buf[2];
	if (buf_len < len + 5) {
		printf("FRU: no space in multirecord buffer, failed check data\n");
		return -5;
	}
  
	data_cs = calc_cs(&buf[5], len) + buf[3];
	if (data_cs != 0) {
		printf("FRU: multirecord data checksum is invalid [0x%02x]\n", data_cs);
		return -3;
	}
	memcpy(m->data, buf + 5, len);

	return (buf[1] & 0x80) ? 0 : len + 5;
}

int
parse_fru(struct fru *f, uint8_t *buf, unsigned int buf_len) {
	int ret = 0;
	int mrec_n = 0;
	int offt = 0;

	if (buf_len < 8) {
		printf("FRU buffer is too short\n");
		return -1;
	}
	if (buf[0] == 0xff) {
		printf("FRU: Empty EEPROM detected\n");
		return -2;
	} else if (buf[0] != FRU_VERSION) {
		printf("FRU: Header version is not valid\n");
		return -3;
	} else if (calc_cs(buf, 8) != 0) {
		printf("FRU: Bad header checksum: %i\n", calc_cs(buf, 8));
		return -4;
	}
	f->board_area_offset = buf[3] * 8;
	f->product_area_offset = buf[4] * 8;
	f->mrec_area_offset = buf[5] * 8;
	if (f->board_area_offset != 0 && f->board_area_offset < buf_len)
		ret = parse_board_area(f, &buf[f->board_area_offset],
					 buf_len - f->board_area_offset);
	if (ret < 0)
		return -5;
	else
		offt = f->board_area_offset + ret;

	if (f->product_area_offset >= offt && f->product_area_offset < buf_len) {
		ret = parse_product_area(f, &buf[f->product_area_offset],
					 buf_len - f->product_area_offset);
		if (ret < 0)
			return -6;
		else
			offt = f->product_area_offset + ret;
	}

	f->mrec_count = 0;
	if (f->mrec_area_offset == 0)
		return 0;
	if (f->mrec_area_offset < offt || f->mrec_area_offset >= buf_len)
		return -6;
	offt = f->mrec_area_offset;
	if (offt == 0)
		return 0;

	while (ret >= 0 && (mrec_n < N_MULTIREC)) {
		f->mrec[mrec_n].data = f->mr_buf + f->mr_buf_used;
		ret = fru_parse_multirecord(&f->mrec[mrec_n], &buf[offt], buf_len - offt);
		if (ret > 0) {
			f->mrec_count++;
			mrec_n++;
			offt += ret;
			f->mr_buf_used += ret;
		} else if (ret == 0) {
			f->mrec_count++;
			/* Account for buffer space used by last record. */
			f->mr_buf_used += f->mrec[mrec_n].length;
			break;
		} else {
			printf("FRU: Failed to parse multirecord\n");
			return -7;
		}
	}

	return 0;
}

int
fru_mk_multirecords_area(struct fru *f, uint8_t *buf, unsigned int buf_len) {
	int i;
	int ret;
	int offt = 0;

	for (i = 0; i < f->mrec_count; i++) {
		ret = fru_mk_multirecord(buf + offt, buf_len - offt,
					 &f->mrec[i],
					 (i < f->mrec_count - 1) ? false : true);
		if (ret < 0) {
			printf("FRU: Failed to pack multirecord\n");
			return -1;
		}
		offt += ret;
	}

	return offt;
}

int
fru_mrec_update_mac(struct fru *f, uint8_t *mac, int iface) {
	int i;
	int mrec_id;

	if (iface == 0)
		mrec_id = MR_MAC_REC;
	else if (iface == 1)
		mrec_id = MR_MAC2_REC;
	else if (iface == 2)
		mrec_id = MR_MAC3_REC;
	else	/* (iface >= N_MAC) */
		return -1;

	memcpy(f->mac_data + iface * 6, mac, 6);

	for (i = 0; i < f->mrec_count; i++) {
		if (f->mrec[i].type == mrec_id) {
			f->mrec[i].data = f->mac_data + iface * 6;
			return 0;
		}
	}

	/* Create new mrec. */
	f->mrec[i].type = mrec_id;
	f->mrec[i].format = 2;
	f->mrec[i].data = f->mac_data + iface * 6;
	f->mrec[i].length = 6;
	f->mrec_count++;

	return 0;
}

int
fru_update_mrec(struct fru *f) {
	int ret;

	if (f->mrec_area_offset == 0)
		return -1;	/* TODO: create new mrec area and update header */
	memcpy(fru_buf2, fru_buf, f->mrec_area_offset);
	ret = fru_mk_multirecords_area(f, fru_buf2 + f->mrec_area_offset,
			f->fru_size - f->mrec_area_offset);
	if (ret < 0)
		return -1;

	ret = write_fru(fru_buf2, 0, f->fru_size);

	if (ret < 0)
		return ret;

	return 0;
}

int
fru_open_parse(struct fru *f) {
	int i = 0;
	int fru_size;

	memset(f, 0, sizeof(struct fru));
	f->mac0 = f->mac_data;
	f->mac1 = f->mac_data + 6;
	f->mac2 = f->mac_data + 12;
	fru_size = read_fru(fru_buf);
	if (fru_size < 0)
		return fru_size;
	f->fru_size = fru_size;
	if (parse_fru(f, fru_buf, fru_size) != 0)
		return -2;

	for (i=0; i < f->mrec_count; i++) {
		if (f->mrec[i].type == MR_MAC_REC) {
			memcpy(f->mac_data, f->mrec[i].data, 6);
			fru_dbg("FRU: found MAC mrec [%02x %02x %02x %02x %02x %02x]\n",
				f->mac_data[0], f->mac_data[1], f->mac_data[2],
				f->mac_data[3], f->mac_data[4], f->mac_data[5]);
		} else if (f->mrec[i].type == MR_MAC2_REC) {
			memcpy(f->mac_data+6, f->mrec[i].data, 6);
			fru_dbg("FRU: found MAC2 mrec [%02x %02x %02x %02x %02x %02x]\n",
				f->mac_data[6], f->mac_data[7], f->mac_data[8],
				f->mac_data[9], f->mac_data[10], f->mac_data[11]);
		} else if (f->mrec[i].type == MR_MAC3_REC) {
			memcpy(f->mac_data+12, f->mrec[i].data, 6);
			fru_dbg("FRU: found MAC3 mrec [%02x %02x %02x %02x %02x %02x]\n",
				f->mac_data[12], f->mac_data[13], f->mac_data[14],
				f->mac_data[15], f->mac_data[16], f->mac_data[17]);
		} else if (f->mrec[i].type == MR_SATADEV_REC) {
			memcpy(f->bootdevice, f->mrec[i].data,
				(f->mrec[i].length > FRU_STR_MAX ?
					 FRU_STR_MAX : f->mrec[i].length));
			fru_dbg("FRU: found boot device [%s]\n", f->bootdevice);
		} else if (f->mrec[i].type == MR_PASSWD_REC) {
			memcpy(f->passwd_line, f->mrec[i].data,
				(f->mrec[i].length > FRU_PWD_MAX ?
					FRU_PWD_MAX : f->mrec[i].length));
			fru_dbg("FRU: found passwd line [%s]\n", f->passwd_line);
		} else if (f->mrec[i].type == MR_TESTOK_REC) {
			f->test_ok = f->mrec[i].data[0];
			fru_dbg("FRU: found test ok record [0x%02x]\n",
				f->test_ok);
		} else if (f->mrec[i].type == MR_POWER_POLICY_REC) {
			f->power_policy = f->mrec[i].data[0];
			fru_dbg("FRU: found power policy record [0x%02x]\n",
				f->power_policy);
		} else {
			printf("FRU: unexpected multirecord %x (length %d)\n",
				f->mrec[i].type, f->mrec[i].length);
		}
	}

	return 0;
}
