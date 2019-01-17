#include <common.h>
#include <asm/dbgout.h>

void debug_out(unsigned int type, unsigned int val) {
//	printf("DBG: arr address 0x%08X\n", arr);
//	printf("DBG: %s()\n", __func__);
//	printf("DBG: type: 0x%08X, TYPEMASK: 0x%08X, TYPEMAGIC: 0x%08X\n", type, TYPEMASK, TYPEMAGIC);
//	printf("DBG: type & TYPEMASK: 0x%08X\n", type & TYPEMASK);
//	printf("DBG: type & TYPEMASK != TYPEMAGIC: %d\n", type & TYPEMASK != TYPEMAGIC);
	if(type & TYPEMASK != TYPEMAGIC) {
		printf("DBG_OUT: Type:0x%X, Value:0x%X\n", type, val);
		return;
	}
	if(type == TLBINDEX) {
		printf("DBG_OUT: TLBIndex: 0x%X\n", val);
		return;
	}
	if(type == TLBENTRY0) {
		printf("DBG_OUT: TLBEntry0: 0x%08X. PFN: 0x%X, C: 0x%X, D: 0x%X, V: 0x%X, G: 0x%X\n", 
			val,
			(val >> 6) & 0xFFFFFF,
			(val & 0x38)>> 3,
			(val & 0x4) >> 2,
			(val & 0x2) >> 1,
			(val & 0x1) 
		      );
return;
	}	
	if(type == TLBENTRY1) {
		printf("DBG_OUT: TLBEntry1: 0x%08X. PFN: 0x%X, C: 0x%X, D: 0x%X, V: 0x%X, G: 0x%X\n", 
			val,
			(val >> 6) & 0xFFFFFF,
			(val & 0x38)>> 3,
			(val & 0x4) >> 2,
			(val & 0x2) >> 1,
			(val & 0x1) 
		      );
		return;
	}	
	if(type == TLBENTRYHI) {
		printf("DBG_OUT: TLBEntryHi: 0x%08X. VPN2: 0x%X\n" ,val, val >> 13);
		return;
	}	
	if(type == PGMASK) {
		printf("DBG_OUT: PageMask: 0x%08X\n", val);
		return;
	}
	if(type == VAL) {
		printf("DBG_OUT: Value:0x%X\n", val);
		return;
	}
	if(type == TREG) {
		printf("DBG_OUT: Register:0x%X\n", val);
		return;
	}
	if(type == CFG0) {
		printf("DBG_OUT: Config register 0 value:0x%08X\n", val);
		return;
	}
	if(type == CFG1) {
		printf("DBG_OUT: Config register 1 value:0x%08X\n", val);
		return;
	}
	if(type == CFG2) {
		printf("DBG_OUT: Config register 2 value:0x%08X\n", val);
		return;
	}
	if(type == CFG3) {
		printf("DBG_OUT: Config register 3 value:0x%08X\n", val);
		return;
	}
	if(type == CFG4) {
		printf("DBG_OUT: Config register 4 value:0x%08X\n", val);
		return;
	}
	if(type == CFG5) {
		printf("DBG_OUT: Config register 5 value:0x%08X\n", val);
		return;
	}
	if(type == CFG6) {
		printf("DBG_OUT: Config register 6 value:0x%08X\n", val);
		return;
	}

}
