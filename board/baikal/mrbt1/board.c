// SPDX-License-Identifier: GPL-2.0
/*
 * File:	board.c
 * Description:	T-platforms MRBT1 Baikal-T1 based board initialization
 * Authors:
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	/* TBD */
	printf("mrbt1: board_late_init\n");
	return 0;
}
#endif
