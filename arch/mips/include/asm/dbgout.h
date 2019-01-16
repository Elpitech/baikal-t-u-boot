#ifndef DEBUG_OUT
#define DEBUG_OUT
#ifndef ASM
// Place here definitions for C-code

void cfgreg_out(void);
void ddr_ddrc_regs_print(void);

#else // ASM defined
// Place here definitions for asm-code

.macro	dbg_out type, val

	_savereg stack
	lui a0, (\type >> 16)
	ori a0, a0,(\type & 0x0000FFFF)
	add a1, zero, \val

	la t9, debug_out
	jalr t9
	nop
	_restorereg stack
.endm


.macro	dbg_out_val val

	_savereg stack
	lui a0, (0xDEAD)
	ori a0, (0x0010)
	lui a1, (\val >> 16)
	ori a1, (\val & 0xFFFF)

	la t9, debug_out
	jalr t9
	nop
	_restorereg stack
.endm

#endif // ASM
// Place here common definitions

#define TYPEMAGIC	0xDEAD0000
#define TYPEMASK	0xFFFF0000
#define TREG		0xDEADFFF0
#define CFG0		0xDEADCF00
#define CFG1		0xDEADCF10
#define CFG2		0xDEADCF20
#define CFG3		0xDEADCF30
#define CFG4		0xDEADCF40
#define CFG5		0xDEADCF50
#define CFG6		0xDEADCF60
#define VAL		0xDEAD0010
#endif // DEBUG_OUT
