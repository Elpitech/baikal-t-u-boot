#include <asm/arch/sysreg.h>
#include <asm/arch/regmacro.S>
#define ASM
#include <asm/dbgout.h>

.global cfgreg_out
.text

cfgreg_out:

        savereg

        mfc0 t0, MIPS_CP0_CONFIG0
        dbg_out CFG0, t0

        mfc0 t0, MIPS_CP0_CONFIG1
        dbg_out CFG1, t0

        mfc0 t0, MIPS_CP0_CONFIG2
        dbg_out CFG2, t0

        mfc0 t0, MIPS_CP0_CONFIG3
        dbg_out CFG3, t0

        mfc0 t0, MIPS_CP0_CONFIG4
        dbg_out CFG4, t0

        mfc0 t0, MIPS_CP0_CONFIG5
        dbg_out CFG5, t0

        mfc0 t0, MIPS_CP0_CONFIG6
        dbg_out CFG6, t0

        restorereg
        jr ra
        and v0, zero, v0
