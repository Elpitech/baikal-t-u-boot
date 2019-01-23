/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014-2016 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Main header file.
 */

#ifndef __LLENV_H__
#define __LLENV_H__


/* Include architecture - specific features */
#ifdef ARCH_MIPS
   #include "mips/llenv_mips.h"
#else /* MIPS, others */
   #include "arm/llenv_arm.h"
#endif

#include "mem_map.h"

#define OK			0
#define ERROR			-1
#define ERROR_UNIMPLEMENTED	100
#define ERROR_BIT0		0x0001
#define ERROR_BIT1		0x0002
#define ERROR_BIT2		0x0004
#define ERROR_BIT3		0x0008
#define ERROR_MISMATCH		ERROR_BIT0	/* 1 */
#define ERROR_MISMATCH0		ERROR_MISMATCH
#define ERROR_NO_IRQ		ERROR_BIT1	/* 2 */
#define ERROR_BAD_STATUS	ERROR_BIT2	/* 4 */
#define ERROR_DDR_CKSUM_SPD	ERROR_BIT2	/* 4 */
#define ERROR_DDR_INIT		ERROR_BIT3	/* 8 */
#define ERROR_MISMATCH1		0x0010
#define ERROR_MISMATCH2		0x0020
#define ERROR_MISMATCH3		0x0040
#define ERROR_MISMATCH4		0x0080
#define ERROR_MISMATCH5		0x0100
#define ERROR_MISMATCH6		0x0200
#define ERROR_MISMATCH7		0x0400
#define ERROR_MISMATCH8		0x0800
#define ERROR_MISMATCH9		0x1000
#define ERROR_MISMATCH10	0x2000
#define ERROR_MISMATCH11	0x4000
#define ERROR_MISMATCH12	0x8000

#define ERROR_SHIFT_16(x)	(x << 16)

/*
 * The INIT2ZERO macro is used to zero static variables for
 * debugging RTL only.
 */
#ifdef RTL
#define INIT2ZERO(x)	(x = 0)
#else /* RTL */
#define INIT2ZERO(x)
#endif /* RTL */

typedef struct
{
	int (*ts_init) (int tc_num);
	int  (*ts_run) (char *tc_name);
	void (*ts_info) (void);
	int (*ts_teardown) (int tc_num);
	char *testlink_ts_name;
} llenv_tsuite_t; /* type of test suite. */

typedef struct
{
	int (*tc_run) (void);
	char *testlink_tc_name;
} llenv_tcase_t;

typedef struct
{
	volatile int lock;
} llenv_spinlock_t;

#define INIT_SPIN_UNLOCKED(x)	((x)->lock = 0)
#define INIT_SPIN_LOCKED(x)	((x)->lock = 1)

#ifdef RTL
#define llenv32_output_lock (*((volatile llenv_spinlock_t *)(SRAM_BASE + 64)))
#else
extern llenv_spinlock_t llenv32_output_lock;
#endif

#ifdef RUN_ON_ALL_CORES
#define OUTPUT_INIT(x)		INIT_SPIN_UNLOCKED(x)
#define OUTPUT_LOCK(x)		brom_spin_lock((llenv_arch_t)(x))
#define OUTPUT_UNLOCK(x)	brom_spin_release((llenv_arch_t)(x))
#else
#define OUTPUT_INIT(x)
#define OUTPUT_LOCK(x)
#define OUTPUT_UNLOCK(x)
#endif /* RUN_ON_ALL_CORES */

/* Define vector table macros. */
#define VTABLE_MAGIC		0xCAFE
#define VTABLE_DEFAULT_ADDR	0x0

#define TCASE_RUN_HNDL(x, y)	llenv_##x##_##y##_run
#define INIT_HNDL(x)		llenv_##x##_init
#define RUN_HNDL(x)		llenv_##x##_run
#define INFO_HNDL(x)		llenv_##x##_info
#define TEARDOWN_HNDL(x)	llenv_##x##_teardown

#define TSUITE_STRUCT(x)	llenv_##x##_tsuite
#define TCASE_STRUCT(x, y)	llenv_##x##_##y##_tcase
#define TSUITE_LIST(x)		llenv_##x##_list

#define	DEC_TESTSUITE_INIT(ts)	int INIT_HNDL(ts) (int tc_num)
#define DEC_TESTSUITE_RUN(ts)	int RUN_HNDL(ts) (char *tc_name)
#define DEC_TESTSUITE_INFO(ts)	void INFO_HNDL(ts) (void)
#define DEC_TESTSUITE_TEARDOWN(ts)	int TEARDOWN_HNDL(ts) (int tc_num)

#define DEC_TESTCASE_RUN(ts, tc)	int TCASE_RUN_HNDL(ts, tc) (void)

#define DEC_TESTSUITE(ts, name) \
extern DEC_TESTSUITE_INIT(ts); \
extern DEC_TESTSUITE_RUN(ts); \
extern DEC_TESTSUITE_INFO(ts); \
extern DEC_TESTSUITE_TEARDOWN(ts); \
llenv_tsuite_t TSUITE_STRUCT(ts) = { &INIT_HNDL(ts), &RUN_HNDL(ts), \
				&INFO_HNDL(ts), &TEARDOWN_HNDL(ts), name }

#define DEC_TSUITE(x) \
extern llenv_tsuite_t TSUITE_STRUCT(x)

#define DEC_TESTCASE(ts, tc, name)	\
extern DEC_TESTCASE_RUN(ts, tc);		\
llenv_tcase_t TCASE_STRUCT(ts, tc) = { TCASE_RUN_HNDL(ts, tc), name }

#define DEC_TCASE(ts, tc) \
extern llenv_tcase_t TCASE_STRUCT(ts, tc)

#define BEGIN_TSUITES_LIST	\
llenv_tsuite_t *llenv_tsuites_list [] = {

#define ADD_TSUITE_TO_LIST(ts)  \
	(&TSUITE_STRUCT(ts)),

#define END_TSUITE_LIST	\
	(0)		\
}



#define BEGIN_TCASE_LIST(ts)	\
llenv_tcase_t * TSUITE_LIST(ts) [] = {

#define ADD_TCASE_TO_LIST(ts, tc)  \
	(&TCASE_STRUCT(ts, tc)),

#define END_TCASE_LIST	\
	(0)		\
}

/* Don't declare test cases for brom. */
#ifdef DONT_DECLARE_TC
#undef ADD_TSUITE_TO_LIST
#define ADD_TSUITE_TO_LIST(ts)
#undef ADD_TCASE_TO_LIST
#define ADD_TCASE_TO_LIST(ts, tc)
#undef TSUITE_LIST
#define TSUITE_LIST(x)		llenv_tc_once_list
#undef BEGIN_TCASE_LIST
#define BEGIN_TCASE_LIST(ts)	extern llenv_tcase_t * TSUITE_LIST(ts) [];
#undef END_TCASE_LIST
#define END_TCASE_LIST
#endif /* DONT_DECLARE_TC */

/*
 * Define the UART print macros. Use uart0 as the console port.
 */
#define UART_PORT_CONSOLE	0

#ifdef UART_EXISTS
#define UART_PRINTS(x) do {				\
/* Print if there is no loopback mode. */		\
if (!llenv_uart_loopback_mode(UART_PORT_CONSOLE))	\
	llenv_uart_puts(UART_PORT_CONSOLE, x);		\
} while (0)

#define UART_PRINTC(x) do {				\
/* Print if there is no loopback mode. */		\
if (!llenv_uart_loopback_mode(UART_PORT_CONSOLE))	\
	llenv_uart_putc(UART_PORT_CONSOLE, x);		\
} while (0)
#else /* UART_EXISTS */
#define UART_PRINTS(x)
#define UART_PRINTC(x)
#endif /* UART_EXISTS */

/* New line symbols. */
#define NL	"\n\r"

extern int llenv32_printf (const char *__restrict __format, ...);

int llenv_default_run_testsuite(llenv_tsuite_t *ts, llenv_tcase_t **tc, char *tc_name);
int llenv_run_all(void);
int llenv_run_ts(char *ts_name, char *tc_name);
int llenv_run_ts_index(int index);
void llenv_init_irq(void);
void llenv_disable_irq(void);

/* Define the test delay. */
#ifdef RTL
#define LOOP_DELAY	400
#else
#define LOOP_DELAY	100000
#endif /* RTL */

/* Define the quantity of the test iterations. */
#define MAX_TIMES	3

/* Convert string to integer. */
#define LLENV32_ATOI_BASE10		10
#define LLENV32_ATOI_BASE16		16
#define LLENV32_ATOI_TLEN_MAX		16
int llenv32_atoi(char **buf, int base, int tlen);
char *llenv32_itoa(char *buf, unsigned int dig, int base, int tlen, int caps);
void  *llenv32_memset(void *addr, int len, int c);
void  *llenv32_memcpy(void *dst, void * src, unsigned int bytes);

int llenv32_test_read(char *start, int size);
int llenv32_test_rwr(char *start, int size);
char *llenv_strcpy(char *s1, const char *s2);
int llenv_strlen(const char *str);
void llenv32_mem_map_info(void);
int llenv_strcmp(const char *cs, const char *ct);
void llenv32_install_hdl(void *addr_dst, void *addr_src, int size);
void llenv_general_exc_handler(unsigned int iar, unsigned int excode);
int llenv_get_cpu_freq(void);
void llenv_set_cpu_freq(uint32_t freq);

/* GIC configuration functions. */
void llenv32_set_gic_prop_irq(int sh_irq_num, int cpu_irq_num, int trig, int pol);
void llenv32_clear_gic_prop_irq(int irq_num);

/* Spinlock functions. */
void brom_spin_release(llenv_arch_t lock);
int brom_spin_lock(llenv_arch_t lock);
void llenv32_spin_release(unsigned int lock);
int llenv32_spin_lock(unsigned int lock);

/* SPI flash functions. */
void llenv32_spi_master_init(int spi_port, int spi_mode);
int llenv32_spi_jdec(int spi_port, int spi_line);
int llenv32_spi_read_start(int spi_port, unsigned int addr);
int llenv32_spi_read_finish(int spi_port);
int llenv32_spi_read_cs(int spi_port, int spi_line, uint32_t addr, uint8_t *out, int out_len);

/* Configure UARTs. */
int llenv_config_uart_by_string(int port, char *cfg);
char llenv_uart_trygetc(int port);

#endif /* __LLENV_H__ */

