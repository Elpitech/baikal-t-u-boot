#include <asm/arch/sysreg.h>
#include <asm/arch/regmacro.S>

#define ASM		// This definition is strictly needed before 
#include <preparemmu.h>	// including special headers to avoid compilation error
#include <asm/dbgout.h>

#ifdef DEBUG

#define DBG(...) __VA_ARGS__

#else // DEBUG not defined

#define DBG(...)

#endif

.global prepare_mmu, stack

.data

stack: .word 0:256 

.text

prepare_mmu:

//dbg_out_val 0x1
	
	// Eval the beginning of virtual address
	and t0, t0, zero		// Virtual address is 0x0

	// Eval the beginning of physical address
	lui t1, 0x2000			// Phys addr is 0x20000000 = 512MB

	// Eval the last TLB index
	addi t2, zero, 0x3		// TLBEntry index

	// Eval page size
	lui t3, 0x1000			// Set page size 256 MB

	// Eval page mask
	// 2PageMask = 0x1FFFFFFF is for 256MB pages
	lui t4, 0x1FFF			// Load upper 16bit of 2PageMask
	ori t4, 0xFFFF			// Load lower 16bit of 2PageMask

1:
	bltz t2, 2f			// Exit from cycle if index < 0
	nop

//dbg_out_val 0x3
//dbg_out TREG, t0
//dbg_out TREG, t1
//dbg_out TREG, t2
//dbg_out TREG, t3
//dbg_out TREG, t4


// 1	// Set currently being modofied TLBIndex

//dbg_out TLBINDEX, t2
	mtc0 t2, MIPS_CP0_TLB_INDEX
	ehb
	sync
	mfc0 t6, MIPS_CP0_TLB_INDEX
dbg_out TLBINDEX, t6

// 2	// Set PageMask
//dbg_out PGMASK, t4
	mtc0 t4, MIPS_CP0_TLB_PG_MASK
	ehb
	sync
	mfc0 t6, MIPS_CP0_TLB_PG_MASK
dbg_out PGMASK, t6

	// Eval TLBEntry0 value
	lui t5, 0xF000
//dbg_out TREG, t1
//dbg_out TREG, t5
	and t5, t1, t5			// PFN0 = PhysAddr & 0xF000 0000
//dbg_out TREG, t5
	srl t5, 22			// Load PFN[29:6] =  PFN0 >> (PageSizeBits(28) - 6)
	ori t5, 0x17			// Add C[5:3]=2, D[2]=0, V[1]=1, G[0]=1 bits

// 3	// Set Entry0
//dbg_out TLBENTRY0, t5
	mtc0 t5, MIPS_CP0_TLB_LO0
	ehb
	sync
	mfc0 t6, MIPS_CP0_TLB_LO0
dbg_out TLBENTRY0, t6

	// Increment phys addr by 1 page (256MB)
	addu t1, t3

	// Eval TLBEntry1 value
	lui t5, 0xF000
//dbg_out TREG, t1
//dbg_out TREG, t5
	and t5, t1, t5			// PFN1 = PhysAddr & 0xF000 0000
//dbg_out TREG, t5
	srl t5, 22			// Load PFN[29:6] =  PFN1 >> (PageSizeBits(28) - 6)
	ori t5, 0x17			// Add C[5:3]=2, D[2]=1, V[1]=1, G[0]=1 bits

// 4	// Set TLBEntry1 
//dbg_out TLBENTRY1, t5
	mtc0 t5, MIPS_CP0_TLB_LO1
	ehb
	sync
	mfc0 t6, MIPS_CP0_TLB_LO1
dbg_out TLBENTRY1, t6

	// Eval VPN2
	srl t6, t0, 16

// 5	// Set EntryHi
//dbg_out TLBENTRYHI, t6
	mtc0 t6, MIPS_CP0_TLB_ENTRY_HI
	ehb
	sync
	mfc0 t6, MIPS_CP0_TLB_ENTRY_HI
dbg_out TLBENTRYHI, t6

	// Increment phys addr by 1 page (256MB)
	addu t1, t3

	// Increeent virt addr by 2 pages (512MB)
	addu t0, t3
	addu t0, t3
	
	// Eval next TLBIndex number
	subu t2, t2, 0x1		// Decrement 
//dbg_out_val 0x4

	tlbwi

	// Go to next iteration
	b 1b
	nop
2:
//dbg_out_val 0x5
	jr	ra
	addi	v0, zero, 0xA5
