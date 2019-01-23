/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Simple command line.
 */

#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#include "sysreg.h"
#include "uart.h"

/* Control characters */
#define CH_BASE		0x40
#define CTRL_A		('A'- CH_BASE)
#define CTRL_B		('B'- CH_BASE)
#define CTRL_C		('C'- CH_BASE)
#define CTRL_D		('D'- CH_BASE)
#define CTRL_E		('E'- CH_BASE)
#define CTRL_F		('F'- CH_BASE)
#define CTRL_H		('H'- CH_BASE)
#define CTRL_K		('K'- CH_BASE)
#define CTRL_N		('N'- CH_BASE)
#define CTRL_P		('P'- CH_BASE)
#define CTRL_U		('U'- CH_BASE)
#define BS		0x08
#define DEL		0x7F
#define TAB		0x09
#define CR		0x0D
#define LF		0x0A
#define ESC		0x1B
#define SP		0x20	/* Space */
#define CSI		0x9B
#define CMD_END		SP - 1


/* Define the handlers to service the command line. */
typedef void (*llenv_cmdrun_handler_t) (char *cmd, char *args);
typedef void (*llenv_cmdhelp_handler_t) (void);

#define CMDLINE_SIZE	32
#define LLENV32_BAD_CMD	0xBADCAFE

#define CONSOLE_PUTS(x)		llenv_uart_puts(UART_PORT_CONSOLE, x)
#define CONSOLE_PUTC(x)		llenv_uart_putc(UART_PORT_CONSOLE, x)
#define CONSOLE_GETC		llenv_uart_getc(UART_PORT_CONSOLE)
#ifdef FCT
#define SHOW_PROMPT		CONSOLE_PUTS("\nBAIKAL # ");
#elif defined(BROM)
#define SHOW_PROMPT		CONSOLE_PUTS("\nbr0m"); CONSOLE_PUTC(CURRENT_CPU ? '1' : '0'); CONSOLE_PUTS(" > ")
#else
#define SHOW_PROMPT		CONSOLE_PUTS("\nllenv32 > ")
#endif /* BROM */

char llenv32_toupper (char ch);
void llenv32_output_init(void);
void start_shell(llenv_cmdrun_handler_t run_hdl, llenv_cmdhelp_handler_t help_hdl);

#endif /* __CMDLINE_H__ */

