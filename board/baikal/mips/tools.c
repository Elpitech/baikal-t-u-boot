/*
 * File:        tools.c
 * Author:      Ramil Zaripov <Ramil.Zaripov@baikalelectronics.com>
 * Copyright (C) 2014 Baikal Electronics.
 * Description: contains a number of service functions necessary to work with spi
 */



#include <linux/types.h>
#include <configs/baikal_mips.h>
#include "boot_cntr.h"


/**
 * @brief      switching the operating mode of the load control
 * @param[in]  mode=0 transparent mode of spi memory, mode=1 not transparent
 */
void llenv32_boot_ctrl (int mode)
{
    boot_csr_t *csr = (void*) BC_CSR;
    csr->spi_rda = mode;
}
