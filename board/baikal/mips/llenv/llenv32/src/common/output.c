/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Output functions.
 */

#include <string.h>
#include <stdarg.h>
#include "llenv.h"
#include "cmdline.h"
#include "mem_map.h"

#ifdef ARCH_ARM
/* ARM */
#ifdef USE_SEMIHOSTING
#define llenv32_putc(x) semi_putc(x)
#define llenv32_puts(x) semi_puts(x)
#else /*USE_SEMIHOSTING */
#define llenv32_putc(x) UART_PRINTC(x)
#define llenv32_puts(x) UART_PRINTS(x)
#endif /*USE_SEMIHOSTING */
#elif defined(ARCH_MIPS)
#ifdef YAMON_MALTA
/* MIPS, YAMON_MALTA */
#define llenv32_putc(x)	shell_putc(x, 0)
#define llenv32_puts(x) shell_puts(x, 0)
#else
/* MIPS, others */
#define llenv32_putc(x)	UART_PRINTC(x)
#define llenv32_puts(x)	UART_PRINTS(x)
#endif /* YAMON_MALTA */
#endif /* ARCH_ARM */

#define is_digit(x)	(((x) >= '0') & ((x) <= '9'))
#define MAX_LEN_DIGIT	12

#ifndef RTL
llenv_spinlock_t llenv32_output_lock;
#endif

void llenv32_output_init(void)
{
	OUTPUT_INIT(&llenv32_output_lock);
}

static void llenv32_padding(char *s, int lpad, char pad)
{
	lpad -= llenv_strlen(s);
	if (lpad > 0) {
		while (lpad) {
			llenv32_putc(pad);
			lpad--;
		}
	}
}

/* Simple output function.  */
int llenv32_printf(const char *fmt, ...)
{
	va_list args;
	register llenv_arch_t  lpad, base, caps, arg, sign;
	char buf[MAX_LEN_DIGIT];
	char *cbuf;

	va_start(args, fmt);
	OUTPUT_LOCK(&llenv32_output_lock);

	#ifdef RUN_ON_ALL_CORES
	#ifdef ARCH_ARM
	/* This kinf of core numeration is in gem5 */
	int num_cpu = ((CURRENT_CPU & 0xF00) >> 7) | (CURRENT_CPU & 0x3);
	llenv32_puts("CORE");
	llenv32_putc(num_cpu + 48);
	llenv32_puts(":");
	#else
	llenv32_puts(CURRENT_CPU ? "CORE1:" : "CORE0:");
	#endif /*ARCH_ARM*/
	#endif /* RUN_ON_ALL_CORES */

	while (*fmt) {
		if (*fmt == '%') {
			base = lpad = caps = sign = 0;
			fmt++;

			/* Calculate the padding lenght. */
			for (; is_digit(*fmt); fmt++) {
				lpad = lpad * 10 + *fmt - '0';
			}

			arg = va_arg(args, llenv_arch_t);

			switch (*fmt) {
				case 'd':
					if (arg < 0) {
						sign = 1;
						arg = -arg;
					}
					/* Follow through. */
				case 'u':
					base = 10;
					break;
				case 'X':
					caps = 1;
					/* Follow through. */
				case 'x':
					base = 16;
					break;
				case 'c':
					llenv32_putc(arg);
					fmt++;
					continue;
				case 's':
					llenv32_padding((char *)arg, lpad, ' ');
					llenv32_puts((char *)arg);
					fmt++;
					continue;
				default :
					llenv32_puts(" UNSUP :");
					llenv32_putc(*fmt);
					fmt++;
					continue;
			}

			/* Convert integer to string. */
			cbuf = llenv32_itoa(buf, arg, base, MAX_LEN_DIGIT, caps);
			if (sign) {
				*--cbuf = '-';
			}
			llenv32_padding((char *)cbuf, lpad, '0');
			llenv32_puts(cbuf);
		} else {
			llenv32_putc(*fmt);
		}

		fmt++;
	}
	OUTPUT_UNLOCK(&llenv32_output_lock);
	va_end(args);
}
