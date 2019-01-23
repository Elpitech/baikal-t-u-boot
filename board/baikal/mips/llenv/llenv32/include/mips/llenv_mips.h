/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2015 Baikal Electronics.
 *
 * Author: Ekaterina Skachko <Ekaterina.Skachko@baikalelectronics.ru>
 *
 * Description: Main header file. MIPS-specific part
 */

/*
 * Define llenv_print for printing the testing results.
 */
#include <stdint.h>
#include "uart.h"

typedef uint32_t llenv_arch_t;

#ifdef YAMON_MALTA
/* MIPS, YAMON_MALTA */
#define llenv_print(x)	shell_puts(x, 0)
#else
/* MIPS, others */
#ifndef UBOOT
#define llenv_print(x)	do { 		\
OUTPUT_LOCK(&llenv32_output_lock);	\
UART_PRINTS(x);				\
OUTPUT_UNLOCK(&llenv32_output_lock);	\
} while (0)
#else
#define llenv_print(x)
#endif /* UBOOT */
#endif /* YAMON_MALTA */

/* Define macros for printing debug messages. */
#if (defined(DEBUG) && defined(UART_EXISTS))
#ifndef UBOOT
#define debug_msg llenv32_printf
#define error_msg llenv32_printf
#else /*UBOOT*/
#define debug_msg               printf
#define llenv32_printf          printf
#define error_msg               printf
extern int printf (const char *__restrict __format, ...);
#endif /*UBOOT*/
#else
#define debug_msg(x...)
#define error_msg(x...)
#endif

#define READ_MEMORY_REG(r)       (*((volatile uint32_t *) (r)))
#define WRITE_MEMORY_REG(r, v)   (*((volatile uint32_t *) (r)) = v)

/*
 * TEST_MEM_REG_ID_VAL macro returns 0 if the reg register masked by the
 * reg_mask is to equal to its reg_val default value.
 */
#ifdef DEBUG
#define TEST_MEM_REG_ID_VAL(reg, reg_val, reg_mask) ({              \
   register unsigned int _r = READ_MEMORY_REG(reg);             \
   if ((_r & reg_mask) == reg_val)  {                           \
      debug_msg("%32s: 0x%08x : PASS\n", #reg, _r);                          \
   } else {                                                     \
      error_msg("%32s: FAIL: expected(0x%08x), got(0x%08x)\n",  \
                #reg, reg_val, (_r & reg_mask));                \
   }                                                            \
   (!((_r & reg_mask) == reg_val));})
#else
#define TEST_MEM_REG_ID_VAL(reg, reg_val, reg_mask) ({              \
   register unsigned int _r = READ_MEMORY_REG(reg);             \
   (!((_r & reg_mask) == reg_val));})
#endif /* DEBUG */

/* 
 * TEST_MEM_REG_ID macro returns 0 if the reg register is to equal to
 * its reg_val default value.
 */
#ifdef DEBUG
#define TEST_MEM_REG_ID(reg, reg_val) ({                      \
   register unsigned int _r = READ_MEMORY_REG(reg);      \
   if (_r == reg_val)  {                                        \
      debug_msg("%32s: 0x%08x : PASS\n", #reg, _r);                          \
   } else {                                                     \
      error_msg("%32s: FAIL: expected(0x%08x), got(0x%08x)\n",  \
                #reg, reg_val, _r);                             \
   }                                                            \
   (!(_r == reg_val));})
#else
#define TEST_MEM_REG_ID(reg, reg_val) ({                      \
   register unsigned int _r = READ_MEMORY_REG(reg);      \
   (!(_r == reg_val));})
#endif /* DEBUG */


/* 
 * TEST_READ_MEM_REG macro returns 0 if the reg register can be read.
 */
#define TEST_READ_PATTERN	0x89ABCDEF
#define TEST_READ_MEM_REG(reg) ({                                  \
   register unsigned int _r = TEST_READ_PATTERN;                   \
   _r = READ_MEMORY_REG(reg);                               \
   debug_msg("%32s: 0x%08x\n", #reg, _r);                          \
   (_r == TEST_READ_PATTERN); })

/* Define the wait_for_interrupt instruction. */
/* Set WFI to use the wait instruction or a loop. */
#if 1 /* WFI */
#define WFI	asm volatile("wait")
#else
#define WFI_COUNTER_MAX 300000
#define	WFI	do {					\
   volatile int _wait_cnt = 0;				\
   while (_wait_cnt < WFI_COUNTER_MAX) _wait_cnt++;	\
} while (0)
#endif /* WFI */
