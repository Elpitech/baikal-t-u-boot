/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: List of all the test suites.
 */

#include "llenv.h"
#ifdef UBOOT
DEC_TSUITE(memtest);
#else
DEC_TSUITE(mem);
DEC_TSUITE(uart);
#ifdef ARCH_MIPS
/* TBD */
DEC_TSUITE(sysreg);
DEC_TSUITE(gic);
#endif
#ifdef ARCH_ARM
/* TBD */
//DEC_TSUITE(gtimer);
DEC_TSUITE(aarch32);
#endif /* ARCH_ARM */
DEC_TSUITE(wdt);
DEC_TSUITE(timer);
DEC_TSUITE(i2c);
DEC_TSUITE(rtc);
DEC_TSUITE(spi);
DEC_TSUITE(gpio);
#ifdef ARCH_MIPS
DEC_TSUITE(cp0);
DEC_TSUITE(cp1);
DEC_TSUITE(l1);
DEC_TSUITE(l2);
DEC_TSUITE(mmu);
DEC_TSUITE(gcr);
DEC_TSUITE(cpc);
DEC_TSUITE(dma_cfgreg);
DEC_TSUITE(dma_crypto);
DEC_TSUITE(except);
DEC_TSUITE(dhry);
DEC_TSUITE(dgemm);
DEC_TSUITE(stream);
DEC_TSUITE(crypto);
#endif /* UBOOT */
#endif /* ARCH_MIPS */

BEGIN_TSUITES_LIST
#ifdef UBOOT
ADD_TSUITE_TO_LIST(memtest)
#else
#ifndef YAMON_MALTA
ADD_TSUITE_TO_LIST(mem)
ADD_TSUITE_TO_LIST(uart)
#endif /* YAMON_MALTA */
#ifdef ARCH_MIPS
/* TBD */ 
ADD_TSUITE_TO_LIST(sysreg)
ADD_TSUITE_TO_LIST(gic)
#endif /* ARCH_MIPS */
#if defined (ARCH_MIPS) && !defined(YAMON_MALTA)
ADD_TSUITE_TO_LIST(cp0)
ADD_TSUITE_TO_LIST(l1)
ADD_TSUITE_TO_LIST(mmu)
ADD_TSUITE_TO_LIST(gcr)
ADD_TSUITE_TO_LIST(cpc)
ADD_TSUITE_TO_LIST(dma_crypto)
ADD_TSUITE_TO_LIST(dma_cfgreg)
ADD_TSUITE_TO_LIST(crypto)
#endif /* ARCH_MIPS */
#ifdef ARCH_ARM
/* TBD */
ADD_TSUITE_TO_LIST(aarch32)
//ADD_TSUITE_TO_LIST(gtimer)
#endif /* ARCH_ARM */
#ifndef YAMON_MALTA
ADD_TSUITE_TO_LIST(timer)
ADD_TSUITE_TO_LIST(i2c)
ADD_TSUITE_TO_LIST(rtc)
ADD_TSUITE_TO_LIST(spi)
ADD_TSUITE_TO_LIST(gpio)
ADD_TSUITE_TO_LIST(wdt)
#endif /* YAMON_MALTA */
#endif /* UBOOT */
END_TSUITE_LIST;
