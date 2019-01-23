/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Boot controller registers.
 */

#include "mem_map.h"

#ifndef __BOOT_CNTR_H__
#define __BOOT_CNTR_H__

#define BC_CSR  (BOOT_CNTR_BASE + 0x0)
#define BC_MAR	(BOOT_CNTR_BASE + 0x4)
#define BC_DRID  (BOOT_CNTR_BASE + 0x8)
#define BC_VID	(BOOT_CNTR_BASE + 0xC)

#define BC_MAR_VAR	1
#define BC_CSR_RDA      (1 << 8)

// регистр контроля и статуса
typedef struct {
    uint32_t mode       :1-0  +1;  //способ загрузки
    uint32_t __         :7-2  +1;
    uint32_t spi_rda    :8-8  +1;  //0- прозрачный режим работы, 1- не прозрачный (см. 2.2 раздел)
    uint32_t _          :31-9 +1;
} boot_csr_t;

// регистр контроля доступа к памяти
typedef struct {
    uint32_t bsab       :0-0  +1;  // сбрасывается при записи в регистр
    uint32_t _          :31-1 +1;
} boot_mar_t;

// typedef struct {
//     uint32_t  _0    :8;  //low
//     uint32_t  _1    :8;
//     uint32_t  _2    :8;
//     uint32_t  _3    :8;  //hi
// } endian_t;

#endif /* __BOOT_CNTR_H__ */

