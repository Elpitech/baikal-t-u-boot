/*
 * Configuration settings for the Baikal-T1 based ET412 boards.
 *
 * Copyright (C) 2022 Elpitech
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_ELPITECH_ET412_H
#define __CONFIG_ELPITECH_ET412_H

/*
 *-----------------------------------------------
 * Default Baikal-T1 config macro
 *-----------------------------------------------
 */
#include <configs/tplatforms_mitx.h>

/*
 *-----------------------------------------------
 * Generic Configurations
 *-----------------------------------------------
 */
#undef  BOARD_NAME
#define BOARD_NAME			"ET412"

#undef BAIKAL_USB_RESET_GPIO
#define BAIKAL_USB_RESET_GPIO		"porta10"

#endif /* __CONFIG_ELPITECH_ET412_H */
