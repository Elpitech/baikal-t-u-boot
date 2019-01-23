#include "mem_map.h"
#include "sysreg.h"

.text
.set noreorder
.set nomacro

.global msafpe_handler
.ent msafpe_handler

msafpe_handler:
#ifdef SIMD_ENABLE
        cfcmsa  t0, MSACSR
        li      t2, ~(MSACSR_CAUSE_MASK << MSACSR_CAUSE_SHIFT)
        li      t3, ~(MSACSR_FLAGS_MASK << MSACSR_FLAGS_SHIFT)
        and     t0, t0, t2
        and     t0, t0, t3
        li      t1, MSACSR_NX
        or      t0, t0, t1
        ctcmsa  MSACSR, t0
#endif /*SIMD_ENABLE*/
        jr      ra
        nop
.end msafpe_handler

