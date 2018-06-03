/*
 * File:        detect.c
 * Description: Detect T-platforms CNC board type
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <pcf857x.h>

#include "devid.h"
#include "detect.h"

struct cnc_dev_config {
	u16 did;
	u8 rev;
	const char *name;
};

static struct cnc_dev_config cnc_cfgs[] = {
#if defined(CONFIG_TPLATFORMS_CNC_SFBT1)
	{CNC_DID_CUDEV, CNC_REV_ANY, "baikal_tplatforms_cnccu-sfbt1.dtb"},
	{CNC_DID_CU, 2, "baikal_tplatforms_cnccu-sfbt1.dtb"},
	{CNC_DID_CU, 3, "baikal_tplatforms_cnccu3-sfbt1.dtb"},
	{CNC_DID_CU, CNC_REV_ANY, "baikal_tplatforms_cnccu3-sfbt1.dtb"},
	{CNC_DID_OUT, CNC_REV_ANY, "baikal_tplatforms_cncio-sfbt1.dtb"},
	{CNC_DID_IN, CNC_REV_ANY, "baikal_tplatforms_cncio-sfbt1.dtb"},
	{CNC_DID_PROBE, CNC_REV_ANY, "baikal_tplatforms_cncio-sfbt1.dtb"},
	{CNC_DID_DAC, CNC_REV_ANY, "baikal_tplatforms_cncdac-sfbt1.dtb"},
	{CNC_DID_TTL, CNC_REV_ANY, "baikal_tplatforms_cncttl-sfbt1.dtb"},
	{CNC_DID_ENDAT22, CNC_REV_ANY, "baikal_tplatforms_cncendat22-sfbt1.dtb"},
	{CNC_DID_E1VPP, CNC_REV_ANY, "baikal_tplatforms_cnce1vpp-sfbt1.dtb"}
#elif defined(CONFIG_TPLATFORMS_CNC_MSBT2)
	{CNC_DID_CUDEV, CNC_REV_ANY, "baikal_tplatforms_cnccu-msbt2.dtb"},
	{CNC_DID_CU, 2, "baikal_tplatforms_cnccu-msbt2.dtb"},
	{CNC_DID_CU, 3, "baikal_tplatforms_cnccu3-msbt2.dtb"},
	{CNC_DID_CU, CNC_REV_ANY, "baikal_tplatforms_cnccu3-msbt2.dtb"},
	{CNC_DID_OUT, CNC_REV_ANY, "baikal_tplatforms_cncio-msbt2.dtb"},
	{CNC_DID_IN, CNC_REV_ANY, "baikal_tplatforms_cncio-msbt2.dtb"},
	{CNC_DID_PROBE, CNC_REV_ANY, "baikal_tplatforms_cncio-msbt2.dtb"},
	{CNC_DID_DAC, CNC_REV_ANY, "baikal_tplatforms_cncdac-msbt2.dtb"},
	{CNC_DID_TTL, CNC_REV_ANY, "baikal_tplatforms_cncttl-msbt2.dtb"},
	{CNC_DID_ENDAT22, CNC_REV_ANY, "baikal_tplatforms_cncendat22-msbt2.dtb"},
	{CNC_DID_E1VPP, CNC_REV_ANY, "baikal_tplatforms_cnce1vpp-msbt2.dtb"}
#endif
};

static const char *cnc_get_fit_config(u16 did, u8 rev)
{
	struct cnc_dev_config *cfg;
	int idx;

	for (idx = 0; idx < ARRAY_SIZE(cnc_cfgs); idx++) {
		cfg = &cnc_cfgs[idx];

		if (cfg->did == did && (cfg->rev == rev || cfg->rev == CNC_REV_ANY)) {
			return cfg->name;
		}
	}

	return NULL;
}

int cnc_detect_board(void)
{
	const char *config;
	char buf[64];
	int data;
	u16 did;
	u8 rev;

	data = pcf857x_get_val(CNC_SHRED_INFO);
	if (data < 0) {
		printf("CNC:   Can't read shred %d-%x\n",
			CNC_SHRED_I2C_BUS, CNC_SHRED_INFO);
		return -1;
	}
	did = CNC_SHRED_GET_DID(data);
	rev = CNC_SHRED_GET_REV(data);

	config = cnc_get_fit_config(did, rev);
	if (!config) {
		printf("CNC:   Can't find config for CNC %s v%hu.%hu\n",
			cnc_dev_name(did), did, rev);
		return -1;
	}

	snprintf(buf, 64, "#conf@%s", config);
	setenv("multi_conf", buf);
	printf("CNC:   %s v%hu.%hhu detected\n", cnc_dev_name(did), did, rev);

	return 0;
}
